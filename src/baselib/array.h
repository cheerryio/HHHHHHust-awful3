


/**
 * 封装array数组
 *
 * 梗概：大概还是要分出array_t 和 array_data_t, array_t是跟pool_t一样的头，声明
 * 一个数组就只有一个
 * array_data_t多个，主要是用来空间不够就去扩充，里面会记录可以存多少个，已经存了多少个
 */




#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdint.h>

#include "gdef.h"
#include "linklist.h"
#include "mempool.h"


#define ARRAY_BASE_SIZE 32


typedef struct array_s array_t;
typedef struct array_data_s array_data_t;

struct array_data_s {
	void *start;
	void *cur;
	void *end;
	size_t bysize;	/*一个元素的字节大小*/
	size_t size;	/*该空间已经存了多少元素*/
	size_t max;	/*该空间共可以存多少个元素*/
	link_head link_node;	/*link_head加入做双向循环链表*/
};

struct array_s {
	array_data_t data;	/*该存储空间的主要数据*/
	size_t base_size;	/*一个存储块存的元素个数*/
	size_t elem_sum;
	size_t block_sum;
	size_t frontpop_sum;
	pool_t *pool;
	link_head *tail_node;	/*末尾空间的地址,希望概念同意，记录的是这个link_node的地址*/
};


array_t *init_array(pool_t *pool,size_t bysize);	/*初始化一个数组头*/
void array_assign(array_t *a,char *elems[],size_t len);
void *array_get(array_t *a,size_t sequence);	/*拿出第几个元素*/
void *array_getlast(array_t *a);					/*基于array_get获得最后一个元素*/
void array_set(array_t *a,sequence_t sequence,void *elem);	/*更改完整元素内容*/
void array_push(array_t *a,void *elem);	/*从末尾push进一个元素*/
void *array_pop(array_t *a);	/*从末尾拿出一个元素*/
size_t array_len(array_t *a);	/*返回数组中元素个数 */
signal_t array_in(array_t *a,void *elem);	/*元素elem在数组中么*/
void array_free(array_t *a);	/*释放数组空间*/
void array_fresh(array_t *a);	/*让一个数组回到开始刚初始化时的状态*/
signal_t array_empty(array_t *a);	/*判断array元素是否0*/
sequence_t array_find(array_t *a,void *elem);	/*给出元素次序，否则返回-1*/
/*--------------内部方法-----------------*/

/*空间不足，另行开辟空间*/
static void array_block(array_t *a,void *elem);






#endif /* ARRAY_H_ */











