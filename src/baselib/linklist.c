
/**
 * 基于linux内核list_head的双向链表写成的链表，单向链表于是也用这个代替咯
 * 注意注意，这里是双向链表，理论上不存在什么头什么尾
 * 大概需要定义在
 * 1. 在给定的cur结点的前面，后面插入一个new_link_node结点
 * 2. 判空
 * 3. 基于遍历找到key值是某个值的结点（可能用宏实现）
 * 4. 初始化第一个头。。
 */


#include <stdint.h>
#include <string.h>

#include "linklist.h"
#include "instruct.h"
#include "gdef.h"


/*使用函数实现真正这个双向链表的第一个节点head的初始化*/
void init_list_head(link_head *link_node){
	link_node->prev=link_node;
	link_node->next=link_node;
}

/*在给定的cur结点后面插入new结点*/
void list_insert_back(link_head *cur_link_node,link_head *new_link_node){
	if(cur_link_node==NULL || new_link_node==NULL)	/*传入参数无值*/
		return ;

	/*将new结点插入到cur结点的后面,cur后面可能已经有结点，可能没有，不过都一样*/
	new_link_node->prev=cur_link_node;	/*先操作new，因为new的赋值需要用到cur当前的值*/
	new_link_node->next=cur_link_node->next;
	cur_link_node->next->prev=new_link_node;
	cur_link_node->next=new_link_node;

	return;
}

/*先给new结点，再给cur结点，把new结点插入到cur结点前面*/
void list_insert_front(link_head *cur_link_node,link_head *new_link_node){
	if(cur_link_node==NULL || new_link_node==NULL)	/*传入参数无值*/
		return ;

	new_link_node->next=cur_link_node;
	new_link_node->prev=cur_link_node->prev;
	cur_link_node->prev->next=new_link_node;
	cur_link_node->prev=new_link_node;

	return ;
}

/*删除给定结点,分为链表只有自己，有一个以上元素。但操作方式相同*/
void list_del(link_head *l){
	l->prev->next=l->next;
	l->next->prev=l->prev;
	return ;
}

/*传入这个l,可以是任何一个结点哦，如果不是NULL就好*/
signal_t list_empty(link_head *l){
	if(l==NULL)
		return FALSE;
	return TRUE;
}

/*链表中一个结点*/
signal_t list_isone(link_head *l){
	if(l==NULL)
		return FALSE;

	if(l->next==l && l->prev==l)	/*需要前后都是自己才行，光一个不行*/
		return TRUE;

	return FALSE;
}


/*链表中两个结点*/
signal_t list_istwo(link_head *l){
	if(l==NULL)
		return FALSE;

	if(l->next==l->prev && l->next!=NULL)
		return TRUE;

	return FALSE;
}

/*链表中三个结点*/
signal_t list_isthree(link_head *l){
	if(l==NULL || l->next==NULL || l->prev==NULL)
		return FALSE;

	if(l->next->next==l->prev)
		return TRUE;

	return FALSE;
}

/*链表中结点数大于1*/
signal_t list_isaboveone(link_head *l){
	if(l==NULL)
		return FALSE;
	if(!list_isone(l))
		return TRUE;
	return FALSE;
}

/*链表中结点树大于2*/
signal_t list_isabovetwo(link_head *l){
	if(l==NULL)
		return FALSE;
	if(!list_isone(l) && !list_istwo(l))
		return TRUE;
	return FALSE;
}

/*判断A结点后面是不是B结点*/
signal_t list_B_back_A(link_head *A,link_head *B){
	if(A==NULL || B==NULL)
		return FALSE;

	if(A->next==B && B->prev==A)
		return TRUE;

	return FALSE;
}

size_t list_len(link_head *l){
	if(l==NULL || l->next==NULL || l->prev==NULL)
		return ZERO;
	link_head *pos=NULL;
	size_t count=-1;
	for(count=0,pos=l->next;pos!=l;pos=pos->next)
		count++;

	return count+1;
}


/*遍历找到key值并返回这个结构的struct_entry操作之后的指针*/
void *locate_list_node(link_head *l){
	if(l==NULL)
		return NULL;

}

























