
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "ast_analyze.h"
#include "gdef.h"
#include "array.h"
#include "pstring.h"
#include "zlog.h"
#include "ast_base.h"
#include "ptree.h"
#include "array.h"
#include "ast_visualize.h"

ast_analyze_t *ast_analyze_init(pool_t *pool,log_t *log){
	if(pool==NULL || log==NULL)
		return NULL;
	ast_analyze_t *ast_analyze=NULL;
	ptree_root_t *root=NULL;
	ast_locationinfo_t *location=NULL;
	FILE *fp=NULL;

	ast_analyze=(ast_analyze_t *)palloc(pool,sizeof(ast_analyze_t));
	if(ast_analyze==NULL)
		return NULL;
	root=ptree_root_init(pool,log,ast_content_t);
	location=(ast_locationinfo_t *)palloc(pool,sizeof(ast_locationinfo_t));
	if(location==NULL)
		return NULL;
	location->file=String(pool,"");
	location->line=0;
	location->count=0;
	location->block_depth=0;
	fp=ast_fopen(AST_MAIN_C,"r",log);
	if(fp==NULL)
		return NULL;
	ast_analyze->ast_file_i=fp;
	ast_analyze->root=root;
	ast_analyze->location=location;
	ast_analyze->pool=pool;
	ast_analyze->log=log;

	return ast_analyze;
}

void ast_analyze_main(ast_analyze_t *ast_analyze){
	if(ast_analyze==NULL)
		return ;
	char c;
	ast_content_t base_content;
	pool_t *pool=ast_analyze->pool;
	log_t *log=ast_analyze->log;
	ast_locationinfo_t *location=ast_analyze->location;
	FILE *fp=ast_analyze->ast_file_i;
	ast_root_t *ast_root=ast_analyze->root;
	ast_node_t *ast_head=NULL,*n=NULL;
	string_t *s=NULL,*ps=NULL;
	char *pc=NULL,*pc2=NULL;
	sequence_t seq=-1;
	link_head *child=NULL,*pos=NULL;
	ast_content_t *pcontent;

	base_content.role=program_node;
	ast_head=ast_node_init(pool,&base_content);
	ast_addchild(ast_root,ast_head);	/*ast_head是root唯一的孩子，ast_head看作是抽象语法树的根节点*/
	while((n=ast_scanner(ast_head,fp, location, pool, log))!=NULL);

	ast_preorder(ast_head,log,0);

	s=ast_visualize_content(ast_head,pool,log);
	pc=string_fetchs(s);
	printf("final:%s\n",pc);

	return ;
}

/**
 * 函数会传入head_n根结点，调用一次能够处理一种情况，比如能够处理一个注释
 * 能够处理一次函数定义，能够处理一段 if else
 * 返回值是这一次加入到head_n里面的孩子结点
 */
ast_node_t *ast_scanner(ast_node_t *head_n,FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log){
	ast_content_t content,base_content;
	ast_node_t *n=NULL,*parent_n=NULL;
	string_t *s=NULL,*prev_s=NULL;;
	char *pc=NULL,*prev_pc=NULL;
	ast_content_t *pcontent=NULL;
	signal_t errsig=FALSE;

	/*scanner专注于生成语法树结点，location记录放在ast_getcontent中*/
	AST_SCANNER_START:;
	content=ast_getcontent(fp,location,pool,log);
	switch(content.type){
	case none:
		return NULL;
	case semicolon:	/*分号跳过*/
		base_content.role=semicolon_blank_node;
		n=ast_node_init(pool,&base_content);
		LOG_WARN(log,"meet a statement but only one semicolon ;");
		return n;
	case space:	/*空白跳过，不加入树*/
		goto AST_SCANNER_START;
	case linecomment:case left_multilinecomment:	/*注释*/
		pc=string_fetchs(content.raw);
		ast_ungets(fp,pc,location);
		n=ast_comment(fp, location, pool, log);
		break;
	case well:
		pc=string_fetchs(content.raw);
		ast_ungets(fp,pc,location);
		n=ast_macro(fp, location, pool, log);
		break;
	/*函数声明或者是变量声明或者是函数内容body*/
	case int_e:case char_e:case float_e:case double_e:case enum_e:
	case short_e:case long_e:case unsigned_e:case short_int:
	case long_int:case long_float:case unsigned_int:case unsigned_char:
		pc=string_fetchs(content.raw);
		ast_ungets(fp,pc,location);
		n=ast_declare(fp, location, pool, log);
		break;
	/*流程控制语句*/
	case if_e:case while_e:case for_e:case return_e:case break_e:
	case continue_e:
		s=content.raw;
		pc=string_fetchs(s);
		ast_ungets(fp,pc,location);
		n=ast_control(fp, location, pool, log);
		break;
	/*标识符开头，可能赋值，可能函数，可能是表达式运算,二值表达式或者+-表达式(识别出来分别调用函数即可）*/
	case identifier_e:
		s=content.raw;
		prev_pc=string_fetchs(s);
		content=ast_getcontent(fp,location,pool,log);
		pc=string_fetchs(content.raw);
		switch(content.type){
		case left_bracket:
			/*调用函数参数解析，返回参数根节点*/
			ast_ungets(fp,pc,location);
			ast_ungets(fp,prev_pc,location);
			n=ast_funccall(fp, location, pool, log);
			break;
		default:	/*不是函数调用，认为是表达式 = + -*/
			ast_ungets(fp,pc,location);
			ast_ungets(fp,prev_pc,location);
			n=ast_statement(fp, location, pool, log);
			break;
		}
		break;
	case left_bigbracket:	/*处理符合语句,方法是递归调用此ast_scanner函数*/
		base_content.role=block_node;
		parent_n=ast_node_init(pool,&base_content);
		while((content=ast_getcontent(fp, location, pool, log)).type!=right_bigbracket){
			if(content.type==none)
				break;
			pc=string_fetchs(content.raw);
			ast_ungets(fp,pc,location);
			n=ast_scanner(parent_n,fp, location, pool, log);
			if(n==NULL)
				break;
		}
		if(content.type==none){
			LOG_ERROR(log,"analyzing block,but cant find } to close this block");
			errsig=TRUE;
		}
		n=parent_n;
		break;
	case right_bigbracket:
		LOG_ERROR(log,"unexpected } found");
		errsig=TRUE;
		break;
	}
	AST_SCANNER_END:;
	if(n==NULL){
		LOG_ERROR(log,"error when ast scanner,make child null,not going to add this child");
	}
	else{
		ast_addchild(head_n,n);
	}
	return n;
}

/**
 * 主要处理 #include 和 #define
 *
 *
 * 										头结点role表示macro_node
 * 										/					\
 * 									   /                      \
 *                                    /                         \
 *                               identifier含内容            macro_content_node
 *                    										/			\
 *                    									   /			  \
 *                                                        /                 \
 *                                                       /                   \
 *                                                  标识符名字			      此标识符对应的内容
 *
 *
 *
 */
static ast_node_t *ast_macro(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log){
	ast_content_t content,base_content;
	ast_node_t *head_n=NULL,*n=NULL,*macro_content_n=NULL;
	signal_t errsig=-1;
	sequence_t seq=-1;
	string_t *filename_s=NULL,*define_content=NULL;
	char c;
	char *pc=NULL;
	enum Macro_type {
		include=0,define,ifdef,ifundef,undef,else_macro
	} macro_type;
	enum Define_state {
		content_define,splash_define,crossline_define
	} define_state;
	char *macros[]={"include","define","ifdef","ifundef","undef","else"};
	string2_t *macros_s2=String2(pool,macros,sizeof(macros)/sizeof(macros[0]));

	base_content.role=macro_node;
	head_n=ast_node_init(pool,&base_content);
	content=ast_getcontent(fp, location, pool, log);
	if(content.type!=well){
		errsig=TRUE;
		LOG_ERROR(log,"in function macro but cant # at start");
		goto AST_MACRO_END;
	}
	content=ast_getcontent(fp, location, pool, log);
	pc=string_fetchs(content.raw);
	seq=string2_finds(macros_s2,pc);
	if(seq<0){
		errsig=TRUE;
		LOG_ERROR(log,"macro type dont in several types");
		goto AST_MACRO_END;
	}
	n=ast_node_init(pool,&content);
	ast_addchild(head_n,n);
	base_content.role=macro_content_node;
	macro_content_n=ast_node_init(pool,&base_content);
	ast_addchild(head_n,macro_content_n);
	content=ast_getcontent(fp, location, pool, log);	/*获得了include或者define那些之后！！的第一个东西*/
	macro_type=seq;
	switch(macro_type){
	case include:
		switch(content.type){
		case less:
			filename_s=String(pool,"");
			while((c=ast_getc(fp))!='>'){
				if(c==EOF){
					errsig=TRUE;
					LOG_ERROR(log,"get macro include filename but cant fine >");
					goto AST_MACRO_END;
				}
				string_pushc(filename_s,&c);
			}
			content.role=not_node;
			content.type=filename;
			content.raw=filename_s;
			break;
		case doublequotation:
			filename_s=String(pool,"");
			while((c=ast_getc(fp))!='\"'){
				if(c==EOF){
					errsig=TRUE;
					LOG_ERROR(log,"get macro include filename but cant fine >");
					goto AST_MACRO_END;
				}
				string_pushc(filename_s,&c);
			}
			content.role=not_node;
			content.type=filename;
			content.raw=filename_s;
			break;
		default:
			errsig=TRUE;
			LOG_ERROR(log,"macro include but cant find < or \"");
			goto AST_MACRO_END;
		}

		n=ast_node_init(pool,&content);
		ast_addchild(macro_content_n,n);
		goto AST_MACRO_END;
	case define:
		if(content.type!=identifier_e){
			errsig=TRUE;
			LOG_ERROR(log,"macro define but cant find identifier after define");
			goto AST_MACRO_END;
		}
		n=ast_node_init(pool,&content);
		ast_addchild(macro_content_n,n);
		/**
		 * 需要读取define identifier 内容 的内容， 判断有无内容调用内容读取程序
		 * 结束的标志是这一行读到了\n但是没有 '\\'
		 */
		define_state=content_define;
		define_content=String(pool,"");
		while((c=ast_getc(fp))!=EOF){
			switch(c){
			case '\\':
				switch(define_state){
				case content_define:
					define_state=splash_define;
					break;
				default:
					errsig=TRUE;
					LOG_ERROR(log,"get macro define content but unexpected \\");
					goto AST_MACRO_END;
				}
				break;
			case '\n':
				switch(define_state){
				case content_define:	/*获取define内容结束*/
					goto AST_MACRO_DEFINE_END;
				case splash_define:
					define_state=content_define;
					break;
				}
				break;
			default:
				if(define_state==content_define)
					string_pushc(define_content,&c);
				else{
					errsig=TRUE;
					LOG_ERROR(log,"unexpected %c when get define content",c);
					goto AST_MACRO_END;
				}
				break;
			}
		}
		AST_MACRO_DEFINE_END:;
		content.role=not_node;
		content.type=define_content_e;
		content.raw=define_content;
		n=ast_node_init(pool,&content);
		ast_addchild(macro_content_n,n);
		break;
	case ifdef:case ifundef:case undef:
		if(content.type!=identifier_e){
			errsig=TRUE;
			LOG_ERROR(log,"macro define but cant find identifier after define");
			goto AST_MACRO_END;
		}
		n=ast_node_init(pool,&content);
		ast_addchild(macro_content_n,n);
		break;
	case else_macro:	/*无内容不读取,返回content读过的内容*/
		pc=string_fetchs(content.raw);
		ast_ungets(fp,pc,location);
		break;
	}

	AST_MACRO_END:;
	if(errsig==TRUE)
		head_n=NULL;
	return head_n;
}


/* 已知下一个字符是 '/'
 * 正常none,读入 '/' 状态possible,
 *   再入 '/' line_comment,读到\n结束
 *   再入 '*' multiline_comment,读到 '*' possible_end,再入 '/' 回到none
 *
 * 主要有 / *字符需要处理
 */
static ast_node_t *ast_comment(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log){
	char c,a;
	string_t *s=NULL;
	signal_t errsig=FALSE;
	ast_node_t *comment_n=NULL,*n=NULL;
	enum Comment_state {
		none,possible_start,line_comment,multiline_comment,possible_end	\
	} comment_state;
	comment_state=none;
	ast_content_t content,base_content;
	content.role=not_node;
	s=String(pool,"");

	base_content.role=comment_node;
	comment_n=ast_node_init(pool,&base_content);
	content=ast_getcontent(fp,location,pool,log);
	switch(content.type){
	case linecomment:
		while((c=ast_getc(fp))!=EOF && c!='\n')	/*EOF也能作为单行注释的结束*/
			string_pushc(s,&c);
		string_popc(s);	/*取出\n或者EOF*/
		break;
	case left_multilinecomment:
		while((c=ast_getc(fp))!=EOF){
			switch(c){
			case EOF:
				errsig=TRUE;
				LOG_ERROR(log,"multuline comment without right end");
				goto AST_GETCOMMENTS_END;
			case '*':
				a=ast_getc(fp);
				if(a=='/'){
					errsig=FALSE;
					ast_ungetc(fp,&c);
					goto AST_GETCOMMENTS_END;
				}
				break;
			default:
				string_pushc(s,&c);
				break;
			}
		}
		break;
	default:
		LOG_ERROR(log,"in ast_comment function ,but first content is not left_multilinecomment or linecomment");
		errsig=TRUE;
		break;
	}
	AST_GETCOMMENTS_END:;
	if(errsig==TRUE)
		comment_n=NULL;
	else{
		content.raw=s;
		n=ast_node_init(pool,&content);
		ast_addchild(comment_n,n);
	}
	return comment_n;
}


/**
 * 此函数中head_n是最终返回的结点
 */
static ast_node_t *ast_control(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log){
	ast_content_t content,base_content;
	signal_t errsig=FALSE;
	ast_node_t *n=NULL,*head_n=NULL,*statement_n=NULL,*body_n=NULL;
	char *pc=NULL;
	content.role=not_node;

	content=ast_getcontent(fp, location, pool, log);
	switch(content.type){
	case if_e:
		base_content.role=if_node;
		head_n=ast_node_init(pool,&base_content);
		content=ast_getcontent(fp, location, pool, log);
		if(content.type!=left_bracket){
			LOG_ERROR(log,"handle if but dont meet  (");
			errsig=TRUE;
			break;
		}
		/*获取if的condition*/
		statement_n=ast_statement(fp, location, pool, log);
		if(statement_n==NULL){
			LOG_ERROR(log,"handle if but condition statement is not ok");
			errsig=TRUE;
			break;
		}
		base_content.role=condition_node;
		n=ast_node_init(pool,&base_content);
		ast_addchild(n,statement_n);
		ast_addchild(head_n,n);
		/*判断if的condition结束以后是否有右括号*/
		content=ast_getcontent(fp, location, pool, log);
		if(content.type!=right_bracket){
			LOG_ERROR(log,"handle if but dont meet  ) in if condition");
			errsig=TRUE;
			break;
		}
		base_content.role=ifbody_node;
		body_n=ast_node_init(pool,&base_content);
		ast_addchild(head_n,body_n);
		n=ast_scanner(body_n,fp,location,pool,log);	/*statement或者复合语句结点作为孩子*/
		if(n==NULL){
			LOG_ERROR(log,"handle if but dont find if body");
			errsig=TRUE;
			break;
		}
		/*继续判断是否有else*/
		content=ast_getcontent(fp, location, pool, log);
		if(content.type==else_e){
			base_content.role=elsebody_node;
			body_n=ast_node_init(pool,&base_content);
			ast_addchild(head_n,body_n);
			n=ast_scanner(body_n,fp,location,pool,log);	/*statement或者复合语句结点作为孩子*/
			if(n==NULL){
				LOG_ERROR(log,"handle if but cant find if body");
				errsig=TRUE;
				break;
			}
		}else if(content.type==none){
			NULL;
		}else{
			pc=string_fetchs(content.raw);
			ast_ungets(fp,pc,location);
		}
		break;
	/*while跟if的区别就是while + condition后面能直接; ,if 必须要body*/
	case while_e:
		base_content.role=while_node;
		head_n=ast_node_init(pool,&base_content);
		content=ast_getcontent(fp, location, pool, log);
		if(content.type!=left_bracket){
			LOG_ERROR(log,"handle while but dont meet  (");
			errsig=TRUE;
			break;
		}
		/*获取while的condition*/
		statement_n=ast_statement(fp, location, pool, log);
		if(statement_n==NULL){
			LOG_ERROR(log,"handle if but condition statement is not ok");
			errsig=TRUE;
			break;
		}
		base_content.role=condition_node;
		n=ast_node_init(pool,&base_content);
		ast_addchild(n,statement_n);
		ast_addchild(head_n,n);
		/*判断condition结束是否有右括号*/
		content=ast_getcontent(fp, location, pool, log);
		if(content.type!=right_bracket){
			LOG_ERROR(log,"handle if condition but dont meet )");
			errsig=TRUE;
			break;
		}
		content=ast_getcontent(fp, location, pool, log);
		switch(content.type){
		case semicolon:
			goto AST_CONTROL_END;
		default:
			pc=string_fetchs(content.raw);
			ast_ungets(fp,pc,location);
			base_content.role=body_node;
			body_n=ast_node_init(pool,&base_content);
			ast_addchild(head_n,body_n);
			n=ast_scanner(body_n,fp,location,pool,log);	/*statement或者复合语句结点作为孩子*/
			if(n==NULL){
				LOG_ERROR(log,"handle if but dont find if body");
				errsig=TRUE;
				goto AST_CONTROL_END;
			}
			break;
		}
		break;
	case return_e:
		base_content.role=return_node;
		head_n=ast_node_init(pool,&base_content);
		content=ast_getcontent(fp, location, pool, log);
		switch(content.type){
		case semicolon:
			goto AST_CONTROL_END;
		default:
			pc=string_fetchs(content.raw);
			ast_ungets(fp,pc,location);
			n=ast_statement(fp,location,pool,log);	/*statement或者复合语句结点作为孩子*/
			if(n==NULL){
				LOG_ERROR(log,"handle if but dont find if body");
				errsig=TRUE;
				goto AST_CONTROL_END;
			}
			ast_addchild(head_n,n);
			break;
		}
		break;
	case break_e:
		base_content.role=break_node;
		head_n=ast_node_init(pool,&base_content);
		content=ast_getcontent(fp, location, pool, log);
		if(content.type!=semicolon){
			LOG_ERROR(log,"control sentense break but cant find ; after");
			errsig=TRUE;
			goto AST_CONTROL_END;
		}
		break;
	case continue_e:
		base_content.role=continue_node;
		head_n=ast_node_init(pool,&base_content);
		content=ast_getcontent(fp, location, pool, log);
		if(content.type!=semicolon){
			LOG_ERROR(log,"control sentense break but cant find ; after");
			errsig=TRUE;
			goto AST_CONTROL_END;
		}
		break;
	default:
		errsig=TRUE;
		LOG_ERROR(log,"in control sentense,but cant find any control keyword");
		break;
	}
	AST_CONTROL_END:;
	if(errsig==TRUE)
		head_n=NULL;
	return head_n;
}



/**
 * 已知一行 ；前为statement语句，有number或标识符，进行 > < = + - 运算
 *
 *
 *
 * 								=
 * 						       / \
 *                            /    \
 *                           a      b
 *
 * 需要先生成前序表达式，传入fp，从fp读入所有与本次表达式有关的字符入栈
 * 出栈，判断优先级，生成前序表达式
 *
 * 如何判断是本次表达式所有有关的字符?
 * 1. 记录左括号数量，当读到右括号-左括号=1，作结束判断，注意，0 即两者数目相等都不行
 * 2. 读到分号判断结束
 * 3. 读到大括号判断结束
 *
 * 对a+4*6+b这个式子，    b输入，+进操作数栈，6输入，*比+优先，进操作数栈，4输入，
 * 						+输入，则*输出，+输出，a输出，+输出
 *						结果：b 6 4 * + a +
 *						逆转：+ a + * 4 6 b
 */
ast_node_t *ast_statement(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log){
	ast_content_t *pre_pcontent=NULL,*pcontent=NULL,content,base_content;
	ast_content_t *pcontent_l=NULL,*pcontent_r=NULL;
	array_t *operator_stack=NULL,*member_stack=NULL,*preorder_stack=NULL;
	array_t *head_stack=NULL;
	ast_node_t *l_n=NULL,*head_n=NULL,*r_n=NULL,*prev_head_n=NULL,*statement_n=NULL;
	ast_node_t **phead_n=NULL;
	char *pc=NULL;
	signal_t ranksig=-1;
	signal_t errsig=FALSE;
	signal_t bracket_count=0;
	void *p=NULL,*p1=NULL;
	link_head *child=NULL,*pos=NULL;
	signal_t sig=0;
	content.role=not_node;
	base_content.role=statement_node;
	statement_n=ast_node_init(pool,&base_content);
	operator_stack=init_array(pool,sizeof(ast_content_t));
	member_stack=init_array(pool,sizeof(ast_content_t));
	preorder_stack=init_array(pool,sizeof(ast_content_t));
	enum Statement_state {
		start_statement,bracket_statement,operator_statement,member_statement
	} state;

	while((content=ast_getcontent(fp, location, pool, log)).type!=none){
		content.role=not_node;
		switch(content.type){
		case left_bracket:
			bracket_count++;
			array_push(member_stack,&content);
			break;
		case right_bracket:
			bracket_count--;
			if(bracket_count<0){
				pc=string_fetchs(content.raw);
				ast_ungets(fp,pc,location);	/*多读的右括号还回去,只是代表读取表达式环节结束，当读取的表达式正确继续处理*/
				goto AST_STATEMENT_PUSH_MEMBER_END;
			}
			array_push(member_stack,&content);
			break;
		case semicolon:
			if(bracket_count>0){
				LOG_ERROR(log,"meet selicolon ';' ,but left bracket and right bracket no match");
				errsig=TRUE;
			}
			goto AST_STATEMENT_PUSH_MEMBER_END;
			break;
		case number:case identifier_e:
		case plusplus:case minusminus:case not:case multiply:
		case division:case remainder_e:case plus:case minus:
		case more:case less:case moreequal:case lessequal:
		case equalequal:case notequal:case equal:case divisionequal:
		case multiplyequal:	case plusequal:case minusequal:case remainderequal:
			array_push(member_stack,&content);
			pc=string_fetchs(content.raw);
			break;
		default:
			if(bracket_count>0){
				LOG_ERROR(log,"meet unexpected letter but bracket left and right don't match");
				errsig=TRUE;
				goto AST_STATEMENT_END;
			}else{
				pc=string_fetchs(content.raw);
				ast_ungets(fp,pc,location);	/*多读的右括号还回去*/
				goto AST_STATEMENT_PUSH_MEMBER_END;
			}
			break;
		}
	}
	AST_STATEMENT_PUSH_MEMBER_END:;
	/*member_stack把所有该表达式的符号都收集到，现生成前序表达式，出栈*/

	AST_STATEMENT_PREOEDER_START:;
	while((pcontent=(ast_content_t *)array_pop(member_stack))!=NULL){
		switch(pcontent->type){
		case number:case identifier_e:	/*运算成员入结果栈*/
			array_push(preorder_stack,pcontent);
			break;
		case right_bracket:	/*右括号直接入运算符栈*/
			array_push(operator_stack,pcontent);
			break;
		/*此运算符比前一符号有限，入运算符栈，否则前一个运算符出栈，知道遇到优先的，或者遇到左括号*/
		case plusplus:case minusminus:case not:case multiply:
		case division:case remainder_e:case plus:case minus:
		case more:case less:case moreequal:case lessequal:
		case equalequal:case notequal:	case equal:case divisionequal:
		case multiplyequal:	case plusequal:case minusequal:case remainderequal:
			/*当 非空 && 前一个元素不是括号 && 栈顶元素优先级大于此元素*/
			while(!array_empty(operator_stack) && (pre_pcontent=(ast_content_t *)array_getlast(operator_stack))->type!=left_bracket && pre_pcontent->type!=right_bracket && ast_operator_compare(*pcontent,*pre_pcontent)<=0){
				pre_pcontent=(ast_content_t *)array_pop(operator_stack);	/*拿出上一个运算符*/
				array_push(preorder_stack,pre_pcontent);	/*输出*/
			}
			array_push(operator_stack,pcontent);
			break;
		case left_bracket:
			/*operator_stack里面运算符全部输出直到遇到右括号,全面已经做过检测，左右是配对的*/
			while((pre_pcontent=(ast_content_t *)array_pop(operator_stack))->type!=right_bracket)
				array_push(preorder_stack,pre_pcontent);
			break;
		default:
			break;
		}
	}
	while(!array_empty(operator_stack)){
		pre_pcontent=(ast_content_t *)array_pop(operator_stack);	/*拿出上一个运算符*/
		array_push(preorder_stack,pre_pcontent);	/*输出*/
	}
	/**
	 * 生成树,应该用递归，  遇到运算符，后面一个一定是成员，判断下一个是运算符就再调用此函数
	 * 如果再下一个不是运算符是成员，生成右边孩子就好
	 *
	 * 还有一个特殊情况，只有一个标识符或者数字
	 */
	if(preorder_stack->elem_sum==1){
		pcontent=(ast_content_t *)array_pop(preorder_stack);
		if(pcontent->type!=identifier_e && pcontent->type!=number){
			LOG_ERROR(log,"if condition with only one charactor but not number or identifier");
			errsig=TRUE;
			goto AST_STATEMENT_END;
		}
		pcontent->role=not_node;
		head_n=ast_node_init(pool,pcontent);
	}else{
		head_stack=init_array(pool,sizeof(ast_node_t *));
		while(!array_empty(preorder_stack)){
			pcontent=(ast_content_t *)array_pop(preorder_stack);	/*运算符*/
			pcontent_l=(ast_content_t *)array_pop(preorder_stack);	/*identifier或者number*/
			pcontent_r=(ast_content_t *)array_pop(preorder_stack);	/*判断标识符还是identifier 或 number*/
			head_n=ast_node_init(pool,pcontent);
			l_n=ast_node_init(pool,pcontent_l);
			ast_addchild(head_n,l_n);
			switch(pcontent_r->type){
			case number:case identifier_e:
				pcontent_r->role=not_node;
				r_n=ast_node_init(pool,pcontent_r);
				ast_addchild(head_n,r_n);
				array_push(head_stack,&head_n);
				break;
			case plusplus:case minusminus:case not:case multiply:
			case division:case remainder_e:case plus:case minus:
			case more:case less:case moreequal:case lessequal:
			case equalequal:case notequal:	case equal:case divisionequal:
			case multiplyequal:	case plusequal:case minusequal:case remainderequal:
				array_push(head_stack,&head_n);
				array_push(preorder_stack,pcontent_r);
				break;
			default:
				break;
			}
		}
		phead_n=(ast_node_t **)array_pop(head_stack);
		head_n=*phead_n;
		prev_head_n=head_n;
		while(!array_empty(head_stack)){
			phead_n=(ast_node_t **)array_pop(head_stack);
			head_n=*phead_n;
			ast_addchild(head_n,prev_head_n);
		}
	}
	AST_STATEMENT_END:;
	if(errsig==TRUE)
		return NULL;
	else
		ast_addchild(statement_n,head_n);
	return statement_n;

}

/**
 * 变量声明的树
 * 							变量声明
 * 							/	   \
 * 						   /        \
 * 						  /          \
 * 					   变量类型        变量
 * 					   		          /   \
 * 					                 /      \
 * 					               /          \
 * 					              标识符名字     \
 * 					                            变量
 * 					   							/	\
 * 					   						   /	  \
 * 					   						  名字       \
 * 					   						              变量
 *
 * 函数只知道第一个读到的是标识符，会随着时间的跟进判断这是一个什么状态
 * head_n最开先的role是变量声明，如果读完第一个标识符后读到;或者=或者,就可以
 * 断定这是一个变量声明的语句
 * 读完第一个标识符读到( 状态变为函数声明，读完所有形参，
 * 读完形参之后如果是;就断定是函数声明
 * 读完形参之后如果是{就断定是函数定义，调用scanner获取此块内容
 *
 *
 * id_n只是指路结点，没有有用内容。左孩子存identifier名字是id_content，右孩子还是id_n
 */
static ast_node_t *ast_declare(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log){
	string_t *s=NULL;
	char c;
	char *pc=NULL;
	signal_t errsig=FALSE;
	ast_content_t base_content,content,id_content;	/*变量声明可以有很多id，用id_content存储*/
	ast_content_t type;
	ast_content_t id;
	ast_node_t *head_n=NULL,*type_n=NULL,*id_n=NULL,*id_content_n=NULL,*n=NULL;
	ast_node_t *func_variable_n=NULL,*body_n=NULL;
	enum Declare_state {
		start_declare,id_declare,array_number_declare,equal_declare,assign_declare,constant_declare,comma_declare,
		func_variable,func_body
	} declare_state;
	enum Role_state {
		none_role,variable_declare_role,func_declare_role,func_content_declare_role
	} role_state;
	content.role=not_node;

	role_state=none_role;
	declare_state=start_declare;	/*先默认在声明变量*/
	base_content.role=variable_declare_node;
	head_n=ast_node_init(pool,&base_content);
	head_n->depth=1;
	content=ast_getcontent(fp,location,pool,log);
	if(!(content.type>=int_e && content.type<=unsigned_char)){
		LOG_ERROR(log,"dont find variable type when analyzing first word of declaration");
		errsig=TRUE;
		goto AST_DECLARE_END;
	}
	n=ast_node_init(pool,&content);
	ast_addchild(head_n,n);	/*加入变量类型*/

	/**
	 * 函数变量声明这个只标识不包含有用内容的结点都通用
	 * 如果是在声明变量有一个id_n,孩子们都是identifier，如果init赋值这些identifier的孩子是数据
	 */
	content.role=id_node;	/*可能需要加入许多声明变量的标识符，不用n临时，*/
	id_n=ast_node_init(pool,&content);
	id_content=ast_getcontent(fp, location, pool, log);	/*获取identifier name*/
	if(id_content.type!=identifier_e){
		LOG_ERROR(log,"dont find identifier when analyzing second word of declaration");
		errsig=TRUE;
		goto AST_DECLARE_END;
	}
	/*获得了第一个标识符（变量声明可能有多个），放在id_n结点的孩子。此结点不用赋值role*/
	declare_state=id_declare;
	id_content_n=ast_node_init(pool,&id_content);
	ast_addchild(id_n,id_content_n);
	ast_addchild(head_n,id_n);

	AST_DECLARE_START:;
	content=ast_getcontent(fp, location, pool, log);
	switch(content.type){
	case semicolon:CASE_AST_EOF	/*分号结束,此函数一定需要标志*/
		switch(declare_state){
		case start_declare:case id_declare:case constant_declare:
			role_state=variable_declare_role;
			goto AST_DECLARE_END;
		case func_variable:
			role_state=func_declare_role;
			goto AST_DECLARE_END;
		default:
			LOG_ERROR(log,"unexpected letter ; in declaration");
			errsig=TRUE;
			break;
		}
		break;
	case identifier_e:	/*遇到标识符*/
		switch(declare_state){
		case comma_declare:
			/*创建标识符存名字的结点，做id_n的孩子,等遇到comma再创建role结点*/
			id_content_n=ast_node_init(pool,&content);
			ast_addchild(id_n,id_content_n);
			declare_state=id_declare;
			goto AST_DECLARE_START;
			break;
		case assign_declare:	/*用标识符给标识符赋值*/
			n=ast_node_init(pool,&content);
			ast_addchild(id_content_n,n);
			declare_state=constant_declare;
			goto AST_DECLARE_START;
			break;
		default:
			LOG_ERROR(log,"unexpected identifier, not after equal or before comma in id declare");
			errsig=TRUE;
			goto AST_DECLARE_END;
		}
		break;
	case left_middlebracket:	/*识别到 [  ,确定是数组*/
		content=ast_getcontent(fp, location, pool, log);
		if(content.type!=number){	/*数组方括号中不是数字*/
			LOG_ERROR(log,"detected [,but no number found in id declare");
			errsig=TRUE;
			goto AST_DECLARE_END;
		}
		n=ast_node_init(pool,&content);	/*数组有多少个的数字，存在id_content下面*/
		ast_addchild(id_content_n,n);
		content=ast_getcontent(fp, location, pool, log);
		if(content.type!=right_middlebracket){
			LOG_ERROR(log,"detected [ number ,but no ] found in id declare");
			errsig=TRUE;
			goto AST_DECLARE_END;
		}
		goto AST_DECLARE_START;
		break;
	case equal:	/*遇到等号，状态需要是identifier_declare*/
		if(declare_state==id_declare || declare_state==start_declare){
			role_state=variable_declare_role;
			declare_state=assign_declare;
			goto AST_DECLARE_START;
		}else{
			LOG_ERROR(log,"unexpected  equal = not after identifier in id declare");
			errsig=TRUE;
			goto AST_DECLARE_END;
		}
	case number:	/*数字常量，只能处在=背后*/
		switch(declare_state){
			case assign_declare:
				n=ast_node_init(pool,&content);
				ast_addchild(id_content_n,n);
				declare_state=constant_declare;
				goto AST_DECLARE_START;
			default:
				LOG_ERROR(log,"unexpected number '%s', in id declare",string_fetchs(content.raw));
				errsig=TRUE;
				goto AST_DECLARE_END;
		}
		break;
	case singlequotation:case doublequotation:
		content=ast_getstring(fp,location,pool,log);
		if(content.type!=string){
			LOG_ERROR(log,"assign constant start with \",but not analyzed as string");
			errsig=TRUE;
			goto AST_DECLARE_END;
		}
		n=ast_node_init(pool,&content);
		ast_addchild(id_content_n,n);
		declare_state=constant_declare;
		goto AST_DECLARE_START;
	case comma:	/*确定了在定义变量，无需考虑是函数在定义变量,函数变量获取单独弄函数*/
		role_state=variable_declare_role;
		switch(declare_state){
		case id_declare:case constant_declare:	/*=常量，以及id后可以有逗号*/
			declare_state=comma_declare;
			goto AST_DECLARE_START;
		default:
			LOG_ERROR(log,"unexpected  comma ',' in id declare");
			errsig=TRUE;
			goto AST_DECLARE_END;
		}
		break;
	case left_bracket:
		if(declare_state==id_declare){	/*进入获得参数变量*/
			role_state=func_declare_role;
			func_variable_n=ast_declare_func(fp, location, pool, log);
			if(n==NULL){
				LOG_ERROR(log,"error get function variable");
				errsig=TRUE;
				goto AST_DECLARE_END;
			}
		}
		else{
			errsig=TRUE;
			LOG_ERROR(log,"meet left bracket in some declare but not after first identifier");
			goto AST_DECLARE_END;
		}
		declare_state=func_variable;
		goto AST_DECLARE_START;
		break;
	case left_bigbracket:
		if(declare_state!=func_variable){
			LOG_ERROR(log,"get func declare but find unexpected {");
			errsig=TRUE;
			goto AST_DECLARE_END;
		}
		role_state=func_content_declare_role;
		pc=string_fetchs(content.raw);
		ast_ungets(fp,pc,location);
		ast_addchild(head_n,func_variable_n);
		base_content.role=body_node;
		body_n=ast_node_init(pool,&base_content);
		n=ast_scanner(body_n, fp, location, pool, log);
		if(body_n==NULL){
			LOG_ERROR(log,"get func defination body error");
			errsig=TRUE;
			goto AST_DECLARE_END;
		}
		ast_addchild(head_n,body_n);
		break;
	default:
		if(content.raw!=NULL)
			LOG_ERROR(log,"meet unexpected charactor '%s' when handle declare",string_fetchs(content.raw));
		else
			LOG_ERROR(log,"meet unexpected charactor when handle declare");
		errsig=TRUE;
		break;
		}
	AST_DECLARE_END:;
	if(errsig==TRUE)
		head_n=NULL;
	else{
		if(role_state==none_role)
			errsig=TRUE;
		if(role_state==variable_declare_role){
			((ast_content_t *)(head_n->elem))->role=variable_declare_node;
		}
		if(role_state==func_declare_role){
			((ast_content_t *)(head_n->elem))->role=func_declare_node;
			ast_addchild(head_n,func_variable_n);
		}
		if(role_state==func_content_declare_role)
			((ast_content_t *)(head_n->elem))->role=func_content_declare_node;
	}
	return head_n;
}

/**
 * 函数形参的树
 *
 * 									参数-----------------参数-------类型
 * 								   /    \                  \
 *                                /      \                   \
 *                               /        \                    \
 *                              /           \                    名字
 *                            参数           参数
 * 				             /   \
 *                         /      \
 *                       类型       名字
 *
 *
 *
 */

static ast_node_t *ast_declare_func(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log){
	ast_node_t *head_n=NULL,*base_n=NULL,*n=NULL,*id_n=NULL;
	signal_t errsig=FALSE;
	signal_t findcomma=FALSE;
	ast_content_t base_content,content;
	base_content.role=func_variable_node;

	head_n=ast_node_init(pool,&base_content);
	AST_DECLARE_FUNC_START:;
	content=ast_getcontent(fp, location, pool, log);
	switch(content.type){
	case none:
		LOG_ERROR(log,"unexpected end file when analyzing function variable");
		errsig=TRUE;
		goto AST_DECLARE_FUNC_END;
	case comma:
		if(findcomma==TRUE){
			LOG_ERROR(log,"improper comma , found when analyzing function variable");
			errsig=TRUE;
			goto AST_DECLARE_FUNC_END;
		}
		goto AST_DECLARE_FUNC_START;
	case right_bracket:	/*读进去*/
		goto AST_DECLARE_FUNC_END;
	case int_e:case char_e:case float_e:case double_e:case enum_e:
	case short_e:case long_e:case unsigned_e:case short_int:
	case long_int:case long_float:case unsigned_int:case unsigned_char:
		findcomma=FALSE;
		base_n=ast_node_init(pool,&base_content);
		ast_addchild(head_n,base_n);
		n=ast_node_init(pool,&content);
		ast_addchild(base_n,n);	/*加入参数类型*/
		content=ast_getcontent(fp, location, pool, log);
		if(content.type!=identifier_e){
			LOG_ERROR(log,"assign constant start with \",but not analyzed as string");
			errsig=TRUE;
			goto AST_DECLARE_FUNC_END;
		}
		n=ast_node_init(pool,&content);
		ast_addchild(base_n,n);
		goto AST_DECLARE_FUNC_START;
	default:
		LOG_ERROR(log,"unexpected thing when analyzing function variable");
		errsig=TRUE;
		goto AST_DECLARE_FUNC_END;
	}
	AST_DECLARE_FUNC_END:;
	if(errsig==TRUE)
		head_n=NULL;
	return head_n;
}

static ast_node_t *ast_funccall(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log){
	ast_content_t base_content,content;
	string_t *s=NULL;
	char *pc=NULL;
	ast_node_t *n=NULL,*head_n=NULL,*variable_n=NULL;
	signal_t errsig=FALSE;
	enum Funccall_state {
		start_funccall,identifier_funccall,comma_funccall,right_bracket_funccall
	} state;
	content.role=not_node;

	/*head_n是表示在 调用函数的主结点*/
	base_content.role=funccall_node;
	head_n=ast_node_init(pool,&base_content);
	head_n->depth=1;
	/*获取函数标识符*/
	content=ast_getcontent(fp, location, pool, log);
	if(content.type!=identifier_e){
		LOG_ERROR(log,"in funccall function,but cant find identifier of function");
		errsig=TRUE;
		goto AST_FUNCCALL_END;
	}
	/*创建存储函数声明部分的标识符存储结点*/
	n=ast_node_init(pool,&content);
	ast_addchild(head_n,n);

	/*获取左括号*/
	content=ast_getcontent(fp, location, pool, log);
	if(content.type!=left_bracket){
		LOG_ERROR(log,"in funccall function,but cant find left bracket of variable");
		errsig=TRUE;
		goto AST_FUNCCALL_END;
	}
	base_content.role=func_variable_node;
	variable_n=ast_node_init(pool,&base_content);
	ast_addchild(head_n,variable_n);

	/*循环得到所有variable，加到variable_n的孩子*/
	state=start_funccall;
	AST_FUNCCALL_START:;
	content=ast_getcontent(fp, location, pool, log);
	switch(content.type){
	case identifier_e:
		switch(state){
		case start_funccall:case comma_funccall:
			n=ast_node_init(pool,&content);
			ast_addchild(variable_n,n);
			state=identifier_funccall;
			break;
		default:
			LOG_ERROR(log,"in funccall function,but found unexpected identifier");
			errsig=TRUE;
			goto AST_FUNCCALL_END;
		}
		goto AST_FUNCCALL_START;
	case comma:
		if(state!=identifier_funccall){
			LOG_ERROR(log,"in funccall function,but not found identifier after comma");
			errsig=TRUE;
			goto AST_FUNCCALL_END;
		}
		state=comma_funccall;
		goto AST_FUNCCALL_START;
	case right_bracket:
		switch(state){
		case start_funccall:case identifier_funccall:
			state=right_bracket_funccall;
			goto AST_FUNCCALL_START;
		default:
			LOG_ERROR(log,"in funccall function,but found unexpected right bracket");
			errsig=TRUE;
			goto AST_FUNCCALL_END;
		}
		break;
	case semicolon:
		if(state!=right_bracket_funccall){
			LOG_ERROR(log,"in funccall function,but found unexpected semicolon ;");
			errsig=TRUE;
		}
		goto AST_FUNCCALL_END;
	default:
		LOG_ERROR(log,"in funccall function,but found unexpected thing");
		errsig=TRUE;
		goto AST_FUNCCALL_END;
	}

	AST_FUNCCALL_END:;
	if(errsig==TRUE)
		head_n=NULL;
	return head_n;
}

































