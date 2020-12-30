


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "gdef.h"
#include "mempool.h"
#include "linklist.h"
#include "array.h"



/*初始化一个数组
 *1. 申请空间
 *2. 赋值各个属性
 *3. 把list_head链表弄好
 */
array_t *init_array(pool_t *pool,size_t bysize){

	if(bysize<=0)	/*不能有虚空*/
		return NULL;

	array_t *a=NULL;	/*最终会返回初始化成功的数组头*/

	a=(array_t *)palloc(pool,sizeof(array_t)+bysize*ARRAY_BASE_SIZE);	/*出去头外的是真是存储空间*/
	if(a==NULL)	/*分配失败*/
		return NULL;

	init_list_head(&(a->data.link_node));	/*初始化双向链表第一个结点*/
	a->data.start=(void *)a+sizeof(array_t);	/*真是存储空间起始地址*/
	a->data.cur=(void *)a+sizeof(array_t);
	a->data.end=(void *)a+sizeof(array_t)+bysize*ARRAY_BASE_SIZE;
	a->data.bysize=bysize;
	a->data.size=0;	/*这个size表已经存了多少个*/
	a->data.max=ARRAY_BASE_SIZE;

	a->pool=pool;
	a->base_size=ARRAY_BASE_SIZE;
	a->elem_sum=ZERO;
	a->block_sum=ONE;
	a->frontpop_sum=ZERO;
	a->tail_node=&(a->data.link_node);

	return a;
}

/*用二位数组初始化*/
/**
 * 对刚刚才初始化的数组，传入字符串数组，即二维数组，对array_t进行赋值
 * 如果对数组的cur!=start,或者后面有多开辟的空间，调用array_fresh(...)
 * 让数组变成原来的初始状态，再进行遍历赋值
 */
void array_assign(array_t *a,char *elems[],size_t len){
	if(elems==NULL || len<=0)
		return ;
	array_fresh(a);	/*初始化数组*/

	signal_t sig=-1;
	for(sig=0;sig<len;sig++)
		array_push(a,elems[sig]);

	return ;
}


/**输入数字次序，返回这个次序元素的首地址
 * 数字需要满足上限和下限，就是大于0小于数组总元素个数
 * 需要跨越存储块查找，这里只能用循环遍历，由于双向链表，只要能计算出遍历个数，就能够决定是向前遍历快还是向后遍历快
 */
void *array_get(array_t *a,size_t sequence){
	if(a==NULL)
		return NULL;

	size_t seq=-1;
	size_t base_size=a->base_size;	/*一个存储块可以存元素个数*/
	size_t elem_sum=a->elem_sum;
	size_t block_sum=a->block_sum;	/*多少个存储块*/
	size_t frontpop_sum=a->frontpop_sum;	/*被frontpop操作了多少次*/
	size_t block_sequence=-1,block_minus=-1,rel_sequence=-1;
	size_t sig=-1,block_move_sig=-1;
	link_head *link_node=NULL;
	array_data_t *target_a=NULL;	/*目标元素所在的存储块*/
	void *res_ptr=NULL;

	seq=sequence+frontpop_sum;	/*默认进行了frontpop操作之后不会再进行其他奇怪的操作,两者相加得到seq即需要元素的实际物理位置次序*/
	if(sequence<0 || sequence>elem_sum-1)	/*传入次序不满足数组元素个数上限下限*/
		return NULL;

	/*把seq看成根本没有frontpop操作的数组里面的次序即可*/
	block_sequence=seq/base_size;	/*从0开始，跟数组从0开始的下标一样计数*/
	block_minus=block_sum-block_sequence-1;
	assert(block_sequence>=0 && block_minus>=0);

/*遍历双向链表，用prev或next快速找到目标存储块。prev与next需要复用同样代码，用define减少代码重用*/
#define FIND_TARGET_PTR(DIRECTION)	\
	for(sig=0,link_node=&(a->data.link_node);sig<block_move_sig;link_node=link_node->DIRECTION,sig++)	\
		if(link_node==NULL)	\
			return NULL;	\
	rel_sequence=seq-block_sequence*base_size;	\
	target_a=(array_data_t *)struct_entry(link_node,array_data_t,link_node);	\
	assert(rel_sequence>=0);	\
	res_ptr=target_a->start+(frontpop_sum+rel_sequence)*target_a->bysize;

	if(block_minus>=block_sequence){		/*例如3-1>1,用next找*/
		block_move_sig=block_sequence;
		FIND_TARGET_PTR(next);	/*此时res_ptr变量得到需要返回的结果*/
	}
	else{	/*用prev找*/
		block_move_sig=block_sum-block_sequence;
		FIND_TARGET_PTR(prev);	/*res_ptr得到需要return的结果*/
	}

	return res_ptr;
}

/*返回最后一个元素的指针，基于array_get*/
void *array_getlast(array_t *a){
	void *res_ptr=NULL;
	res_ptr=array_get(a,a->elem_sum-1);
	return res_ptr;
}

/**
 * 调用array_get得到目的地的指针，再用memcopy改变元素值
 */
void array_set(array_t *a,sequence_t sequence,void *elem){
	if(sequence==NULL || elem==NULL)
		return ;

	/*获取目的元素指针*/
	void *cur=NULL,*dest=NULL,*src=NULL;
	cur=array_get(a,sequence);
	if(cur==NULL)
		return ;

	dest=cur;src=elem;
	memcpy(dest,src,a->data.bysize);

	return ;

}


/*实现array的push操作，也是入栈操作，也是向数组末尾添加一个元素操作
 * 大致思路是，找到tail标示的最后一个空间，如果空间最后一个元素被占，就另行开闭空间
 * 分配元素
 *
 * 本次会使用到memcopy 复制指定地址后面指定字节到某个地方
 *
 * 注意空间不够时需要另行开辟，头中tail需要变动，新生成空间的
 */
void array_push(array_t *a,void *elem){
	if(a==NULL || elem==NULL)
		return ;

	link_head *head_node=&(a->data.link_node);
	link_head *tail_node=a->tail_node;	/*tail空间的link_node指针*/
	array_data_t *tail_a=NULL;
	void *dest=NULL,*src=NULL;
	size_t bysize=-1;

	tail_a=(array_data_t *)struct_entry(tail_node,array_data_t,link_node);
	if(tail_a->size<tail_a->max){	/*这里空间还足够加入一个元素*/
		dest=tail_a->cur;
		src=elem;
		bysize=tail_a->bysize;
		assert(bysize>0);
		memmove(dest,src,bysize);

		/*成功push元素，改变各属性值*/
		tail_a->cur=tail_a->cur+bysize;
		tail_a->size=tail_a->size+1;
		a->elem_sum+=1;
	}
	else
		/*这里的空间不够加入一个元素，但是后面没有一片开辟好的空间*/
		if(list_isback(tail_node,head_node))
			return array_block(a,elem);
		else{	/*后面有开辟好的空间，并且完全空，一个元素都没有*/
			a->tail_node=tail_node->next;
			return array_push(a,elem);	/*调用自己push元素*/
		}

	return ;
}


/**数组存储空间不够，这里需要再自动分配空间
 * 分配的空间需要链到link_node上
 *
 */
static void array_block(array_t *a,void *elem){
	/*push时内部调用，传入的参数都已经验证有效*/

	pool_t *pool=NULL;
	array_data_t *tail_a=NULL;	/*接收尾arr*/
	array_t *new_a=NULL;
	size_t bysize=-1;
	link_head *tail_node=NULL;
	void *dest=NULL,*src=NULL;

	pool=a->pool;
	tail_node=a->tail_node;
	tail_a=(array_data_t *)struct_entry(tail_node,array_data_t,link_node);
	bysize=tail_a->bysize;

	assert(tail_a->cur==tail_a->end || tail_a->size==tail_a->max);
	assert(bysize>0);

	new_a=(array_t *)palloc(pool,sizeof(array_data_t)+bysize*ARRAY_BASE_SIZE);
	if(new_a==NULL)
		return ;

	new_a->data.start=(void *)new_a+sizeof(array_data_t);
	new_a->data.cur=(void *)new_a+sizeof(array_data_t);
	new_a->data.end=(void *)new_a+sizeof(array_data_t)+bysize*ARRAY_BASE_SIZE;
	new_a->data.bysize=bysize;
	new_a->data.size=0;
	new_a->data.max=ARRAY_BASE_SIZE;

	list_insert_back(tail_node,&(new_a->data.link_node));

	/*新空间注册完成*/
	/*开始push元素*/

	dest=new_a->data.cur;
	src=elem;
	memcpy(dest,src,bysize);
	new_a->data.cur=dest+bysize;
	new_a->data.size+=1;

	a->elem_sum+=1;		/*元素个数+1*/
	a->block_sum+=1;	/*存储块+1*/
	a->tail_node=&(new_a->data.link_node);

	return ;

}


/**a->tail指向的块一定是我直接在那一块取的块，这个理念在我 开始取 和 取之后改变值 都要用到！
 * 函数开头先要判断array里面还有没有元素
 * 如果tail指向块中仅有一个元素，把这个元素取出来之后，tail需要指向前一个块。
 * 如果array里面一个元素都没有了，tail指向不动
 */
void *array_pop(array_t *a){
	if(a==NULL || a->elem_sum<=0)	/*array中无元素也要返回NULL*/
		return NULL;
	link_head *tail_node=NULL;
	array_data_t *tail_a=NULL;
	void *cur=NULL;
	size_t bysize=-1;
	void *elem_pop=NULL;
	pool_t *pool=NULL;
	void *dest=NULL,*src=NULL;

	pool=a->pool;
	tail_node=a->tail_node;
	tail_a=(array_data_t *)struct_entry(tail_node,array_data_t,link_node);
	assert(tail_a->cur!=tail_a->start);	/*此空间一定至少有一个元素*/
	assert(tail_a->size>0);
	assert(bysize>0);
	bysize=tail_a->bysize;
	elem_pop=(void *)palloc(pool,bysize);
	if(elem_pop==NULL)
		return NULL;
	tail_a=(array_data_t *)struct_entry(tail_node,array_data_t,link_node);
	assert(tail_a->cur!=tail_a->start);	/*此空间一定至少有一个元素*/
	assert(tail_a->size>0);

	cur=tail_a->cur;

	elem_pop=(void *)palloc(pool,bysize);
	if(elem_pop==NULL)
		return NULL;
	cur=cur-bysize;
	assert(cur>=tail_a->start);
	dest=elem_pop;src=cur;
	memcpy(dest,src,bysize);

	tail_a->cur=cur;
	tail_a->size-=1;
	a->elem_sum-=1;

	if(tail_a->size==0 || tail_a->cur==tail_a->start)	/*该空间没有元素*/
		if(tail_node!=&(a->data.link_node))	/*该空间不是头空间*/
			a->tail_node=tail_node->prev;

	return elem_pop;

}

/*元素elem是否在数组a中*/
signal_t array_in(array_t *a,void *elem){
	if(elem==NULL)
		return FALSE;

	size_t len=array_len(a);
	size_t bysize=a->data.bysize;
	signal_t sig=-1;
	void *r=NULL;

	for(sig=0;sig<len;sig++){
		r=array_get(a,sig);
		if(r==NULL)
			return FALSE;

		if(memcmp(r,elem,bysize)==ZERO)
			return TRUE;
	}

	return FALSE;
}

/*array本质是返回存储空间的一个指针，此函数给出这个指针，并返回这个指针处在的次序*/
sequence_t array_find(array_t *a,void *elem){
	if(elem==NULL)
		return -1;
	size_t len=array_len(a);
	size_t bysize=a->data.bysize;
	signal_t sig=-1;
	signal_t findsig=FALSE;
	void *r=NULL;
	for(sig=0;sig<len;sig++){
		r=array_get(a,sig);
		assert(r!=NULL);	/*sig<elem_sum,一定会返回指针*/
		if(memcmp(r,elem,bysize)==ZERO){
			findsig=TRUE;
			break;
		}
	}
	if(findsig==FALSE)
		sig=-1;
	return sig;
}

/*返回数组元素个数*/
size_t array_len(array_t *a){
	if(a==NULL || a->elem_sum==NULL)
		return ZERO;
	return a->elem_sum;
}

/**
 * 调用pfree函数释放数组空间，需要计算每一个存储块加上双子结构的大小，并循环释放
 */
void array_free(array_t *a){
	if(a==NULL)
		return ;

	size_t base_size=-1,bysize=-1,block_sum=-1;
	pool_t *pool=NULL;
	link_head *head=NULL,*pos=NULL;
	void *ff=NULL;
	pool=a->pool;
	head=&(a->data.link_node);
	base_size=a->base_size;
	bysize=a->data.bysize;
	block_sum=a->block_sum;

	/**数组头是array_t+存储空间，单独释放
	 * 其后的是array_data_t加存储空间，循环释放
	 * 释放其实只是把这个空间的信息连在一个链表上
	 */
	pfree(pool,(void *)a,sizeof(array_t)+bysize*base_size);	/*释放数组头*/
	for(pos=head->next;pos!=head;pos=pos->next){
		ff=(void *)struct_entry(pos,array_data_t,link_node);
		pfree(pool,ff,sizeof(array_data_t)+bysize*base_size);
	}

	return ;
}

/*让数组回到初始状态*/
void array_fresh(array_t *a){
	link_head *head=NULL,*pos=NULL;
	pool_t *pool=NULL;
	memsize_t memsize=-1;

	pool=a->pool;
	head=&(a->data.link_node);
	memsize=sizeof(array_data_t)+a->data.bysize*a->base_size;
	/*后面有开辟的存储空间，需要free*/
	if(!list_isone(head))
		for(pos=head->next;pos!=head;pos=pos->next)
			pfree(pool,(void *)struct_entry(pos,array_data_t,link_node),memsize);

	a->data.cur=a->data.start;	/*不判断了直接赋值*/
	init_list_head(head);
	a->elem_sum=ZERO;
	a->block_sum=ONE;
	a->frontpop_sum=ZERO;
	a->tail_node=head;
	a->data.size=ZERO;

	return ;
}

signal_t array_empty(array_t *a){
	if(a==NULL)
		return TRUE;
	if(a->elem_sum<1)
		return TRUE;
	return FALSE;
}

























