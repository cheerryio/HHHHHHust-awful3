

#ifndef LINKLIST_H_
#define LINKLIST_H_

#include "gdef.h"

/*对头结点（第一个声明的，用来让prev和next指向自己*/
#define LINK_HEAD_INIT(name) {&(name),&(name)}

/*创建了一个名叫name的list_head结构，是实体创建，没有用指针  malloc*/
#define LINK_HEAD(name) \
	struct link_head name=LINK_HEAD_INIT(name)

/*-------------------------------------------------*/
/*把container_of换了一个名字。。目的是去找*/
#define struct_entry(ptr,type,member)	\
	container_of(ptr,type,member)

/*计算member成员在结构体里面的偏移量，可以是任何成员哦*/
#define offsetof(TYPE,MEMBER) ((size_t)&((TYPE *)0)->MEMBER)

/*
 * __mptr是这个结构里面list_head实际的地址
 * offsetof计算了list_head在结构里面的偏移量，是一个相对的地址，
 * 二者相减计算得到了这个结构的地址
 */
#define container_of(ptr,type,member)	\
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/*-----------------------------------------------------------*/

/*判断A,B结点的相对位置关系*/
#define list_isfront(A,B) list_B_back_A((A),(B))
#define list_isback(A,B) list_B_back_A((A),(B))
#define	list_add(A,B) list_insert_front((A),(B))
#define init_listhead(A)  init_list_head((A))



typedef struct link_head_s link_head;
struct link_head_s {
	struct link_head_s *prev;
	struct link_head_s *next;
};


void init_list_head(link_head *link_node);
void list_insert_back(link_head *cur_link_node,link_head *new_link_node);
void list_del(link_head *l);
signal_t list_empty(link_head *l);
signal_t list_isone(link_head *l);
signal_t list_istwo(link_head *l);
signal_t list_isthree(link_head *l);
signal_t list_isaboveone(link_head *l);
signal_t list_isabovetwo(link_head *l);
signal_t list_B_back_A(link_head *A,link_head *B);
size_t list_len(link_head *l);
void *locate_list_node(link_head *l);
















#endif
