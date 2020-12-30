


#ifndef AST_AST_VISUALIZE_H_
#define AST_AST_VISUALIZE_H_

#include "ast_analyze.h"
#include "gdef.h"
#include "zlog.h"


/*=============================FUNC DEC==================================*/

void ast_preorder(ast_node_t *ast_head,log_t *log,depth_t depth);
string_t *ast_visualize_content(ast_node_t *ast_head,pool_t *pool,log_t *log);
static string_t *ast_visualize_macro(ast_node_t *ast_head,pool_t *pool,log_t *log);
static string_t *ast_visualize_statement(ast_node_t *ast_head,pool_t *pool,log_t *log);
static void ast_visualize_statement_preorder(ast_node_t *ast_node,string_t *s,log_t *log);
static string_t *ast_visualize_variable_declare(ast_node_t *ast_head,pool_t *pool,log_t *log);
static string_t *ast_visualize_func_declare(ast_node_t *ast_head,pool_t *pool,log_t *log);
static string_t *ast_visualize_funccall(ast_node_t *ast_head,pool_t *pool,log_t *log);
static string_t *ast_visualize_func_content_declare(ast_node_t *ast_head,pool_t *pool,log_t *log);
static string_t *ast_visualize_if(ast_node_t *ast_head,pool_t *pool,log_t *log);
static string_t *ast_visualize_while(ast_node_t *ast_head,pool_t *pool,log_t *log);
static string_t *ast_visualize_return(ast_node_t *ast_head,pool_t *pool,log_t *log);
#endif
