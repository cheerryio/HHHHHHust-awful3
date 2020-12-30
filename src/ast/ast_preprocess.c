
/**
 * 所有函数传入的参数还是应该是preprocess总结点
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

#include "mempool.h"
#include "string.h"
#include "gdef.h"
#include "trei.h"
#include "ast_preprocess.h"
#include "ast_base.h"
#include "linklist.h"
#include "zlog.h"

/*初始化ast_preprocess_t结构,并将其接在ast上*/
ast_preprocess_t *ast_preprocess_init(pool_t *pool,log_t *log){
	if(pool==NULL || log==NULL)
		return NULL;
	ast_preprocess_t *ast_preprocess=NULL;
	FILE *fp=NULL;
	trei_root_t *trei_define_root=NULL;
	string2_t *filenames_s2=NULL;
	string_t *filename_s=NULL;
	char c_postfix[3]=".c";
	char i_postfix[3]=".i";
	char *char_filenames[]=AST_FILENAMES;	/*所有源文件名字，无后缀*/

	ast_preprocess=(ast_preprocess_t *)palloc(pool,sizeof(ast_preprocess_t));
	if(ast_preprocess==NULL)
		return NULL;
	filenames_s2=String2(pool,char_filenames,AST_FILENAMES_SIZE);
	ast_preprocess->pool=pool;
	ast_preprocess->log=log;
	ast_preprocess->trei_define_root=trei_root_init(pool,log);

	signal_t sig=-1;
	link_head *mainfile_node=NULL;
	ast_source_t *ast_source=NULL;
	char filename[20];
	char file[22];
	link_head *ast_source_head=&(ast_preprocess->ast_source_head);
	init_list_head(ast_source_head);
	for(sig=ZERO;sig<filenames_s2->elem_sum;sig++){
		ast_source=(ast_source_t *)palloc(pool,sizeof(ast_source_t));
		if(ast_source==NULL)
			return NULL;
		filename_s=string2_get(filenames_s2,sig);
		string_giveelem(filename_s,filename);

/*得到文件全名放到file里面*/
#define FULL_FILENAME(_postfix)	\
	memset(file,'\0',22);	\
	strcat(file,filename);	\
	strcat(file,_postfix);

		/*打开此名字下的 .i .c文件*/
		FULL_FILENAME(c_postfix);
		fp=ast_fopen(file,"r",log);
		ast_source->ast_source_c=fp;

		FULL_FILENAME(i_postfix);
		fp=ast_fopen(file,"w",log);
		ast_source->ast_source_i=fp;

		list_add(ast_source_head,&(ast_source->ast_source_node));
	}
	/*初始化 找main函数的文件指针*/
	mainfile_node=ast_source_head->prev;
	ast_source=(ast_source_t *)struct_entry(mainfile_node,ast_source_t,ast_source_node);
	ast_preprocess->ast_main_c=ast_source->ast_source_c;
	ast_preprocess->ast_main_i=ast_source->ast_source_i;

	return ast_preprocess;
}

/*
 * 动态初始化的主函数,预处理都是#开头
 * 读到#开头，进入宏处理函数
 * 读到_字母开头，进标识符替换函数（获取这个标识符，与define树里面的标识符比较，如果被define过进行替换）
 * 读到 '/' 识别是否注释,c有  /*   //两种注释,进入注释处理程序
 * 当未触发上述条件时，读入的字符都缓存到string_t中
 *
 * 有 include,define,ifdef,ifundef,endif,undef
 */
#define MACROS {"include","define","ifdef","ifundef","endif","undef"}
#define MACROS_SIZE 6
#define MACROS_ENUM enum macros_enum {	\
	none=-1,include=0,define,ifdef,ifundef,endif,undef	\
}
void ast_preprocess_scanner(ast_preprocess_t *ast_preprocess){
	if(ast_preprocess==NULL)
		return ;
	char c;
	char *pc=NULL;
	pool_t *pool=ast_preprocess->pool;
	log_t *log=ast_preprocess->log;
	trei_root_t *define_root=ast_preprocess->trei_define_root;
	char *macros[]=MACROS;
	string2_t *macros_s2=String2(pool,macros,MACROS_SIZE);
	link_head *ast_source_head=&(ast_preprocess->ast_source_head);
	link_head *pos=NULL;
	ast_source_t *ast_source=NULL;
	FILE *ast_source_c=NULL;
	FILE *ast_source_i=NULL;
	string_t *identifier_s=String(pool,""),*s=NULL;
	string_t *ifile_s=String(pool,"");
	char buffer[50];
	MACROS_ENUM macros_seq;
	/*循环不遵循先main后其他的文件顺序*/
	for(pos=ast_source_head->next;pos!=ast_source_head;pos=pos->next){
		ast_source=(ast_source_t *)struct_entry(pos,ast_source_t,ast_source_node);
		ast_source_c=ast_source->ast_source_c;
		ast_source_i=ast_source->ast_source_i;
		string_fresh(ifile_s);	/*清空上一次输出.i文件缓冲区*/
		while((c=ast_getc(ast_source_c))!=EOF){
			switch(c){
			/*进入宏处理程序，*/
			case '#':
				fscanf(ast_source_c,"%[^\t\v\f\n( ]",buffer);
				macros_seq=string2_finds(macros_s2,buffer);
				switch(macros_seq){
				case none:
					LOG_ERROR(log,"improper '#' when preprocess,terminated with -1");
					exit(-1);
					break;
				case include:

					break;
				case define:
					ast_preprocess_getdefine(ast_source_c,define_root);
					break;
				case ifdef:

					break;
				case ifundef:

					break;
				case endif:

					break;
				case undef:

					break;
				}
				break;
			/*先识别出下一个identifier，将此identifier与define的内容比较，有就替换*/
			CASE_AST_LETTER case '_':
				ast_ungetc(ast_source_c,&c);
				identifier_s=ast_getword(ast_source_c,identifier_s);	/*会把s清空再重新赋值*/
				break;
			case '\'':	/*跳过这个字符串*/
				c=ast_charactor(ast_source_c);
				if(c==-1 || c==EOF)
					LOG_ERROR(log,"unexpected define of charactor");
				c=ast_charactor(ast_source_c);
				if(c!='\'')
					LOG_ERROR(log,"unexpected end of charactor");
				break;
			default:
				string_pushc(ifile_s,&c);
				break;
			}
		}
	}

	return ;
}

/*传入文件指针，输出缓冲，进行include引入,注意*/
static void ast_preprocess_include(FILE *fp,string_t *ifile_s){
	if(fp==NULL || ifile_s==NULL)
		return ;
	char c;

}

/*define的键值加入到trei树,包括标识符，参数，内容*/
static void ast_preprocess_getdefine(FILE *fp,trei_root_t *define_root){
	if(define_root==NULL)
		return ;
	char *pc=NULL;
	char c;
	char define[7]="define";
	char buffer[50];
	sequence_t seq=-1;
	string_t *identifier_s=NULL,*content_s=NULL,*s=NULL;
	string2_t *variables_s2=NULL;
	char *identifier=NULL,*variable=NULL,*content=NULL;
	trei_node_t *endnode=NULL;
	link_head *define_data_node;
	define_data_t *define_data=NULL;
	signal_t findblank=FALSE;
	signal_t findvariable=FALSE;
	signal_t findcontent=FALSE;
	signal_t success_sig=FALSE;
	pool_t *pool=define_root->pool;
	log_t *log=define_root->log;
	identifier_s=String(pool,"");
	/*已经读完了 '#define' 接下来空格接标识符 ( | ' '  define内容*/

	/*处理 #define 与 标识符之间的空格*/
	while((c=ast_getc(fp))!=EOF){
		switch(c){
		CASE_AST_BLANK
			findblank=TRUE;
			break;
		default:
			ast_ungetc(fp,&c);
			goto AST_GETDEFINE_A_END;
		}
	}
	AST_GETDEFINE_A_END:;
	if(findblank==FALSE){
		LOG_ERROR(log,"expected blank after #define");
		goto AST_GETDEFINE_END;
	}

	/*处理#define的标识符名称*/
	identifier_s=ast_getword(fp,identifier_s);
	if(identifier_s==NULL){
		LOG_ERROR(log,"wrong identifier of define");
		goto AST_GETDEFINE_END;
	}

	/*判断identifier之后有无空格进行 获取参数程序 或者直接去获取define内容*/
	findblank=FALSE;
	c=ast_getc(fp);
	switch(c){
	CASE_AST_BLANK
		findblank=TRUE;
		findvariable=FALSE;
		break;
	default:
		ast_ungetc(fp,&c);
		break;
	}
	if(findblank==FALSE){
		variables_s2=ast_getvariable(fp,pool,log);
		if(variables_s2==NULL){
			LOG_ERROR(log,"error preprocess #define variable");
			goto AST_GETDEFINE_END;
		}
	}

	/**
	 * 开始获取define的内容，当读到 \n时 仍然没有内容，判定没有内容
	 * 内容自动删去 '\\' ,其他所有字符照抄 参数的)与内容之间应该有 blank
	 * 否则报错
	 */
	findblank=FALSE;
	findcontent=FALSE;
	while((c=ast_getc(fp))!=EOF){
		switch(c){
		case ' ':case '\t':case '\v':case '\f':
			findblank=TRUE;
			break;
		case '\n':
			ast_ungetc(fp,&c);
			goto AST_GETDEFINE_B_END;
			break;
		default:
			findcontent=TRUE;
			ast_ungetc(fp,&c);
			goto AST_GETDEFINE_B_END;
		}
	}
	AST_GETDEFINE_B_END:;
	if(findblank==FALSE && findcontent==TRUE){
		LOG_ERROR(log,"unexpected letter:%c ,supposed to be blank",c);
		goto AST_GETDEFINE_END;
	}
	/*有合法define内容可以获取*/
	if(findcontent==TRUE){
		content_s=ast_getdefine_getcontent(fp,define_root,pool,log);
		if(content_s==NULL)	/*有内容但格式错误获取失败*/
			goto AST_GETDEFINE_END;
	}
	/*
	 * 根据 identifier_s,variables_s2,content_s
	 * 并判断findthing==TRUE|FALSE为trei树赋值
	 */

	/*在trei树addword标识符*/
	identifier=string_fetchs(identifier_s);
	/*为数据结点进行赋值identifier,variable,content,NULL也直接赋值*/
	define_data=(define_data_t *)palloc(pool,sizeof(define_data_t));
	if(define_data==NULL)
		goto AST_GETDEFINE_END;
	define_data->identifier=identifier_s;
	define_data->variable=variables_s2;
	define_data->content=content;
	endnode=trei_addword(define_root,identifier);
	/*加入末尾数据结点*/
	list_add(&(endnode->child),&(define_data->define_data_linknode));
	define_data=ast_preprocess_finddefine(define_root,identifier);

	AST_GETDEFINE_END:;
	return ;
}

/*获取define的内容,应该遍历每一个标识符进行宏展开再存到string_t里边*/
static string_t *ast_getdefine_getcontent(FILE *fp,trei_root_t *define_root,pool_t *pool,log_t *log){
	if(fp==NULL || pool==NULL || log==NULL)
		return NULL;
	char c;
	char *pc=NULL;
	define_data_t *define_data=NULL;
	string_t *content_s=NULL;	/*函数需要返回的define*/
	string_t *s=NULL;
	string_t *identifier=NULL;
	string_t *content=NULL;
	string2_t *variable=NULL;
	enum Content_state {
		iscontent,crossline
	} content_state;

	s=String(pool,"");
	content_s=String(pool,"");
	/*循环后正常应该以content状态结束*/
	while((c=ast_getc(fp))!=EOF){
		switch(c){
		case '\\':
			if(content_state==iscontent)
				content_state=crossline;
			else{
				LOG_ERROR(log,"unexpected word:%c when preprocess get #define content crossing line",c);
				goto AST_GETDEFINE_CONTENT_END;
			}
			break;
		case '\n':
			if(content_state==iscontent)
				goto AST_GETDEFINE_CONTENT_END;
			else
				content_state==iscontent;
			break;
		CASE_AST_LETTER case '_':
			if(content_state==content){
				ast_ungetc(fp,&c);
				s=ast_getword(fp,s);
				pc=string_fetchs(s);
				define_data=ast_preprocess_finddefine(define_root,pc);
				if(define_data==NULL)	/*此关键字不在trei树，不展开，加入原值*/
					string_pushs(content_s,pc);
				else{	/*存进去的都是已经expand的宏*/
					variable=define_data->variable;
					content=define_data->content;
				/*进行参数替换*/
				}
			}
			else{
				LOG_ERROR(log,"unexpected letter:%c",c);
				goto AST_GETDEFINE_CONTENT_END;
			}
			break;
		default:
			string_pushc(content_s,&c);
			break;
		}
	}
	AST_GETDEFINE_CONTENT_END:;
	if(content_state==crossline){
		string_free(content_s);
		content_s=NULL;
	}
	return content_s;
}



/*此identifier在define trei树中，返回末尾结点define_data_t型结点指针*/
define_data_t *ast_preprocess_finddefine(trei_root_t *define_root,char *identifier){
	if(define_root==NULL || identifier==NULL)
		return NULL;
	string_t *define_s=NULL;
	char *word=NULL;
	trei_node_t *endnode=NULL;
	define_data_t *define_data=NULL;
	link_head *define_data_linknode=NULL;

	endnode=trei_findword(define_root,identifier);
	if(endnode==NULL)
		return NULL;
	define_data_linknode=endnode->child.next;
	define_data=(define_data_t *)struct_entry(define_data_linknode,define_data_t,define_data_linknode);
	return define_data;
}






