


/**
 * ast_preprocessing 主要完成对初始.c源文件的预处理，预处理后生成.i文件
 * 再开始抽象语法树模块的解读
 *
 * 关于对源代码文件的预处理
 * 实现目标：
 * 1. 删去注释
 * 2. #define 替换
 * 3. 判断#ifdef #ifndef 后面的代码是否需要写入.i文件进行编译
 * 4. 将引用文件包括进来
 *
 * 实现思路：
 * 从文件中逐个的读入字符，边读入边定义状态，当然我做的都是最简单的
 * 预处理都是处理#开头的字符,需要替换的都是识别为一个单词,#define的本质是取替换一个identifier
 *
 * 1. 正常状态保存字符
 * 2. 注释状态不保存字符
 * 3. 当读到#define定义的东西时，生成define结构去存储（define需支持变量）
 *    并建立对应名字的索引，需要支持检索,define需要支持 \ 换行
 * 4. 读到ifdef ifndef要确定接下来的内容是否需要去编译
 * 5. 生成trie树进行单词检索，如果此次检索失败，需要回溯
 */


#ifndef AST_AST_PREPROCESS_H_
#define AST_AST_PREPROCESS_H_

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "array.h"
#include "string.h"
#include "linklist.h"
#include "trei.h"


typedef struct ast_preprocess_s ast_preprocess_t;
typedef struct ast_define_data_s define_data_t;
typedef struct ast_include_val_s ast_include_val_t;
typedef struct ast_source_s ast_source_t;

struct ast_preprocess_s {
	FILE *ast_main_c;	/*c源代码文件指针*/
	FILE *ast_main_i;	/*预处理后的输出文件,毕竟main函数的文件要直接给词法分析器*/

	string2_t *filenams;	/*需要预编译文件的文件名，无后缀*/
	link_head ast_source_head;
	trei_root_t *trei_define_root;	/*define 值的trei树*/
	pool_t *pool;
	log_t *log;
};

struct ast_source_s {				/*在出错时需要遍历此链表释放文件*/
	FILE *ast_source_c;
	FILE *ast_source_i;
	link_head ast_source_node;
};

/**
 * 对 #define A B 的语句，记录把pattern 和 replace 的值
 */
struct ast_define_data_s {
	string_t *identifier;
	string2_t *variable;
	string_t *content;
	link_head define_data_linknode;
};

struct ast_include_val_s {
	FILE *include;
	link_head *link_node;
};

/*初始化，打开文件，返回总结点*/
ast_preprocess_t *ast_preprocess_init(pool_t *pool,log_t *log);
void ast_preprocess_scanner(ast_preprocess_t *ast_preprocess);

static void ast_preprocess_getdefine(FILE *fp,trei_root_t *define_root);
static string_t *ast_getdefine_getcontent(FILE *fp,trei_root_t *define_root,pool_t *pool,log_t *log);	/*给getdefine函数调用*/
define_data_t *ast_preprocess_finddefine(trei_root_t *define_root,char *identifier);





#endif
