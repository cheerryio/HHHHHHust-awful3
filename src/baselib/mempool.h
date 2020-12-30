

#ifndef MEMPOOL_H_
#define MEMPOOL_H_

#include <stdint.h>

#include "gdef.h"
#include "linklist.h"
#include "zlog.h"



#define POOL_ALIGNMENT 16
#define POOL_FAIL_ALLOC 4
#define MEMSIZE_STEP {100,200,400,800,1600,3200,6400,128000,256000,-1}

typedef struct pool_s pool_t;
typedef struct pool_large_s pool_large_t;
typedef struct pool_data_s pool_data_t;
typedef struct garcycle_s garcycle_t;
typedef struct garcycle_data_s garcycle_data_t;


struct pool_data_s {
    u_char *cur; /* 当前内存分配的结束位置，即下一段可分配内存的起始位置 */
    u_char *end;  /* 内存池的结束位置 */
    size_t available_size;
    pool_t *next; /* 指向下一个内存池 */
    uint8_t failed;	/* 记录内存池内存分配失败的次数 */
};

struct pool_large_s {
	void *start;	/*指向分配的大块内存*/
	size_t size;
	pool_large_t *next;    /*指向下一块大块内存*/
};


/**关于内存池pool的回收部分，有一个数组chain，每一个元素是100,200,400,800等下限内存空间
 * 的链表的头，因为是线性，直接用哈希函数找位置。
 *
 * garcycle_t是chain中每一个元素，garcycle_data_t是链表里面的每一个元素
 */
struct garcycle_s {
	memsize_t minsize;	/*该链表回收的内存下限*/
	memsize_t maxsize;	/*回收的内存上限*/
	size_t sum;	/*该链表上有的内存个数*/
	link_head link_node;	/*作为一个链表的头结点，此link_node指向garcycle_data_t而不是garcycle_t*/
};

struct garcycle_data_s {
	void *start;
	memsize_t memsize;	/*该回收内存的大小*/
	link_head link_node;
};


/*内存池的头部结构，整个程序的内存只有这一个头，可以有很多小的内存池*/
struct pool_s {
    pool_data_t data;    /* 内存池的数据块 */
    size_t size;  /* 每个小内存池可供容纳的大小，用此值判断是分配小内存还是单独开辟大内存 */
    pool_t *current;	/* 指向当前内存池 */
    pool_t *tail;
    pool_large_t *large;	/* 大块内存链表，即需要分配空间超过 size大小 的内存 */
    link_head *garcycle_chain_node;	/*内存回收的数组，因为不能声明array_t结构，此处用void*代替*/
    log_t *log;	/* 内存分配相关的日志信息 */
};



/*创建内存池*/
pool_t *create_pool(size_t size,log_t *log);
/*从池中分配 size大小 的内存*/
void *palloc(pool_t *pool,size_t size);
/*释放空间,将欲释放的空间的信息连在回收链表上*/
void pfree(pool_t *pool,void *start,memsize_t memsize);

/*内部函数*/
static void *palloc_block(pool_t *pool,size_t size);
static void *palloc_large(pool_t *pool,size_t size);
static void pfree_large(pool_t *pool,void *start);
static void reg_garcycle_chain(pool_t *pool);	/*注册回收链表*/
static void *garcycle_palloc(pool_t *pool,size_t size);
static sequence_t garcycle_sequence_hash(memsize_t size);



#endif
