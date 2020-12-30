

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "ast_analyze.h"
#include "gdef.h"
#include "array.h"
#include "pstring.h"
#include "zlog.h"
#include "ast_base.h"
#include "ptree.h"
#include "array.h"
#include "pstring.h"
#include "ast_visualize.h"

void ast_preorder(ast_node_t *ast_head,log_t *log,depth_t depth){
	link_head *child=NULL,*pos=NULL;
	ast_node_t *n=NULL;
	ast_content_t *pcontent=NULL;
	char *pc=NULL;
	ast_indent(depth);
	depth+=1;
	pcontent=(ast_content_t *)(ast_head->elem);
	switch(pcontent->role){
	case program_node:
		printf("程序\n");
		break;
	case macro_node:
		printf("宏定义\n");
		break;
	case macro_content_node:
		printf("宏定义内容\n");
		break;
	case statement_node:
		printf("语句\n");
		break;
	case variable_declare_node:
		printf("变量声明\n");
		break;
	case id_node:
		printf("标识符\n");
		break;
	case func_declare_node:
		printf("函数声明\n");
		break;
	case func_content_declare_node:
		printf("函数定义\n");
		break;
	case funccall_node:
		printf("函数调用\n");
		break;
	case func_variable_node:
		printf("函数参数\n");
		break;
	case if_node:
		printf("if程序控制\n");
		break;
	case while_node:
		printf("while程序控制\n");
		break;
	case return_node:
		printf("return程序控制\n");
		break;
	case break_node:
		printf("break程序控制\n");
		break;
	case continue_node:
		printf("continue程序控制\n");
		break;
	case condition_node:
		printf("条件\n");
		break;
	case body_node:
		printf("主体\n");
		break;
	case ifbody_node:
		printf("if主体\n");
		break;
	case elsebody_node:
		printf("else主体\n");
		break;
	case block_node:
		printf("复合语句\n");
		break;
	case not_node:
		switch(pcontent->type){
		case identifier_e:
			printf("标识符:");
			break;
		case number:
			printf("数字:");
			break;
		case filename:
			printf("文件名:");
			break;
		case define_content_e:
			printf("替换内容:");
			break;
		case int_e:case char_e:case float_e:case double_e:
		case enum_e:case short_e:case long_e:case unsigned_e:
		case short_int:case long_int:case long_float:
		case unsigned_int:case unsigned_char:
			printf("类型名:");
			break;
		case less:case more:case lessequal:case moreequal:
		case equal:case plus:case minus:case plusplus:
		case minusminus:case plusequal:case minusequal:
		case division:case multiply:case divisionequal:
		case multiplyequal:case power:case remainder_e:
		case remainderequal:case equalequal:case not:case notequal:
		case and:case or:
			printf("运算符:");
			break;
		}
		pc=string_fetchs(pcontent->raw);
		printf("%s\n",pc);
		break;
	}
	child=&(ast_head->child);
	for(pos=child->next;pos!=child;pos=pos->next){
		n=(ast_node_t *)struct_entry(pos,ast_node_t,love);
		pcontent=(ast_content_t *)(n->elem);
		ast_preorder(n,log,depth);
	}
	return ;
}

/**
 * 根据ast树生成原始文件。基本思想:
 * 递归。。每往下深入一层理论上建立一个string_t 存储这个结点子子孙孙生成的源代码
 * 最小的单位是一个role,遇到role才生成一个string，对role的孩子们调用各自role对应的处理函数
 *
 * 有一个特殊情况，就是block,只有看到block才需要递归再次递归本函数
 * 对各自的处理函数返回的string_t,合并到此次的string_t,返回
 *
 */
string_t *ast_visualize_content(ast_node_t *ast_head,pool_t *pool,log_t *log){
	link_head *child=NULL,*pos=NULL;
	ast_node_t *n=NULL;
	ast_content_t *pcontent=NULL;
	char *pc=NULL;
	string_t *s=NULL;
	string_t *child_s=NULL,*child2_s=NULL;
	signal_t isroot=FALSE;
	signal_t isblock=FALSE;
	char semicolon_c=';';
	char left_bigbracket_c='{';
	char right_bigbracket_c='}';

	pcontent=(ast_content_t *)(ast_head->elem);
	switch(pcontent->role){
	case program_node:
		s=String(pool,"");	/*根结点生成根root string_t*/
		break;
	case macro_node:
		child_s=ast_visualize_macro(ast_head, pool, log);
		pc=string_fetchs(child_s);
		s=child_s;
		LOG_DEBUG(log,pc);
		goto AST_VISUALIZE_CONTENT_END;
	case statement_node:
		child_s=ast_visualize_statement(ast_head, pool, log);
		pc=string_fetchs(child_s);
		s=child_s;
		LOG_DEBUG(log,pc);
		goto AST_VISUALIZE_CONTENT_END;
	case variable_declare_node:
		child_s=ast_visualize_variable_declare(ast_head,pool,log);
		pc=string_fetchs(child_s);
		s=child_s;
		LOG_DEBUG(log,pc);
		goto AST_VISUALIZE_CONTENT_END;
	case func_declare_node:
		child_s=ast_visualize_func_declare(ast_head,pool,log);
		pc=string_fetchs(child_s);
		s=child_s;
		LOG_DEBUG(log,pc);
		goto AST_VISUALIZE_CONTENT_END;
	case func_content_declare_node:
		child_s=ast_visualize_func_content_declare(ast_head,pool,log);
		pc=string_fetchs(child_s);
		s=child_s;
		LOG_DEBUG(log,pc);
		goto AST_VISUALIZE_CONTENT_END;
	case funccall_node:
		child_s=ast_visualize_funccall(ast_head,pool,log);
		pc=string_fetchs(child_s);
		s=child_s;
		LOG_DEBUG(log,pc);
		goto AST_VISUALIZE_CONTENT_END;
	case if_node:
		child_s=ast_visualize_if(ast_head,pool,log);
		pc=string_fetchs(child_s);
		s=child_s;
		LOG_DEBUG(log,pc);
		goto AST_VISUALIZE_CONTENT_END;
	case while_node:
		child_s=ast_visualize_while(ast_head,pool,log);
		pc=string_fetchs(child_s);
		s=child_s;
		LOG_DEBUG(log,pc);
		goto AST_VISUALIZE_CONTENT_END;
		break;
	case return_node:
		child_s=ast_visualize_return(ast_head,pool,log);
		pc=string_fetchs(child_s);
		s=child_s;
		LOG_DEBUG(log,pc);
		goto AST_VISUALIZE_CONTENT_END;
		break;
		break;
	case break_node:
		child_s=String(pool,"");
		string_pushs(child_s,"break");
		s=child_s;
		LOG_DEBUG(log,"continue");
		goto AST_VISUALIZE_CONTENT_END;
	case continue_node:
		child_s=String(pool,"");
		string_pushs(child_s,"continue");
		s=child_s;
		LOG_DEBUG(log,"continue");
		goto AST_VISUALIZE_CONTENT_END;
	/*对block_node的处理涉及到递归调用，此处不用函数直接写在case下面*/
	case block_node:case body_node:
		s=String(pool,"");
		string_pushc(s,&left_bigbracket_c);
		isblock=TRUE;
		break;
	default:

		goto AST_VISUALIZE_CONTENT_END;
	}

	child=&(ast_head->child);
	for(pos=child->next;pos!=child;pos=pos->next){
		n=(ast_node_t *)struct_entry(pos,ast_node_t,love);
		child_s=ast_visualize_content(n,pool,log);
		pc=string_fetchs(child_s);
		string_pushs(s,pc);
	}
	if(isblock)
		string_pushc(s,&right_bigbracket_c);
	return s;

	AST_VISUALIZE_CONTENT_END:;
	string_pushc(s,&semicolon_c);

	return s;
}

/**
 * 描述一个macro node的结构
 * 此head结点  左孩子是一个identifier,可以是include或者是define
 * 右孩子是一个macro_content_node标示性的结点，孩子
 *
 */
static string_t *ast_visualize_macro(ast_node_t *ast_head,pool_t *pool,log_t *log){
	link_head *child=NULL,*pos=NULL;
	ast_node_t *n=NULL;
	ast_content_t *pcontent=NULL;
	char *pc=NULL,*pc2=NULL;
	string_t *s=NULL;
	string_t *child_s=NULL;
	signal_t errsig=FALSE;
	size_t childcount=0;
	char space_c=' ';
	char less_c='<';
	char more_c='>';
	pcontent=(ast_content_t *)(ast_head->elem);
	assert(pcontent->role==macro_node);

	s=String(pool,"#");	/*macro生成源代码内容向s去添加*/
	/*必须有两个孩子*/
	child=&(ast_head->child);

	/*child=child->next得到identifier树结点的linknode,把content加进去*/
	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);
	pcontent=(ast_content_t *)(n->elem);
	if(pcontent->role!=identifier_e)
		LOG_ERROR(log,"visualize macro_node but first child is not identifier");
	pc=string_fetchs(pcontent->raw);
	string_pushs(s,pc);
	string_pushc(s,&space_c);

	child=child->next;	/*此时child指向macro_content_node,是一个标识结点，没有内容，内容在孩子身上*/
	childcount=ptree_countchild(child);
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);
	child=&(n->child);	/*至此child得到了内容*/
	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);
	pcontent=(ast_content_t *)(n->elem);
	/**
	 * 判断删一个identifier pc是include还是define
	 * 如果是include,此时child拿到的是文件名,
	 * 如果是define,child拿到identifier,(下一个孩子可能有，如果有，就是define的内容.叫做替换内容)
	 *
	 */
	if(strcmp(pc,"include")==0){
		if(pcontent->type!=filename){
			errsig=TRUE;
			LOG_ERROR(log,"macro type include but content is not filename");
		}
		else{
			pc2=string_fetchs(pcontent->raw);
			string_pushc(s,&less_c);
			string_pushs(s,pc2);
			string_pushc(s,&more_c);
		}
	}else if(strcmp(pc,"define")==0){
		if(pcontent->type!=identifier_e){
			errsig=TRUE;
			LOG_ERROR(log,"macro type define but cant fine define identifier");
		}
		pc2=string_fetchs(pcontent->raw);
		string_pushs(s,pc2);
		if(childcount>1){
			string_pushc(s,&space_c);
			child=child->next;
			n=(ast_node_t *)struct_entry(child,ast_node_t,love);
			pcontent=(ast_content_t *)(n->elem);
			if(pcontent->type!=define_content_e){
				errsig=TRUE;
				LOG_ERROR(log,"handle define ,have second child but not define_content_e");
			}
			pc2=string_fetchs(pcontent->raw);
			string_pushs(s,pc2);
		}
	}
	else{
		errsig=TRUE;
		LOG_ERROR(log,"cant  handle this kind of macro");
	}

	return s;
}

static string_t *ast_visualize_statement(ast_node_t *ast_head,pool_t *pool,log_t *log){
	link_head *child=NULL,*pos=NULL;
	ast_node_t *n=NULL;
	ast_content_t *pcontent=NULL;
	char *pc=NULL,*pc2=NULL;
	string_t *s=NULL,*child_s=NULL;
	signal_t errsig=FALSE;
	array_t *stack=NULL;
	size_t childcount=0;
	pcontent=(ast_content_t *)(ast_head->elem);
	assert(pcontent->role==statement_node);

	s=String(pool,"");
	child=&(ast_head->child);
	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);	/*此处n指向根运算符结点*/
	ast_visualize_statement_preorder(n,s,log);	/*先序遍历，将statement源代码数据存到s里面*/

	return s;
}

static void ast_visualize_statement_preorder(ast_node_t *ast_node,string_t *s,log_t *log){
	char *pc=NULL;
	ast_content_t *pcontent=NULL;
	link_head *child=NULL,*pos=NULL;
	ast_node_t *n=NULL;
	signal_t count=-1;

	child=&(ast_node->child);

	if(list_isone(child)){
		pcontent=(ast_content_t *)(ast_node->elem);
		pc=string_fetchs(pcontent->raw);
		string_pushs(s,pc);
		return ;
	}

	for(count=0,pos=child->next;pos!=child;count++,pos=pos->next){

		if(count==1){	/*访问child指向的中间结点*/
			pcontent=(ast_content_t *)(ast_node->elem);
			pc=string_fetchs(pcontent->raw);
			string_pushs(s,pc);
		}

		n=(ast_node_t *)struct_entry(pos,ast_node_t,love);
		ast_visualize_statement_preorder(n,s,log);
	}
	return ;
}

/*ast_head 为变量声明标识结点，没有内容*/
static string_t *ast_visualize_variable_declare(ast_node_t *ast_head,pool_t *pool,log_t *log){
	link_head *child=NULL,*child2=NULL,*pos=NULL;
	ast_node_t *n=NULL;
	ast_content_t *pcontent=NULL;
	char *pc=NULL,*pc2=NULL;
	string_t *s=NULL,*child_s=NULL;
	signal_t errsig=FALSE;
	array_t *stack=NULL;
	signal_t count=-1;
	char space_c=' ';
	char comma_c=',';
	char equal_c='=';
	pcontent=(ast_content_t *)(ast_head->elem);
	assert(pcontent->role==variable_declare_node);
	s=String(pool,"");

	/*获取第一个孩子  类型名*/
	child=&(ast_head->child);
	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);

	pcontent=(ast_content_t *)(n->elem);
	pc=string_fetchs(pcontent->raw);
	string_pushs(s,pc);
	string_pushc(s,&space_c);

	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);
	pcontent=(ast_content_t *)(n->elem);
	assert(pcontent->role==id_node);
	child=&(n->child);

	/*获取以后的孩子们代表的变量名以及对变量的赋值*/
	for(count=0,pos=child->next;pos!=child;count++,pos=pos->next){
		if(count>=1)
			string_pushc(s,&comma_c);
		n=(ast_node_t *)struct_entry(pos,ast_node_t,love);	/*n指向变量名字的标识符结点*/
		pcontent=(ast_content_t *)(n->elem);
		pc=string_fetchs(pcontent->raw);
		string_pushs(s,pc);
		/*如果有孩子，那孩子是对变量赋的初值*/
		if(ptree_havechild(n)){
			string_pushc(s,&equal_c);
			child2=&(n->child);
			child2=child2->next;
			n=(ast_node_t *)struct_entry(child2,ast_node_t,love);	/*n指向变量名字的标识符结点*/
			pcontent=(ast_content_t *)(n->elem);
			pc=string_fetchs(pcontent->raw);
			string_pushs(s,pc);
		}

	}
	return s;

}

static string_t *ast_visualize_func_declare(ast_node_t *ast_head,pool_t *pool,log_t *log){
	LOG_DEBUG(log,"visualizing func declare");
	link_head *child=NULL,*child2=NULL,*child3=NULL,*pos=NULL;
	ast_node_t *n=NULL;
	ast_content_t *pcontent=NULL;
	char *pc=NULL,*pc2=NULL;
	string_t *s=NULL,*child_s=NULL;
	signal_t errsig=FALSE;
	array_t *stack=NULL;
	signal_t count=-1;
	char space_c=' ';
	char comma_c=',';
	char left_bracket_c='(';
	char right_bracket_c=')';
	char equal_c='=';
	pcontent=(ast_content_t *)(ast_head->elem);
	s=String(pool,"");

	/*获取第一个孩子存的类型*/
	child=&(ast_head->child);
	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);
	pcontent=(ast_content_t *)(n->elem);
	pc=string_fetchs(pcontent->raw);
	string_pushs(s,pc);

	/*获取第二个孩子存的孩子标识符*/
	string_pushc(s,&space_c);
	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);
	child2=&(n->child);
	child2=child2->next;
	n=(ast_node_t *)struct_entry(child2,ast_node_t,love);
	pcontent=(ast_content_t *)(n->elem);
	pc=string_fetchs(pcontent->raw);
	string_pushs(s,pc);

	/*获取第三个孩子存的变量*/
	string_pushc(s,&left_bracket_c);
	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);
	child2=&(n->child);
	if(child2!=NULL){
		for(count=0,pos=child2->next;pos!=child2;count++,pos=pos->next){
			if(count>=1)
				string_pushc(s,&comma_c);
			n=(ast_node_t *)struct_entry(pos,ast_node_t,love);
			child3=&(n->child);
			child3=child3->next;
			n=(ast_node_t *)struct_entry(child3,ast_node_t,love);	/*此处n获取类型*/
			pcontent=(ast_content_t *)(n->elem);
			pc=string_fetchs(pcontent->raw);
			string_pushs(s,pc);

			string_pushc(s,&space_c);

			child3=child3->next;
			n=(ast_node_t *)struct_entry(child3,ast_node_t,love);	/*此处n获取变量名 identifier*/
			pcontent=(ast_content_t *)(n->elem);
			pc=string_fetchs(pcontent->raw);
			string_pushs(s,pc);

		}
	}
	string_pushc(s,&right_bracket_c);

	return s;
}

static string_t *ast_visualize_func_content_declare(ast_node_t *ast_head,pool_t *pool,log_t *log){
	link_head *child=NULL,*child2=NULL,*child3=NULL,*pos=NULL;
	ast_node_t *n=NULL;
	ast_content_t *pcontent=NULL;
	char *pc=NULL,*pc2=NULL;
	string_t *s=NULL,*s1=NULL;
	signal_t errsig=FALSE;
	array_t *stack=NULL;
	signal_t count=-1;
	char space_c=' ';
	char comma_c=',';
	char left_bracket_c='(';
	char right_bracket_c=')';
	char equal_c='=';
	pcontent=(ast_content_t *)(ast_head->elem);

	s=ast_visualize_func_declare(ast_head, pool, log);	/*调用func_declare获取big bracket之前的所有数据*/
	child=&(ast_head->child);
	child=child->next;
	child=child->next;
	child=child->next;
	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);
	child2=&(n->child);
	child=child2->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);
	pcontent=(ast_content_t *)(n->elem);
	s1=ast_visualize_content(n, pool, log);
	string_concat(s, s1);
	return s;
}

static string_t *ast_visualize_funccall(ast_node_t *ast_head,pool_t *pool,log_t *log){
	link_head *child=NULL,*child2=NULL,*child3=NULL,*pos=NULL;
	ast_node_t *n=NULL;
	ast_content_t *pcontent=NULL;
	char *pc=NULL,*pc2=NULL;
	string_t *s=NULL,*child_s=NULL;
	signal_t errsig=FALSE;
	array_t *stack=NULL;
	signal_t count=-1;
	char space_c=' ';
	char comma_c=',';
	char left_bracket_c='(';
	char right_bracket_c=')';
	char equal_c='=';
	pcontent=(ast_content_t *)(ast_head->elem);
	assert(pcontent->role==funccall_node);
	s=String(pool,"");

	/*获取函数名字(是一个标识符)*/
	child=&(ast_head->child);
	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);	/*n指向标识符所在结点*/
	pcontent=(ast_content_t *)(n->elem);
	pc=string_fetchs(pcontent->raw);
	string_pushs(s,pc);

	string_pushc(s,&left_bracket_c);
	child=child->next;
	if(child!=&(ast_head->child)){
		/*满足condition说明有函数参数传入*/
		n=(ast_node_t *)struct_entry(child,ast_node_t,love);
		child2=&(n->child);
		for(count=0,pos=child2->next;pos!=child2;count++,pos=pos->next){
			if(count>=1)
				string_pushc(s,&comma_c);
			n=(ast_node_t *)struct_entry(pos,ast_node_t,love);
			pcontent=(ast_content_t *)(n->elem);
			pc=string_fetchs(pcontent->raw);
			string_pushs(s,pc);
		}
	}
	string_pushc(s,&right_bracket_c);
	return s;
}

static string_t *ast_visualize_if(ast_node_t *ast_head,pool_t *pool,log_t *log){
	link_head *child=NULL,*child2=NULL,*child3=NULL,*pos=NULL;
	ast_node_t *n=NULL;
	ast_content_t *pcontent=NULL;
	char *pc=NULL,*pc2=NULL;
	string_t *s=NULL,*child_s=NULL;
	signal_t errsig=FALSE;
	array_t *stack=NULL;
	signal_t count=-1;
	char left_bracket_c='(';
	char right_bracket_c=')';

	pcontent=(ast_content_t *)(ast_head->elem);
	assert(pcontent->role==if_node);
	s=String(pool,"");
	string_pushs(s,"if");

	/*获取条件*/
	string_pushc(s,&left_bracket_c);
	child=&(ast_head->child);
	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);	/*n指向标识符所在结点*/
	child2=&(n->child);
	child2=child2->next;
	n=(ast_node_t *)struct_entry(child2,ast_node_t,love);	/*此处n指向一个 语句标识结点,本质是if条件*/
	child_s=ast_visualize_statement(n,pool,log);
	pc=string_fetchs(child_s);
	string_pushs(s,pc);
	string_pushc(s,&right_bracket_c);

	/*获取if条件成立执行的内容，判断是statement还是block*/
	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);
	child2=&(n->child);
	child2=child2->next;
	n=(ast_node_t *)struct_entry(child2,ast_node_t,love);
	child_s=ast_visualize_content(n,pool,log);
	pc=string_fetchs(child_s);
	string_pushs(s,pc);

	/*判断有没有else,如果有就获取*/
	child=child->next;
	if(child!=&(ast_head->child)){
		/*有else*/
		string_pushs(s,"else");
		n=(ast_node_t *)struct_entry(child,ast_node_t,love);
		child2=&(n->child);
		child2=child2->next;
		n=(ast_node_t *)struct_entry(child2,ast_node_t,love);
		child_s=ast_visualize_content(n,pool,log);
		pc=string_fetchs(child_s);
		string_pushs(s,pc);
	}
	return s;
}


static string_t *ast_visualize_while(ast_node_t *ast_head,pool_t *pool,log_t *log){
	link_head *child=NULL,*child2=NULL,*child3=NULL,*pos=NULL;
	ast_node_t *n=NULL;
	ast_content_t *pcontent=NULL;
	char *pc=NULL,*pc2=NULL;
	string_t *s=NULL,*child_s=NULL;
	signal_t errsig=FALSE;
	array_t *stack=NULL;
	signal_t count=-1;
	char left_bracket_c='(';
	char right_bracket_c=')';

	pcontent=(ast_content_t *)(ast_head->elem);
	assert(pcontent->role==while_node);
	s=String(pool,"");
	string_pushs(s,"while");

	/*获取条件*/
	string_pushc(s,&left_bracket_c);
	child=&(ast_head->child);
	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);	/*n指向标识符所在结点*/
	child2=&(n->child);
	child2=child2->next;
	n=(ast_node_t *)struct_entry(child2,ast_node_t,love);	/*此处n指向一个 语句标识结点,本质是if条件*/
	child_s=ast_visualize_statement(n,pool,log);
	pc=string_fetchs(child_s);
	string_pushs(s,pc);
	string_pushc(s,&right_bracket_c);

	/*获取while条件成立执行的内容，判断是statement还是block*/
	child=child->next;
	n=(ast_node_t *)struct_entry(child,ast_node_t,love);
	child2=&(n->child);
	child2=child2->next;
	n=(ast_node_t *)struct_entry(child2,ast_node_t,love);
	child_s=ast_visualize_content(n,pool,log);
	pc=string_fetchs(child_s);
	string_pushs(s,pc);

	return s;
}

static string_t *ast_visualize_return(ast_node_t *ast_head,pool_t *pool,log_t *log){
	link_head *child=NULL,*child2=NULL,*child3=NULL,*pos=NULL;
	ast_node_t *n=NULL;
	ast_content_t *pcontent=NULL;
	char *pc=NULL,*pc2=NULL;
	string_t *s=NULL,*child_s=NULL;
	signal_t errsig=FALSE;
	array_t *stack=NULL;
	signal_t count=-1;
	char space_c=' ';

	pcontent=(ast_content_t *)(ast_head->elem);
	assert(pcontent->role==return_node);
	s=String(pool,"");
	string_pushs(s,"return");

	child=&(ast_head->child);
	if(child!=child->next){
		string_pushc(s,&space_c);
		child=child->next;
		n=(ast_node_t *)struct_entry(child,ast_node_t,love);
		child_s=ast_visualize_statement(n, pool, log);
		pc=string_fetchs(child_s);
		string_pushs(s,pc);
	}
	return s;
}













