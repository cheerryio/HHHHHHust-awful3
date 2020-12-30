
#include <stdio.h>
#include <stdlib.h>

#include "mempool.h"
#include "string.h"
#include "gdef.h"
#include "trei.h"
#include "ast_preprocess.h"
#include "ast_base.h"
#include "linklist.h"

/*得到字符串里面的转移字符（已知\开头）*/
#define CHARACTOR_STATE_ENUM enum charactor_state {	\
	none,possible_charactor,one_charactor	\
}
#define CASE_ZHUANYI_CHARACTOR	\
	case 'a':case 'b':case 'f':case 'n':case 'r':case 't':case 'v':	\
	case '\\':case '\'':case '\"':case '\0':
static char ast_zhuanyi_charactor(FILE *fp){
	if(fp==NULL)
		return NULL;
	string_t *s=NULL;
	char c=-1;
	CHARACTOR_STATE_ENUM state=none;
	while(state!=one_charactor){
		switch(state){
		case none:
			if((c=ast_getc(fp))==EOF)
				return -1;
			switch(c){
			case '\\':
				state=possible_charactor;
				break;
			default:
				state=one_charactor;
				break;
			}
			break;
		case possible_charactor:
			if((c=ast_getc(fp))==EOF)
				return -1;
			switch(c){
			CASE_ZHUANYI_CHARACTOR
				state=one_charactor;
				switch(c){
				case 'a':
					c='\a';
					break;
				case 'b':
					c='\b';
					break;
				case 'f':
					c='\f';
					break;
				case 'n':
					c='\n';
					break;
				case 'r':
					c='\r';
					break;
				case 't':
					c='\t';
					break;
				case 'v':
					c='\v';
					break;
				case '0':
					c='0';
					break;
				default:
					break;
				}
			break;
			default:
				return -1;
			}
			break;

		}
	}
	if(state!=one_charactor)
		c=-1;
	return c;
}

/*获得字符串里面的下一个字符，可识别转义字符*/
char ast_charactor(FILE *fp){
	if(fp==NULL)
		return -1;
	char c;
	c=ast_getc(fp);
	switch(c){
		case '\\':
			ast_ungetc(fp,&c);
			c=ast_zhuanyi_charactor(fp);
			if(c==-1)	/*未得到应有的转义字符，异常*/
				return -1;
			break;
		default:
			break;
	}
	return c;
}

/*读参数名字，返回string2_t*/
string2_t *ast_getvariable(FILE *fp,pool_t *pool,log_t *log){
	sequence_t seq=-1;
	char c;
	string_t *s=NULL;
	enum Variable_state {
		none,variable,variable_end,variable_success
	} variable_state;
	string2_t *variables_s2=string2_init(pool);

	/*处理define可能有的变量。每一个状态无需判断variable_end,此状态一定已经退出*/
	variable_state=none;
	while((c=ast_getc(fp))!=EOF){
		switch(c){
		case '(':
			if(variable_state==none)
				variable_state=variable;
			else
				LOG_ERROR(log,"( may not appear in macro param list");
			break;
		CASE_AST_LETTER case '_':
			ast_ungetc(fp,&c);
			if(variable_state==variable){
				s=ast_getword_n(fp,pool);	/*s从新内存中创建的*/
				if((seq=string2_find(variables_s2,&s))==-1)
					string2_push(variables_s2,&s);	/*加入地址*/
				/*找到重名变量名，报错*/
				else{
					LOG_ERROR(log,"variables with same name");
					goto AST_VARIABLE_END;
				}
				variable_state=variable_end;	/*读完一个参数，后面可能 ) 可能,*/
			}
			else{
				LOG_ERROR(log,"unexpected occurrence:%c",c);
				goto AST_VARIABLE_END;
			}
			break;
		case ',':	/*开始读新一个参数*/
			if(variable_state==variable_end)
				variable_state=variable;
			else{
				LOG_ERROR(log,"unexpected %c when reading define variables",c);
				goto AST_VARIABLE_END;
			}
			break;
		case ')':
			if(variable_state==variable_end)
				variable_state=variable_success;
			else
				LOG_ERROR(log,"unexpected %c when reading define variables",c);
			goto AST_VARIABLE_END;
			break;
		CASE_AST_BLANK	/*忽略空白符*/
			break;
		default:
			LOG_ERROR(log,"unexpected %c when reading define variables",c);
			goto AST_VARIABLE_END;
			break;
		}
	}
	AST_VARIABLE_END:;
	if(variable_state==variable_success)
		NULL;
	else{
		string2_free(variables_s2);
		return NULL;
	}
	return variables_s2;
}

/*返回此结构值，不是指针*/
ast_content_t ast_getcontent(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log){
	ast_content_t content;
	content.type=none;
	char blank=' ';
	char c;
	char *pc=NULL;
	signal_t errsig=FALSE;
	AST_GETCONTENT_SPACE_CYCLE:;
	c=ast_getc(fp);
	ast_ungetc(fp,&c);
	switch(c){
	case EOF:
		LOG_ERROR(log,"unexpected end when get content");
		errsig=TRUE;
		content.type=none;
		break;
	case ' ':case '\t':case '\v':case '\f':
		ast_getc(fp);
		location->count+=1;
		goto AST_GETCONTENT_SPACE_CYCLE;
		break;
	case '\n':
		ast_getc(fp);
		location->line+=1;
		goto AST_GETCONTENT_SPACE_CYCLE;
		break;
	CASE_AST_LETTER case '_':	/*标识符关键字*/
		content=ast_getcontent_getword(fp,location,pool,log);
		break;
	CASE_AST_DIGIT
		content=ast_getcontent_getnumber(fp,location,pool,log);
		break;
	default:
		content=ast_getcontent_gettoken(fp,location,pool,log);
		break;
	}
	content.role=not_node;
	return content;
}

/*下两个函数的公共部分*/
#define AST_GETIDENTIFIER_PUBLIC	\
	char c;	\
	signal_t findsig=FALSE;	\
	signal_t notfirst=FALSE;	\
	while((c=ast_getc(fp))!=EOF){	\
		switch(c){	\
		CASE_AST_LETTER CASE_AST_DIGIT case '_':	\
			findsig=TRUE;	\
			string_pushc(s,&c);	\
			break;	\
		default:	\
			ast_ungetc(fp,&c);	\
			goto AST_IDENTIFIER_END;	\
			break;	\
		}	\
	}	\
	AST_IDENTIFIER_END:;
/*返回的string_t 只是参数中s的赋值*/
string_t *ast_getword(FILE *fp,string_t *s){
	if(fp==NULL || s==NULL)
		return NULL;
	string_fresh(s);
	/*已知下一个字符为字母或下划线*/
	AST_GETIDENTIFIER_PUBLIC;
	return s;
}

/*返回的string是新在内存中创建的*/
string_t *ast_getword_n(FILE *fp,pool_t *pool){
	if(fp==NULL || pool==NULL)
		return NULL;
	string_t *s=String(pool,"");
	AST_GETIDENTIFIER_PUBLIC;
	if(findsig==FALSE){
		string_free(s);
		s=NULL;
	}
	return s;
}


static ast_content_t ast_getcontent_getword(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log){
	ast_content_t content;
	char blank=' ';
	char c;
	char *pc=NULL;
	string_t *s=NULL,*ps=NULL,*type_s=NULL;
	sequence_t seq1=-1,seq2=-1;
	signal_t errsig=FALSE;
	char *content_types[]={
			"if","else","while","for","return","break","continue","int","char","float","double","enum","short","long","unsigned"};
	char *variable_prefix_types[]={"short","long","unsigned","const"};
	char *variable_postfix_types[]={"int","char","float","double"};
	string2_t *content_types_s2=String2(pool,content_types,sizeof(content_types)/sizeof(content_types[0]));
	string2_t *variable_postfix_types_s2=String2(pool,variable_postfix_types,sizeof(variable_postfix_types)/sizeof(variable_postfix_types));
	enum Content_type content_type;

	content_type=none;
	c=ast_getc(fp);
	ast_ungetc(fp,&c);
	switch(c){
		case EOF:
			LOG_ERROR(log,"unexpected end of file when getting word");
			errsig=TRUE;
			break;
		CASE_AST_LETTER case '_':
			s=ast_getword_n(fp, pool);
			pc=string_fetchs(s);
				/*s得到了control的关键字*/
			seq1=string2_finds(content_types_s2,pc);
			switch(seq1){
			case none:	/*没有找到,属于标识符*/
				content.type=identifier_e;
				content.raw=s;
				break;
			case short_e:case long_e:case unsigned_e:	/*判断后面还有后续char int float double*/
				ast_ignore_blank(fp,location,pool,log);
				ps=ast_getword_n(fp,pool);
				if(ps==NULL){
					LOG_ERROR(log,"cant find identifier in variable declare sentense");
					break;
				}
				pc=string_fetchs(ps);
				seq2=string2_finds(variable_postfix_types_s2,pc);
				switch(seq2){	/*搜索 int,char,float,double */
				case none:	/*得到了identifier,需要还回去*/
					ast_ungets(fp,pc,location);
					content.type=seq1;
					content.raw=s;
					break;
				case 0:	/*int*/
					switch(seq1){
					case short_e:
						string_pushc(s,&blank);
						string_pushs(s,pc);
						content.type=short_int;
						content.raw=s;
						break;
					case long_e:
						string_pushc(s,&blank);
						string_pushs(s,pc);
						content.type=long_int;
						content.raw=s;
						break;
					case unsigned_e:
						string_pushc(s,&blank);
						string_pushs(s,pc);
						content.type=unsigned_int;
						content.raw=s;
						break;
					}
					break;
				case 1:	/*char*/
					switch(seq1){
					case unsigned_e:
						string_pushc(s,&blank);
						string_pushs(s,pc);
						content.type=unsigned_char;
						content.raw=s;
						break;
					default:
						LOG_ERROR(log,"char only matches unsigned when declare variable");
						errsig=TRUE;
						break;
					}
					break;
				case 2:	/*float*/
					switch(seq1){
					case long_e:
						string_pushc(s,&blank);
						string_pushs(s,pc);
						content.type=long_float;
						content.raw=s;
						break;
					default:
						LOG_ERROR(log,"float only matches long when declare variable");
						errsig=TRUE;
						break;
					}
					break;
				case 3:	/*double*/
					LOG_ERROR(log,"double dont have variable declare prefix");
					errsig=TRUE;
					break;
				default:
					LOG_ERROR(log,"not known error");
					break;
				}
				break;
			default:
				content.type=seq1;
				content.raw=s;
				break;
			}
			break;
		default:
			LOG_ERROR(log,"unexpected letter %c when get word",c);
			errsig=TRUE;
			break;
	}
	return content;
}

static ast_content_t ast_getcontent_gettoken(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log){
	ast_content_t content;
	char c,a;
	char *pc=NULL;
	string_t *s=NULL;
	signal_t errsig=FALSE;
	char *pcend_sig='\0';

	s=String(pool,"");
	c=ast_getc(fp);
	switch(c){
	case EOF:
		LOG_ERROR(log,"unexpected end when get content get token");
		errsig=TRUE;
		break;
	case '<':
		string_pushc(s,&c);
		content.type=less;
		break;
	case '>':
		string_pushc(s,&c);
		content.type=more;
	case '=':
		string_pushc(s,&c);
		if((c=ast_getc(fp))=='='){
			string_pushc(s,&c);
			content.type=equalequal;
		}
		else{
			ast_ungetc(fp,&c);
			content.type=equal;
		}
		break;
	case '+':
		string_pushc(s,&c);
		if((c=ast_getc(fp))=='+'){
			string_pushc(s,&c);
			content.type=plusplus;
		}else if(c=='='){
			string_pushc(s,&c);
			content.type=plusequal;
		}else{
			ast_ungetc(fp,&c);
			content.type=plus;
		}
		break;
	case '-':
		string_pushc(s,&c);
		if((c=ast_getc(fp))=='-'){
			string_pushc(s,&c);
			content.type=minusminus;
		}else if(c=='='){
			string_pushc(s,&c);
			content.type=minusequal;
		}else{
			ast_ungetc(fp,&c);
			content.type=minus;
		}
		break;
	case '*':
		string_pushc(s,&c);
		if((c=ast_getc(fp))=='*'){
			string_pushc(s,&c);
			content.type=power;
		}else if(c=='='){
			string_pushc(s,&c);
			content.type=multiplyequal;
		}else if(c=='/'){
			string_pushc(s,&c);
			content.type=right_multilinecomment;
		}else{
			ast_ungetc(fp,&c);
			content.type=multiply;
		}
		break;
	case '/':
		string_pushc(s,&c);
		if((c=ast_getc(fp))=='*'){
			string_pushc(s,&c);
			content.type=left_multilinecomment;
		}else if(c=='/'){
			string_pushc(s,&c);
			content.type=linecomment;
		}else if(c=='='){
			string_pushc(s,&c);
			content.type=divisionequal;
		}else{
			ast_ungetc(fp,&c);
			content.type=division;
		}
		break;
	case '%':
		string_pushc(s,&c);
		content.type=remainder_e;
		break;
	case '!':
		string_pushc(s,&c);
		if((c=ast_getc(fp))=='='){
			string_pushc(s,&c);
			content.type=notequal;
		}else{
			ast_ungetc(fp,&c);
			content.type=not;
		}
		break;
	case '&':
		string_pushc(s,&c);
		a=c;
		if((c=ast_getc(fp))=='&'){
			string_pushc(s,&c);
			content.type=and;
		}else{
			ast_ungetc(fp,&c);
			ast_ungetc(fp,&a);
			LOG_ERROR(log,"get content get token error,meet %c and %c",a,c);
			errsig=TRUE;
		}
		break;
	case '|':
		string_pushc(s,&c);
		a=c;
		if((c=ast_getc(fp))=='|'){
			string_pushc(s,&c);
			content.type=or;
		}else{
			ast_ungetc(fp,&c);
			ast_ungetc(fp,&a);
			LOG_ERROR(log,"get content get token error,meet %c and %c",a,c);
			errsig=TRUE;
		}
		break;
	case '(':
		string_pushc(s,&c);
		content.type=left_bracket;
		break;
	case ')':
		string_pushc(s,&c);
		content.type=right_bracket;
		break;
	case '[':
		string_pushc(s,&c);
		content.type=left_middlebracket;
		break;
	case ']':
		string_pushc(s,&c);
		content.type=right_middlebracket;
		break;
	case '{':
		string_pushc(s,&c);
		content.type=left_bigbracket;
		break;
	case '}':
		string_pushc(s,&c);
		content.type=right_bigbracket;
		break;
	case '\"':
		string_pushc(s,&c);
		content.type=doublequotation;
		break;
	case '\'':
		string_pushc(s,&c);
		content.type=singlequotation;
		break;
	case ';':
		string_pushc(s,&c);
		content.type=semicolon;
		break;
	case ',':
		string_pushc(s,&c);
		content.type=comma;
		break;
	case '#':
		string_pushc(s,&c);
		content.type=well;
		break;
	case '.':
		string_pushc(s,&c);
		content.type=point;
		break;
	default:
		LOG_ERROR(log,"in get content(get token function), not supported token %c",c);
		errsig=TRUE;
		break;
	}
	if(errsig==TRUE){
		content.raw=NULL;
		return content;
	}
	content.raw=s;
	return content;
}

/**
 * 获取数字,0b二进制，0八进制，十进制，0x十六进制，区分正负号，
 * 无符号u，l为long型，可以浮点long即long double 5.1L
 * 函数读到 + 不写入，设置状态
 */
static ast_content_t ast_getcontent_getnumber(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log){
	ast_content_t content;
	content.type=none;
	char c;
	char relc;
	string_t *number_s=NULL;
	enum Number_type {
		none,t8,t10,t16
	} number_type;
	enum Number_state {
		number_start,prefix0,justzero,prefixend,integer,decimal,postfix,number_end,error
	} number_state;
	enum Number_value {
		n1=1,n2,n3,n4,n5,n6,n7,n8,n9
	} number_value;

	number_s=String(pool,"");
	number_state=number_start;


#define AST_NUMBER_TYPE_CHECK	\
	relc=c-'0';	\
	switch(number_type){	\
	case none:	\
		goto AST_GETNUMBER_END;	\
		break;	\
	case t8:	\
		if(relc>n7){	\
			number_state=error;	\
			goto AST_GETNUMBER_END;	\
		}	\
		break;	\
	case t10:	\
		if(relc>n9){	\
			number_state=error;	\
			goto AST_GETNUMBER_END;	\
		}	\
	default:	\
		break;	\
	}


	while((c=ast_getc(fp))!=EOF){
		string_pushc(number_s,&c);
		printf("%c\n",c);
		switch(c){
		case '+':case '-':
			if(number_state==number_start)
				if(c=='-')
					NULL;
				else
					string_popc(number_s);	/*拿出加入的加号*/
			else{
				string_popc(number_s);	/*+ -号属于数字的分界符号*/
				ast_ungetc(fp,&c);
				goto AST_GETNUMBER_END;
			}
			break;
		case '0':
			printf("ininin");
			switch(number_state){
			case number_start:
				printf("%c\n",c);
				number_state=prefix0;
				c=ast_getc(fp);
				switch(c){
				CASE_AST_BLANK CASE_AST_EOF case';': case ',':
					number_state=justzero;
					ast_ungetc(fp,&c);
					goto AST_GETNUMBER_END;
				default:
					break;
				}
				break;
			case integer:case decimal:
				break;
			default:
				number_state=error;
				LOG_ERROR(log,"unexpected occurrence of %c",c);
				goto AST_GETNUMBER_END;
				break;
			}
			break;
		case 'x':case 'X':	/*此字母仅仅出现在十六进制前缀*/
			if(number_state==prefix0){
				number_state=prefixend;
				number_type=t16;
			}
			else{
				number_state=error;
				LOG_ERROR(log,"letter %c should only be 16 number prefix",c);
				goto AST_GETNUMBER_END;
			}
			break;
		case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
		case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':
			switch(number_state){
			case number_start:
				number_state=integer;
				number_type=t10;
				AST_NUMBER_TYPE_CHECK;
				break;
			case prefix0:	/*八进制数*/
				number_state=integer;
				number_type=t8;
				break;
			case prefixend:
				AST_NUMBER_TYPE_CHECK;
				number_state=integer;
				break;
			case integer:case decimal:
				AST_NUMBER_TYPE_CHECK;
				break;
			default:
				number_state=error;
				LOG_ERROR(log,"unexpected occurnce of %c",c);
				goto AST_GETNUMBER_END;
			}
			break;
		case '.':
			switch(number_state){
			case number_start:
				number_type=t10;
				number_state=decimal;
				break;
			case integer:
				number_state=decimal;
				break;
			default:
				number_state=error;
				LOG_ERROR(log,"unexpected letter %c",c);
				goto AST_GETNUMBER_END;
			}
			break;
		case 'u':case 'U':case 'l':case 'L':
			switch(number_state){
			case integer:case decimal:
				number_state=number_end;
				goto AST_GETNUMBER_END;
			default:
				goto AST_GETNUMBER_END;
			}
			break;
		default:
			ast_ungetc(fp,&c);
			string_popc(number_s);
			goto AST_GETNUMBER_END;
		}
	}
	AST_GETNUMBER_END:;
	switch(number_state){
	case integer:case decimal:case number_end:case justzero:
		content.type=number;
		content.raw=number_s;
		break;
	default:
		string_free(number_s);
		number_s=NULL;
		content.type=none;
		break;
	}
	return content;
}

/*分析文件时得到(已知双引号开头)字符串。字符串中接受有 \ 的 ' 和  "*/
ast_content_t ast_getstring(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log){
	char c=-1;
	string_t *s=NULL;
	ast_content_t content;
	signal_t errsig=FALSE;

	s=String(pool,"");
	c=ast_getc(fp);
	assert(c=='\"');
	while((c=ast_charactor(fp))!='\"'){
		if(c==-1 || c==EOF){
			LOG_ERROR(log,"unexpected end of string");
			errsig=TRUE;
			string_free(s);
			break;
		}
		string_pushc(s,&c);
	}
	if(errsig=TRUE)
		content.type=none;
	else{
		content.type=string;
		content.raw=s;
	}
	return content;
}

/**
 * 输入content,返回这个运算符优先级，否则返回-1,数字越小优先级越高
 */
static signal_t ast_operator_rank(ast_content_t content){
	signal_t rank=-1;
	switch(content.type){
	case plusplus:case minusminus:case not:
		rank=2;
		break;
	case multiply:case division:case remainder_e:
		rank=3;
		break;
	case plus:case minus:
		rank=4;
		break;
	case more:case less:case moreequal:case lessequal:
		rank=6;
		break;
	case equalequal:case notequal:
		rank=7;
		break;
	case equal:case divisionequal:case multiplyequal:
	case plusequal:case minusequal:case remainderequal:
		rank=14;
		break;
	}
	return rank;
}

signal_t ast_operator_compare(ast_content_t content1,ast_content_t content2){
	signal_t sig1=-1;
	signal_t sig2=-1;
	sig1=ast_operator_rank(content1);
	sig2=ast_operator_rank(content2);
	if(sig1<sig2)
		return 1;	/*1比2优先级高*/
	else if(sig1>sig2)
		return -1;
	else
		return 0;
}



























