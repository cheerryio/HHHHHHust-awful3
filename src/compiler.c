

#include <ast_analyze.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "zlog.h"

/*引用文件的生疏，可以把整个目录加入到项目的includes里，就不用每次写前缀*/
#include "linklist.h"
#include "instruct.h"
#include "gdef.h"
#include "array.h"
#include "mempool.h"
#include "pstring.h"
#include "ptree.h"
#include "trei.h"
#include "ast_base.h"
#include "ast_preprocess.h"
#include "ast_analyze.h"


int main(void){
	INIT_ZLOG(ZLOG_CONF,ZLOG_CATEGORY);	/* log 为初始化成功后的log指针*/

	pool_t *pool=create_pool(50480,log);
	ast_analyze_t *ast_analyze=ast_analyze_init(pool,log);
	if(pool==NULL) return -1;

	signal_t sig=1;
	array_t *arr=NULL;
	int *a=NULL;
	char *pc=NULL;
	char c;
	char *c1,*c2;
	int d,e,f,g;
	char elem[]="----lovelovelove----";
	string_t *s=NULL;
	string2_t *s2=NULL;
	memsize_t gar[]=MEMSIZE_STEP;
	ptree_root_t *root=NULL;
	ptree_node_t *node=NULL,*node1=NULL,*node2=NULL,*node3=NULL;
	ast_locationinfo_t *location=ast_analyze->location;
	FILE *fp=ast_analyze->ast_file_i;
	ast_root_t *ast_root=ast_analyze->root;
	FILE *fp1,*fp2;
	ast_content_t content;
	char buffer[5];
	const aaaa=1;
	node=ast_node_init(pool,&content);
	node->depth=1;
	while(sig){
		/*
		 * 输入1:测试
		 * 输入2:读取csrc.c文件并根据源程序生成代码树
		 */
		fputs("input:",stdout);
		scanf("%d",&sig);
		switch(sig){
		case 1:
			content=ast_getcontent(fp, location, pool, log);
			if(content.raw!=NULL){
				pc=string_fetchs(content.raw);
				printf("%s\n",pc);
			}
			break;
		case 2:
			ast_analyze_main(ast_analyze);
			break;
		case 3:
			fopen("out.c","w");
		case 4:
		default:
			sig=1;
			break;
		}
	}

	return 0;
}










