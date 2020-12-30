


#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

#include "gdef.h"
#include "mempool.h"
#include "array.h"
#include "linklist.h"
#include "zlog.h"

pool_t *create_pool(size_t size,log_t *log)
{
	LOG_INFO(log,"start register memory pool,size %d",size);
    pool_t *pool=NULL; /* 执行内存池头部 */

    /* 分配大小为 size 的内存 */
    posix_memalign(&pool,POOL_ALIGNMENT,size);
    if(pool==NULL){
        return NULL;
    }

    /* 以下是初始化 ngx_pool_t 结构信息 */

    pool->data.cur=(u_char *)pool+sizeof(pool_t);
    pool->data.end=(u_char *)pool+size;
    pool->data.available_size=size-sizeof(pool_t);	/*可用空间，直接比较*/
    pool->data.next=NULL;
    pool->data.failed=0;

    size=size-sizeof(pool_data_t);   /*可供分配的空间大小*/

    pool->size=size;
    pool->current=pool; /* 指向当前的内存池 */
    pool->tail=pool;
    pool->large=NULL;
    pool->log=log;
    pool->garcycle_chain_node=NULL;	/*注册此结点会从pool里面先选garcycle看有没有空间，此处不赋值null给他判断不能会出错*/
    LOG_INFO(log,"loop register garcycler array");
    reg_garcycle_chain(pool);	/*完成回收链表注册*/

    if(pool==NULL)
    	LOG_ERROR(log,"create pool error");
	LOG_INFO(log,"memory pool finish register");
    return pool;
}


/**
 * 从内存池pool中分配内存，size是需要的内存大小
 */

void *palloc(pool_t *pool,size_t size){
    u_char *m=NULL;
    pool_t *p=NULL;
    log_t *log=pool->log;
    size<=0 ? LOG_ERROR(log,"size %d, not proper size",size) : NULL;

    if(pool->garcycle_chain_node!=NULL)
    	if((m=garcycle_palloc(pool,size))!=NULL)	/*尝试从回收链表中分配*/
    		return m;

    if (size<=pool->size){	/*属于小内存分配*/
        p=pool->current;

        do{
            m=p->data.cur;	/*获取当前内存池可以提供分配的那个地方*/

            /*这个内存池空间够，就从这里分配，不然看下一个内存池能不能分配，再不然另外开辟内存池*/
            if ((size_t)(p->data.end-m)>=size) {
                p->data.cur= m+size;

                return m;
            }
            p->data.failed+=1;	/*该内存池分配失败次数+1,如果该值超过了POOL_FAIL_ALLOC,就要把current向后移动，代表这个已经是是白的了*/

            if(p->data.failed>POOL_FAIL_ALLOC){
            	assert(p->data.next!=NULL);	/*失败一次后面就一定有一个了，何况这里定的失败那么多次*/
            	p->current=p->data.next;
            }
            p=p->data.next;
        }while(p);

        /*遍历了所有的都不行，另行开辟一个新的内存池*/
        return palloc_block(pool,size);
    }

    return palloc_large(pool,size);	/*大小不满足小内存，进行大内存分配*/
}

static void *palloc_block(pool_t *pool,size_t size){
	log_t *log=pool->log;
	LOG_INFO(log,"new pool created");

    u_char *m=NULL;
    size_t psize=-1;
    pool_t *new=NULL;

    /* 计算pool的大小，即需要分配新的block的大小 */
    psize=(size_t)(pool->data.end-(u_char *) pool);

    /* NGX_POOL_ALIGNMENT对齐操作 */
    posix_memalign(&m,POOL_ALIGNMENT,psize);
    if (m==NULL){	/*分配失败*/
    	LOG_ERROR(log,"error malloc memory for new pool,memory not enough");
        return NULL;
    }

    /*初始化内存池*/
    new=(pool_t *)m;
    new->data.end=m+psize;
    new->data.next=NULL;
    new->data.failed=0;

    /* 让m指向该块内存pool_data_t结构体之后数据区起始位置 */
    m += sizeof(pool_data_t);

    new->data.cur=m+size;	/*开辟这块内存池本质是想要去分配空间，于是要进行一次分配*/

    /* 将分配的block连接到现有的内存池  */
    pool->tail->data.next=new;
    pool->tail=new;

    return m;
}


/* 分配大块内存 */
static void *palloc_large(pool_t *pool, size_t size){
    void *p=NULL;;
    uint32_t n;
    pool_large_t *large=NULL;;
    log_t *log=NULL;

    log=pool->log;
    /*malloc内存分配*/
    posix_memalign(&p,POOL_ALIGNMENT,size);
    if(p==NULL){
    	LOG_ERROR(log,"error malloc memory for large pool");
        return NULL;
    }
    n=0;
    /* 若在该pool之前已经分配了large字段，
     * 则将所分配的大块内存挂载到内存池的large字段中
     */
    for(large=pool->large;large;large=large->next){
        if(large->start==NULL){
            large->start=p;
            return p;
        }
        if (n++>3)
            break;
    }

    /* 若在该pool之前并未分配large字段，
     * 则执行分配pool_large_t结构体分配large字段内存，
     * 再将大块内存挂载到pool的large字段中
     */
    large=palloc(pool,sizeof(pool_large_t));
    if(large==NULL)
    	return NULL;
    large->start=p;
    large->size=size;
    large->next=pool->large;	/*插在链表的头部*/
    pool->large=large;

    return p;
}

/**
 * 回收空间，需要传入需要释放的空间大小，直接通过哈希函数找到pool的garchain数组对应的链表
 * 把这个空间连到对应链表上
 * 形式上需要信任传入的memsize大小，但是事实上可能越界
 */
void pfree(pool_t *pool,void *start,memsize_t memsize){
	log_t *loglog=pool->log;
	if(start==NULL)
		return LOG_ERROR(loglog,"invalid pointer");
	if(memsize<100)
		return ;

	garcycle_data_t *garcycle_data=NULL;
	array_t *garcycle_chain=NULL;
	size_t garcycle_chain_len=-1;
	sequence_t sequence=-1;
	link_head *link_node=NULL;
	garcycle_t *garcycle_head=NULL;


	/*进行结点的注册*/
	garcycle_data=(garcycle_data_t *)palloc(pool,sizeof(garcycle_data_t));
	if(garcycle_data==NULL)
		return ;
	garcycle_data->start=start;
	garcycle_data->memsize=memsize;

	/*通过哈希函数找到数组里面对应的链表*/
	/*0->100--200 , 1->200--400 , 2->400--800 , 3->800--1600 , 大概如此规律*/
	/*含义大概是memsize在100-200的接在第一个链表，在200-400的接在第二个链表，直接忽略100以下*/
	garcycle_chain=(array_t *)struct_entry(pool->garcycle_chain_node,array_data_t,link_node);	/*garcycle_chain是一个array_t*/
	garcycle_chain_len=array_len(garcycle_chain);
	/*sequence是所寻找链表在array中的次序，以下运算类似于一个哈希函数*/
	sequence=(sequence_t)garcycle_sequence_hash(memsize);
	assert(sequence>=0);

	sequence>garcycle_chain_len-1 ? sequence=garcycle_chain_len-1 : NULL;	/*内存大小超过自定义的step，存在最后一个链表*/
	/*array_get(...)函数得到链表头*/
	link_node=*((link_head **)array_get(garcycle_chain,sequence));
	list_add(link_node,&(garcycle_data->link_node));
	garcycle_head=(garcycle_t *)struct_entry(link_node,garcycle_t,link_node);
	garcycle_head->sum+=1;

	return ;
}

static void *garcycle_palloc(pool_t *pool,size_t size){
	link_head *link_node=NULL;
	array_t *garcycle_chain=NULL;
	sequence_t sequence=-1;
	garcycle_t *garcycle_head=NULL;
	garcycle_data_t *data=NULL;

	link_node=pool->garcycle_chain_node;	/*array_t的link_node*/
	garcycle_chain=(array_t *)struct_entry(pool->garcycle_chain_node,array_data_t,link_node);
	sequence=(sequence_t)garcycle_sequence_hash(size);
	if(sequence<0  || sequence>=array_len(garcycle_chain)-1)
		return NULL;

	link_node=*((link_head **)array_get(garcycle_chain,sequence+1));
	garcycle_head=(garcycle_t *)struct_entry(link_node,garcycle_t,link_node);
	if(garcycle_head->sum<=0)
		return NULL;
	link_node=link_node->next;
	list_del(link_node);
	data=(garcycle_data_t *)struct_entry(link_node,garcycle_data_t,link_node);
	return data->start;
}

static sequence_t garcycle_sequence_hash(memsize_t size){
	return (log((int)(size/100))/log(2));
}

static void pfree_large(pool_t *pool,void *start){
	pool_large_t *large=NULL,*l=NULL,*prev=NULL;
	large=pool->large;
	if(large==NULL)	/*未创建任何一个大内存空间*/
		return ;

	if(large->start==start){	/*对链表头结点的处理*/
		free(start);
		free(large);
		return ;
	}

	for(l=large;l;prev=l,l=l->next){
		if(l->start==start){
			prev->next=l->next;
			free(start);
			free(l);
		}
	}

	return ;
}

static void reg_garcycle_chain(pool_t *pool){
	array_t *garcycle_chain=NULL;
	signal_t sig=-1;
	garcycle_t *garcycle_head=NULL;
	memsize_t memsizes[]=MEMSIZE_STEP;	/*数组回收大小阶梯数组，用来初始化garchain*/
	size_t memsizes_len=sizeof(memsizes)/sizeof(memsize_t);
	link_head *link_node=NULL;
	assert(memsizes_len>1);

	garcycle_chain=init_array(pool,sizeof(link_head *));
	if(garcycle_chain==NULL)
		return ;

	for(sig=0;sig<memsizes_len-1;sig++){	/*循环向数组push每个链表头结点的指针,每个头结点都进行一遍初始化*/
		garcycle_head=(garcycle_t *)palloc(pool,sizeof(garcycle_t));
		if(garcycle_head==NULL)	/*空间分配失败*/
			return ;

		garcycle_head->minsize=memsizes[sig];
		garcycle_head->maxsize=memsizes[sig+1];
		garcycle_head->sum=0;

		link_node=&(garcycle_head->link_node);
		init_list_head(link_node);
		array_push(garcycle_chain,&link_node);
	}

	pool->garcycle_chain_node=&(garcycle_chain->data.link_node);	/*链上pool结构，完成注册*/
	return ;
}














