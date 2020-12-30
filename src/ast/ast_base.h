

#ifndef AST_AST_BASE_H_
#define AST_AST_BASE_H_

/*ast 输出时只是这是什么东西*/
enum Content_def {
	variable_def
};


/*保持space-identifier的相对位置与数值不会改变，否则getcontent函数出错*/
enum Content_type {
	space=-2,none=-1,
	if_e,else_e,while_e,for_e,return_e,break_e,continue_e,
	int_e,char_e,float_e,double_e,enum_e,
	short_e,long_e,unsigned_e,
	short_int,
	long_int,long_float,
	unsigned_int,unsigned_char,
	identifier_e,
	less,more,lessequal,moreequal,equal,plus,minus,plusplus,minusminus,plusequal,minusequal,
	division,multiply,divisionequal,multiplyequal,power,remainder_e,remainderequal,
	equalequal,not,notequal,and,or,
	left_bracket,right_bracket,left_middlebracket,right_middlebracket,left_bigbracket,right_bigbracket,
	doublequotation,singlequotation,semicolon,comma,point,
	number,
	linecomment,left_multilinecomment,right_multilinecomment,string,
	well,filename,define_content_e,
};

enum Role_type {
	not_node=0,semicolon_blank_node=1,program_node=2,macro_node=3,macro_content_node=4,statement_node=5,variable_declare_node=6,id_node=7,
	func_variable_node=8,func_declare_node,func_content_declare_node,funccall_node,
	if_node,while_node,return_node,break_node,continue_node,
	condition_node,body_node,ifbody_node,elsebody_node,block_node,comment_node
};

typedef struct ast_content_s ast_content_t;
struct ast_content_s {
	enum Content_type type;
	enum Role_type role;
	string_t *raw;
};


#define CASE_AST_EOF	\
	case EOF:

#define CASE_AST_DIGIT	\
	case '0':case'1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':

#define CASE_AST_LETTER	\
	case 'A':case'B':case 'C':case 'D':case'E':case 'F':case 'G':	\
	case 'H':case'I':case 'J':case 'K':case'L':case 'M':case 'N':	\
	case 'O':case'P':case 'Q':case 'R':case'S':case 'T':	\
	case 'U':case'V':case 'W':case 'X':case'Y':case 'Z':	\
		\
	case 'a':case'b':case 'c':case 'd':case'e':case 'f':case 'g':	\
	case 'h':case'i':case 'j':case 'k':case'l':case 'm':case 'n':	\
	case 'o':case'p':case 'q':case 'r':case's':case 't':	\
	case 'u':case'v':case 'w':case 'x':case'y':case 'z':

#define CASE_AST_BLANK	\
	case ' ':case '\t':case '\v':case '\f':case '\n':

#define CASE_AST_STRING	\
	case '\'': case '\"':

/*十六进制字符*/
#define CASE_AST_HEX_DIGIT \
	case '0':case'1':case '2':case '3':case'4':case '5':case '6':case '7':case'8':case '9':		\
	case 'A':case'B':case 'C':case 'D':case'E':case 'F':	\
	case 'a':case'b':case 'c':case 'd':case'e':case 'f':

/*八进制字符*/
#define CASE_AST_OCT_DIGIT \
	case '0':case'1':case '2':case '3':case'4':case '5':case '6':case '7':

/*long和short型，数字的后缀*/
#define CASE_AST_NUMBER_POSTFIX \
	case 'l':case 'L':case 'u':case 'U':

/*分块定界的运算符*/
#define CASE_AST_OPERATOR_SINGAL \
	case ',':case';':case '?':case '~':case '(':case ')':	\
	case '[':case']':case '{':case '}':

/*计算数字的运算符*/
#define CASE_AST_OPERATOR \
	case '+':case '-':case '*':case '/':case '&':case '|':case ':':	\
	case '=':case '^':case '!':case '#':case '>':case '<':

#define CASE_AST_SPLITE CASE_AST_BLANK CASE_AST_OPERATOR CASE_AST_OPERATOR_SINGAL

#define ast_filerewind(_fp,_log)	\
	if(fseek((_fp),0,SEEK_SET)==ZERO)	\
		NULL;	\
	else	\
		LOG_ERROR((_log),"ast filerewind error");

#define ast_getc(_fp) (fgetc(_fp))

#define ast_ungetc(_fp,_c) (ungetc(*(_c),_fp))

#define ast_ungets(_fp,_pc,_location) ({	\
	int len=strlen(_pc);	\
	int i;	\
	for(i=0;i<len;i++){	\
		if(*((_pc)+len-i-1)=='\n')	\
			(_location)->line-=1;	\
		(_location)->count-=1;	\
		ungetc(*((_pc)+len-i-1),(_fp));	\
	}	\
})

#define ast_fopen(_file,_mode,_log) ({	\
	FILE *fp=NULL;	\
	if((fp=fopen((_file),(_mode)))==NULL){	\
		LOG_ERROR((_log),"open file fail,errno:,reason:");	\
		exit(1);	\
	}	\
	fp;	\
})

#define ast_ignore_blank(_fp,_location,_pool,_log) ({	\
	char c;	\
	signal_t findthing_sig=FALSE;	\
	while((c=ast_getc(fp))!=EOF){	\
		if(findthing_sig==TRUE)	\
			break;	\
		switch(c){	\
		case ' ':case '\t':case '\v':case '\f':	\
			(_location)->count+=1;	\
			break;	\
		default:	\
			findthing_sig=TRUE;	\
			ast_ungetc(_fp,&c);	\
			break;	\
		}	\
	}	\
	ast_ungetc(_fp,&c);	\
})


#define count_forward(_count,_n) ((_count)+=(_n))

#define ast_indent(_n) ({	\
	signal_t sig=-1;	\
	for(sig=0;sig<(_n);sig++)	\
		printf("   ");	\
})


typedef uint32_t loc_t;

typedef struct ast_locationinfo_s ast_locationinfo_t;
struct ast_locationinfo_s {
	string_t *file;
	loc_t line;
	loc_t count;
	loc_t block_depth;
};

string_t *ast_getword(FILE *fp,string_t *s);
string_t *ast_getword_n(FILE *fp,pool_t *pool);
char ast_charactor(FILE *fp);
ast_content_t ast_getstring(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);

static char ast_zhuanyi_charactor(FILE *fp);
string2_t *ast_getvariable(FILE *fp,pool_t *pool,log_t *log);

ast_content_t ast_getcontent(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
static ast_content_t ast_getcontent_getword(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
static ast_content_t ast_getcontent_gettoken(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
static ast_content_t ast_getcontent_getnumber(FILE *fp,ast_locationinfo_t *location,pool_t *pool,log_t *log);
static signal_t ast_operator_rank(ast_content_t content);
signal_t ast_operator_compare(ast_content_t content1,ast_content_t content2);







#endif
