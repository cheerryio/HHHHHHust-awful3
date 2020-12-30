


/**
 * 建立trei字符串检索树，大概是传入一个string，把这个单词加入到树上
 * 实现的功能大概是
 * 1. 加入一个新单词
 * 2. 判断此单词是否已经在树中
 * 3. 为末尾字符结点添加 是关键词属性bool
 * 4. 为不是末尾字符添加 不是关键词属性bool
 * 5. 从一个结点开始判断是否有下一个字符 c
 *
 *
 *
 */

#ifndef BASELIB_TREI_C_
#define BASELIB_TREI_C_

#include "gdef.h"
#include "ptree.h"
#include "string.h"

typedef ptree_root_t trei_root_t;
typedef ptree_node_t trei_node_t;
typedef struct trei_data_s trei_data_t;

struct trei_data_s {
	bool isword;
	char c;
};

#define trei_root_init(_pool,_log)	\
	ptree_root_init((_pool),(_log),trei_data_t);
/*初始化结点是传入字符c*/
#define trei_node_init(pool,_c)	({	\
	trei_node_t *TREI_NODE_INIT_node=NULL;	\
	trei_data_t *TREI_NODE_INIT_elem=NULL;	\
	TREI_NODE_INIT_node=ptree_node_init(pool);	\
	TREI_NODE_INIT_elem=(trei_data_t *)palloc((pool),sizeof(trei_data_t));	\
	TREI_NODE_INIT_elem->c=*(_c);	\
	TREI_NODE_INIT_elem->isword=FALSE;	\
	ptree_assign_elem(TREI_NODE_INIT_node,TREI_NODE_INIT_elem);	\
	TREI_NODE_INIT_node;	\
	})
/*加入一个字符结点*/
#define trei_addchild(trei_parent,trei_child)	\
	ptree_addchild((trei_parent),(trei_child));
/*删除一个字符结点*/
#define trei_delnode(node)	\
	ptree_delnode(node);
/*加入一个单词,传入根节点和string_t结构单词*/
#define trei_addword(_root,_word) ({	\
	signal_t endsig=FALSE;	\
	trei_root_t *TREI_ADDWORD_root=(_root);	\
	char *TREI_ADDWORD_word=(_word);	\
	pool_t *pool=TREI_ADDWORD_root->pool;	\
	char *TREI_ADDWORD_c=NULL;	\
	signal_t sig=-1;	\
	trei_node_t *TREI_ADDWORD_node=NULL;	\
	link_head *TREI_ADDWORD_linknode1=NULL,*TREI_ADDWORD_linknode2=NULL;	\
	if((_root)==NULL)	\
		return ABNORMAL;	\
	if(strlen(_word)<1)	\
		endsig=TRUE;	\
	if(endsig==FALSE){	\
		TREI_ADDWORD_linknode1=&(TREI_ADDWORD_root->child);	\
		string_t *TREI_ADDWORD_s=String(pool,TREI_ADDWORD_word);	\
		TREI_ADDWORD_c=string_fetchs(TREI_ADDWORD_s);	\
		for(sig=0;sig<string_len(TREI_ADDWORD_s);sig++){	\
			TREI_ADDWORD_c=(char *)string_get(TREI_ADDWORD_s,sig);	\
			TREI_ADDWORD_node=trei_node_init(pool,TREI_ADDWORD_c);	\
			TREI_ADDWORD_linknode2=&(TREI_ADDWORD_node->love);	\
			ptree_ex_addchild_linknode(TREI_ADDWORD_linknode1,TREI_ADDWORD_linknode2);	\
			TREI_ADDWORD_linknode1=&(TREI_ADDWORD_node->child);	\
			}	\
		((trei_data_t *)(TREI_ADDWORD_node->elem))->isword=TRUE;	\
	}	\
	TREI_ADDWORD_node;	\
	})
/*判断node是否有letter值为c的孩子,node可能是root可能是node。失败返回NULL*/
#define trei_findchild(_node,_c) ({	\
	signal_t endsig=FALSE;	\
	signal_t findsig=FALSE;	\
	trei_node_t *TREI_FINDCHILD_node=NULL;	\
	trei_data_t *TREI_FINDCHILD_elem=NULL;	\
	link_head *TREI_FINDCHILD_linknode=NULL,*TREI_FINDCHILD_pos=NULL;	\
	if((_node)==NULL || (_c)==NULL)	\
		endsig=TRUE;	\
	if(endsig==FALSE){	\
		char TREI_FINDCHILD_c=*(_c);	\
		TREI_FINDCHILD_linknode=&((_node)->child);	\
		for(TREI_FINDCHILD_pos=TREI_FINDCHILD_linknode->next;TREI_FINDCHILD_pos!=TREI_FINDCHILD_linknode;TREI_FINDCHILD_pos=TREI_FINDCHILD_pos->next){	\
			TREI_FINDCHILD_node=(trei_node_t *)struct_entry(TREI_FINDCHILD_pos,trei_node_t,love);	\
			TREI_FINDCHILD_elem=(trei_data_t  *)(TREI_FINDCHILD_node->elem);	\
			if(TREI_FINDCHILD_elem->c==TREI_FINDCHILD_c){	\
				findsig=TRUE;	\
				break;	\
			}	\
		}	\
		if(findsig==FALSE)	\
		TREI_FINDCHILD_node=NULL;	\
	}	\
	TREI_FINDCHILD_node;	\
	})
/*判断此node结点是否是单词末尾*/
#define trei_iswordend(_node) ({	\
	signal_t endsig=FALSE;	\
	signal_t iswordend=FALSE;	\
	if((_node)==NULL)	\
		endsig=TRUE;	\
	if(endsig==FALSE){	\
		trei_node_t *TREI_ISWORDEND_node=(_node);	\
		trei_data_t *TREI_ISWORDEND_elem=(trei_data_t *)(TREI_ISWORDEND_node->elem);	\
		if(TREI_ISWORDEND_elem->isword==TRUE)	\
			iswordend=TRUE;	\
	}	\
	iswordend;	\
	})
/*判断给定单词是否在trei树中,传入参数的名字与声明的临时变量不要重名！！！*/
#define trei_iswordin(_root,_word) ({	\
	if((_root)==NULL || (_word)==NULL)	\
		return ABNORMAL;	\
	signal_t iswordin=FALSE;	\
	signal_t sig=0;	\
	char *TREI_ISWORDIN_pc=NULL;	\
	trei_node_t *TREI_ISWORDIN_node=NULL;	\
	\
	char *TREI_ISWORDIN_word=(_word);	\
	trei_root_t *TREI_ISWORDIN_root=(_root);	\
	pool_t *TREI_ISWORDIN_pool=TREI_ISWORDIN_root->pool;	\
	string_t *TREI_ISWORDIN_s=String(TREI_ISWORDIN_pool,TREI_ISWORDIN_word);	\
	TREI_ISWORDIN_pc=(char *)string_get(TREI_ISWORDIN_s,sig);	\
	TREI_ISWORDIN_node=trei_findchild(TREI_ISWORDIN_root,TREI_ISWORDIN_pc);	\
	if(TREI_ISWORDIN_node==NULL)	\
		NULL;	\
	else{	\
		for(sig=ONE;sig<string_len(TREI_ISWORDIN_s);sig++){	\
			TREI_ISWORDIN_pc=(char *)string_get(TREI_ISWORDIN_s,sig);	\
			TREI_ISWORDIN_node=trei_findchild(TREI_ISWORDIN_node,TREI_ISWORDIN_pc);	\
			if(TREI_ISWORDIN_node==NULL)	\
				break;	\
			if(trei_iswordend(TREI_ISWORDIN_node) && sig==string_len(TREI_ISWORDIN_s)-1){	\
				iswordin=TRUE;	\
				break;	\
			}	\
		}	\
	}	\
	string_free(TREI_ISWORDIN_s);	\
	iswordin;	\
})
/*给定单词，如果单词在树中，返回末尾字符结点地址，否则返回NULL*/
#define trei_findword(_root,_word) ({	\
	if((_root)==NULL || (_word)==NULL)	\
		return ABNORMAL;	\
	signal_t findsig=FALSE;	\
	signal_t sig=0;	\
	char *TREI_FINDWORDEND_pc=NULL;	\
	trei_node_t *TREI_FINDWORDEND_node=NULL;	\
	\
	char *TREI_FINDWORDEND_word=(_word);	\
	trei_root_t *TREI_FINDWORDEND_root=(_root);	\
	pool_t *TREI_FINDWORDEND_pool=TREI_FINDWORDEND_root->pool;	\
	string_t *TREI_FINDWORDEND_s=String(TREI_FINDWORDEND_pool,TREI_FINDWORDEND_word);	\
	TREI_FINDWORDEND_pc=(char *)string_get(TREI_FINDWORDEND_s,sig);	\
	TREI_FINDWORDEND_node=trei_findchild(TREI_FINDWORDEND_root,TREI_FINDWORDEND_pc);	\
	if(TREI_FINDWORDEND_node==NULL)	\
		NULL;	\
	else{	\
		for(sig=ONE;sig<string_len(TREI_FINDWORDEND_s);sig++){	\
			TREI_FINDWORDEND_pc=(char *)string_get(TREI_FINDWORDEND_s,sig);	\
			TREI_FINDWORDEND_node=trei_findchild(TREI_FINDWORDEND_node,TREI_FINDWORDEND_pc);	\
			if(TREI_FINDWORDEND_node==NULL)	\
				break;	\
			if(trei_iswordend(TREI_FINDWORDEND_node) && sig==string_len(TREI_FINDWORDEND_s)-1){	\
				findsig=TRUE;	\
				break;	\
			}	\
		}	\
		if(findsig==FALSE)	\
			TREI_FINDWORDEND_node=NULL;	\
	}	\
	TREI_FINDWORDEND_node;	\
	})


#define trei_havechild(_node) ptree_havechild((_node))
#define trei_havesibling(_node) ptree_havesibling((_node))






















#endif
