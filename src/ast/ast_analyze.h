

/**
 * 预处理之后的格式：
 * 										第一行			第一次出现在.i文件
 * 遇到#include <hfilename.h> 此行替换为 # 1 "hfilename.h" 1
 * 下接.h文件不做替换的原本内容
 *
 * # #include<hfilename.h>所在行号 "csrc.c" 第几次出现
 *
 * 词法分析还是从上往下分析，分析到错误报行号，列号。该文件中#开头表示行号
 * 不会有注释，还是要看字符串
 */






#ifndef AST_BASE_H_
#define AST_BASE_H_


#include <stdint.h>
#include <stdio.h>


#include "zlog.h"
#include "linklist.h"
#include "array.h"
#include "mempool.h"
#include "pstring.h"
#include "ast_preprocess.h"
#include "ast_base.h"

typedef ptree_root_t ast_root_t;
typedef ptree_node_t ast_node_t;

typedef struct ast_analyze_s ast_analyze_t;

struct ast_analyze_s {
	FILE *ast_file_i;
	ast_root_t *root;
	ast_locationinfo_t *location;
	pool_t *pool;
	zlog_category_t *log;
};



#define ast_root_init(_pool,_log,_type)	ptree_root_init((_pool),(_log),(_type))

#define ast_node_init(_pool,_content) ({	\
	ptree_node_t *node=NULL;	\
	void *elem=NULL;	\
	node=ptree_node_init((_pool));	\
	elem=palloc((_pool),sizeof(*(_content)));	\
	memcpy(elem,(_content),sizeof(*(_content)));	\
	node->elem=elem;	\
	node;	\
})

#define ast_addchild(node_parent,node_child)	\
	ptree_addchild(node_parent,node_child)















/*----------------------函数声明------------------------*/
ast_analyze_t *ast_analyze_init(pool_t *pool,log_t *log);
static ptree_node_t *ast_analyze_macros(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
static ptree_node_t **ast_analyze_variable_statement(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
static ast_node_t *ast_declare(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
static ast_node_t *ast_declare_func(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
static ast_node_t *ast_funccall(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
static ast_node_t *ast_control(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
static ast_node_t *ast_comment(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
static ast_node_t *ast_macro(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
ast_node_t *ast_statement(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
ast_node_t *ast_scanner(ast_node_t *head_n,FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
#endif




















