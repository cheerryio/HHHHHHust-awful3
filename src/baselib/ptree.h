


/**
 * 建立基于link_head的树,目标结构设计越简单越好
 *
 * 设计思路:
 * 每个母亲和所有孩子通过list_head形成一个环形链表，
 * 每生成一个孩子加入到树里面，可以通过母亲的深度+1得到自己的深度
 *
 * 目标：
 * 1. 通过宏传入
 */



#ifndef BASELIB_PTREE_H_
#define BASELIB_PTREE_H_

#include <assert.h>

#include "linklist.h"
#include "mempool.h"
#include "zlog.h"
#include "gdef.h"
#include "pstring.h"

typedef uint32_t depth_t;

#define ROOT_DEPTH 1


/*初始化根*/
#define ptree_root_init(_pool,_log,_type) ({	\
	if((_pool)==NULL || (_log)==NULL)	\
		return ABNORMAL;	\
	pool_t *PTREE_ROOT_INIT_pool=(_pool);	\
	ptree_root_t *PTREE_ROOT_INIT_r=NULL;	\
	PTREE_ROOT_INIT_r=(ptree_root_t *)palloc(PTREE_ROOT_INIT_pool,sizeof(ptree_root_t));	\
	if(PTREE_ROOT_INIT_r==NULL)	\
		return ABNORMAL;	\
		\
	PTREE_ROOT_INIT_r->elem_size=sizeof(_type);	\
	PTREE_ROOT_INIT_r->root=PTREE_ROOT_INIT_r;	\
	PTREE_ROOT_INIT_r->depth=ONE;	\
	PTREE_ROOT_INIT_r->pool=PTREE_ROOT_INIT_pool;	\
	PTREE_ROOT_INIT_r->log=(_log);	\
	init_listhead(&(PTREE_ROOT_INIT_r->child));	\
	init_listhead(&(PTREE_ROOT_INIT_r->love));	\
	PTREE_ROOT_INIT_r;	\
	})

/*初始化树的node*/
#define ptree_node_init(_pool) ({	\
	ptree_node_t *PTREE_NODE_INIT_node=NULL;	\
	PTREE_NODE_INIT_node=(ptree_node_t *)palloc((_pool),sizeof(ptree_node_t));	\
	if(PTREE_NODE_INIT_node==NULL)	\
		return ABNORMAL;	\
	PTREE_NODE_INIT_node->elem=NULL;	\
	PTREE_NODE_INIT_node->info=String((_pool),"");	\
	PTREE_NODE_INIT_node->info2=string2_init(_pool);	\
	PTREE_NODE_INIT_node->depth=-1;	\
	PTREE_NODE_INIT_node->data=NULL;	\
	\
	init_listhead(&(PTREE_NODE_INIT_node->child));	\
	init_listhead(&(PTREE_NODE_INIT_node->love));	\
	PTREE_NODE_INIT_node;	\
	})

/*为树的node的elem赋值指针 */
#define ptree_assign_elem(_node,_elem)	\
	if((_node)==NULL || (_elem)==NULL)	\
		return ABNORMAL;	\
	(_node)->elem=(_elem);

/*赋值node或者root的string型的info*/
#define ptree_assign_info(node,info)	\
	if((node)==NULL)	\
		return ABNORMAL;	\
	string_assign((node)->info,(info));

/*修改elem结构中的值需要知道结构是什么*/
#define ptree_modify_elem(node,_k,_v)

/*addchild 传入结点指针，不是link_node指针*/
#define ptree_addchild(node_parent,node_child)	\
	list_add(&((node_parent)->child),&((node_child)->love));	\
	(node_child)->root=(node_parent)->root;	\
	(node_child)->depth=(node_parent)->depth+1;

/**
 * 传入link_head，即parent的child 和 child的love进行addchild
 * 要用struct_entry得到结构首地址，为其他各个属性(depth,root)赋值,
 * parent可能为根节点，注意区分
 */
#define ptree_ex_addchild_linknode(parent_linknode,child_linknode) ({	\
	if((parent_linknode==NULL) || (child_linknode==NULL))	\
		return ABNORMAL;	\
	link_head *PTREE_EX_ADDCHILD_LINKNODE_child=(parent_linknode);	\
	link_head *PTREE_EX_ADDCHILD_LINKNODE_love=(child_linknode);	\
	size_t PTREE_EX_ADDCHILD_LINKNODE_root_depthrel=(void *)&(((ptree_root_t *)0)->child)-(void *)&(((ptree_root_t *)0)->depth);	\
	ptree_root_t *PTREE_EX_ADDCHILD_LINKNODE_root=NULL;	\
	ptree_node_t *PTREE_EX_ADDCHILD_LINKNODE_node=NULL,*PTREE_EX_ADDCHILD_LINKNODE_childnode=NULL;	\
	void *PTREE_EX_ADDCHILD_LINKNODE_pdepth=(void *)PTREE_EX_ADDCHILD_LINKNODE_child-PTREE_EX_ADDCHILD_LINKNODE_root_depthrel;	\
	PTREE_EX_ADDCHILD_LINKNODE_childnode=(ptree_node_t *)struct_entry(PTREE_EX_ADDCHILD_LINKNODE_love,ptree_node_t,love);	\
	if(*((size_t *)PTREE_EX_ADDCHILD_LINKNODE_pdepth)==ROOT_DEPTH){	\
		PTREE_EX_ADDCHILD_LINKNODE_root=(ptree_root_t *)struct_entry(PTREE_EX_ADDCHILD_LINKNODE_child,ptree_root_t,child);	\
		PTREE_EX_ADDCHILD_LINKNODE_childnode->depth=PTREE_EX_ADDCHILD_LINKNODE_root->depth+1;	\
		PTREE_EX_ADDCHILD_LINKNODE_childnode->root=PTREE_EX_ADDCHILD_LINKNODE_root;	\
	}	\
	else{	\
		PTREE_EX_ADDCHILD_LINKNODE_node=(ptree_node_t *)struct_entry(PTREE_EX_ADDCHILD_LINKNODE_child,ptree_node_t,child);	\
		PTREE_EX_ADDCHILD_LINKNODE_childnode->depth=PTREE_EX_ADDCHILD_LINKNODE_node->depth+1;	\
		PTREE_EX_ADDCHILD_LINKNODE_childnode->root=PTREE_EX_ADDCHILD_LINKNODE_node->root;	\
	}	\
	list_add(PTREE_EX_ADDCHILD_LINKNODE_child,PTREE_EX_ADDCHILD_LINKNODE_love);	\
	})

/*自定义compare函数，传入A,B参数，A==B返回0，A>B返回1，否则-1,进行由小到大排序*/
#define ptree_ex_addchild_preorder(node_parent,node_child,compare)

/*由大到小排序*/
#define ptree_ex_addchild_postorder(node_parent,node_child,compare)

/*对root和node都要执行的del操作*/
#define ptree_delbase(node)	\
	list_del(&((node)->love));	\
	list_del(&((node)->child));	\
	string_free((node)->info);
/*删除给定结点,给出结点结构起始位置指针,不是根*/
#define ptree_delnode(node)	\
	ptree_delbase(node);	\
	pfree((node)->root->pool,(node)->elem,(node)->root->elem_size);	\
	pfree((node)->root->pool,node,sizeof(ptree_node_t));
/*删除根结点*/
#define ptree_delroot(root)	\
	ptree_delbase(root);	\
	pfree((root)->pool,root,sizeof(ptree_root_t));


/**返回母亲结点的首地址,在此宏内部调用struct_entry
 * 此node结点已经被加在了树上，love链表上至少有两个结点
 * 获得结构里面depth是根据struct特定的元素顺序，用linknode-sizeof(depth_t)..
 * node->depth<1,判定node还没有被加到树上，end
 */
#define ptree_findparent(node)	({	\
	signal_t endsig=FALSE;	\
	link_head *PTREE_FINDPARENT_pos=NULL;	\
	ptree_node_t *PTREE_FINDPARENT_node=NULL;	\
	void *PTREE_FINDPARENT_pr=NULL;	\
	void *PTREE_FINDPARENT_pdepth=NULL;	\
	depth_t PTREE_FINDPARENT_depth=-1;	\
	size_t PTREE_FINDPARENT_rel=-1;	\
	\
	if((node)==NULL || (node)->love.next==NULL)	\
		endsig=TRUE;	\
	if((node)->depth<ONE)	\
		endsig=TRUE;	\
	if((node)->depth==ONE)	\
		endsig=TRUE;	\
	if((node)->depth==TWO){	\
		PTREE_FINDPARENT_pr=node->root;	\
		endsig=TRUE;	\
	}	\
	\
	if(endsig==FALSE){	\
		PTREE_FINDPARENT_node=(node);	\
		PTREE_FINDPARENT_rel=(void *)&(PTREE_FINDPARENT_node->love)-(void *)&(PTREE_FINDPARENT_node->depth);	\
		PTREE_FINDPARENT_depth=PTREE_FINDPARENT_node->depth;	\
		for(PTREE_FINDPARENT_pos=PTREE_FINDPARENT_node->love.next;PTREE_FINDPARENT_pos!=&(PTREE_FINDPARENT_node->love);PTREE_FINDPARENT_pos=PTREE_FINDPARENT_pos->next){	\
			PTREE_FINDPARENT_pdepth=(void *)PTREE_FINDPARENT_pos-PTREE_FINDPARENT_rel;	\
			if(memcmp(PTREE_FINDPARENT_pdepth,&PTREE_FINDPARENT_depth,sizeof(depth_t))!=0)	\
				break;	\
			}	\
			PTREE_FINDPARENT_pr=(void *)struct_entry(PTREE_FINDPARENT_pos,ptree_node_t,child);	\
	}	\
	PTREE_FINDPARENT_pr;	\
	})

/*传入树的node或root结点，判断有无孩子*/
#define ptree_nochild(node)	({	\
	signal_t is_nochild=TRUE;	\
	signal_t endsig=FALSE;	\
	if(node->depth<1)	\
		endsig=TRUE;	\
	if(endsig==FALSE)	\
		if(!list_isone((node)->child))	\
			is_nochild=FALSE;	\
	\
	is_nochild;	\
	})

/*判断此结点有无孩子*/
#define ptree_havechild(_node) ({	\
	if((_node)==NULL)	\
		return ABNORMAL;	\
	signal_t havechild=FALSE;	\
	link_head *PTREE_HAVECHILD_child=&((_node)->child);	\
	if(list_isaboveone(PTREE_HAVECHILD_child))	\
		havechild=TRUE;	\
	havechild;	\
	})
/*判断此结点有无兄弟*/
#define ptree_havesibling(_node) ({	\
	if((_node)==NULL)	\
		return ABNORMAL;	\
	signal_t havesibling=FALSE;	\
	link_head *PTREE_HAVESIBLING_love=&((_node)->love);	\
	if(list_isabovetwo(PTREE_HAVESIBLING_love))	\
		havesibling=TRUE;	\
	havesibling;	\
	})

/*判断node是否在root里面*/
#define ptree_intree(_root,_node)	({	\
	signal_t isintree=FALSE;	\
	if((_node)->root==_root)	\
		isintree=TRUE;	\
	isintree;	\
	})

/*计算有几个孩子*/
#define ptree_countchild(_linknode)	({	\
	size_t PTREE_COUNTCHILD_len=list_len(_linknode);	\
	if(PTREE_COUNTCHILD_len==ZERO){}	\
	else	\
		PTREE_COUNTCHILD_len--;	\
	\
	PTREE_COUNTCHILD_len;	\
	})



/*不知道parent是node类型还是root类型，无法得到这个结构的起始指针*/
#define ptree_addchild_linknode(linknode_parent,linknode_child)


typedef struct ptree_node_s ptree_node_t;
typedef struct ptree_root_s ptree_root_t;


/**
 * 树的根结点
 */
struct ptree_root_s {
	string_t *info;
	ptree_root_t *root;
	depth_t depth;
	link_head love;
	link_head child;
	size_t elem_size;
	pool_t *pool;
	log_t *log;
};
/**
 * 树的结点,一个结点有且仅仅能被一个结点指向，一个结点可以指向多个结点
 */
struct ptree_node_s {
	string_t *info;
	string2_t *info2;
	ptree_root_t *root;
	depth_t depth;
	link_head love;
	link_head child;
	void *elem;	/*自定义的struct其实地址，初始化结点的时候需要pool分配空间*/
	void *data;	/*赋值任意结构指针*/
};


#endif
