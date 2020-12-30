


#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "array.h"
#include "linklist.h"
#include "mempool.h"
#include "pstring.h"


/**用一维数组初始化array，只能是char的一维
 * 注意要从一维数组的末尾开始进行push
 */
void string_assign(string_t *s,void *elems){
	if(elems==NULL)
		return ;
	string_fresh(s);
	s->data.bysize=sizeof(char);	/*bysize跟随传入的len*/
	char *elem=NULL;
	elem=(char *)elems;
	elem--;
	do
		string_push(s,++elem);
	while(*elem!='\0');

	return ;
}

/*将s中的字符串数据放在elem里面*/
void string_giveelem(string_t *s,char *elem){
	if(s==NULL || elem==NULL)
		return ;
	signal_t sig=-1;
	char *p=elem;
	for(sig=0;sig<s->elem_sum;sig++)
		*p++=*((char *)array_get(s,sig));
	*p='\0';
	return ;
}

/**取出字符串，返回char *型 指针
 * 由于可能有很多区块，可以计算出目标字符串的长度上限，
 * 遍历所有区块，把字符串里面的每一个字符都给拿到
 * 仅仅需要调用array_get(...) 函数
 */
char *string_fetchs(string_t *s){
	if(s==NULL)
		return NULL;

	size_t len=-1;
	size_t sig=-1;
	char *str=NULL,*p=NULL;
	pool_t *pool=NULL;

	pool=s->pool;
	len=s->elem_sum;
	str=(char *)palloc(pool,len+1);	/*多出一个'\0'的空间*/
	if(str==NULL)
		return NULL;
	p=str;
	for(sig=0;sig<len;sig++)
		*p++=*((char *)array_get(s,sig));
	*p='\0';	/*末尾补上\0结尾*/

	return str;
}

/*将s2的内容合并到s1,返回s1*/
char *string_concat(string_t *s1,string_t *s2){
	if(s1==NULL || s2==NULL)
		return NULL;
	char *pc=NULL;

	pc=string_fetchs(s2);
	string_pushs(s1,pc);
	return s1;
}

/*不改变s1,s2,,返回内容合并后的char*指针*/
char *string_concat_fetch(string_t *s1,string_t *s2){
	if(s1==NULL || s2==NULL)
		return NULL;
	char *c1=NULL,*c2=NULL,*c3=NULL;
	pool_t *pool=NULL;
	pool=s1->pool;
	c1=string_fetchs(s1);
	c2=string_fetchs(s2);
	strcat(c1,c2);
	return c1;
}

/*加入一个字符，对给的s，最后一个字符一定'\0',先pop再push进c再push进'\0'*/
void string_pushc(string_t *s,char *pc){
	if(s==NULL)
		return ;
	char c='\0';
	if(s->elem_sum>0)
		array_pop(s);
	array_push(s,pc);
	array_push(s,&c);
	return ;
}

/*给定字符串，push进这个字符串，以 '\0' 为结束标志*/
void string_pushs(string_t *s,char *pc){
	if(s==NULL || pc==NULL)
		return ;
	char *p=pc;;
	string_pop(s);	/*	取出原来最后的 '\0' */
	do
		string_push(s,p);
	while(*p++!='\0');

	return ;
}

/*取出末尾字符，注意避开'\0'*/
char *string_popc(string_t *s){
	if(s==NULL)
		return NULL;
	char *pc;
	char cend='\0';
	string_pop(s);
	pc=(char *)string_pop(s);
	string_push(s,&cend);

	return pc;
}

/*内部调用strcmp,返回其返回的结果*/
signal_t string_compare(string_t *s1,string_t *s2){
	if(s1==NULL || s2==NULL)
		return -2;
	pool_t *pool=s1->pool;
	char *pc1=NULL,*pc2=NULL;
	signal_t compare_sig=-2;
	pc1=(char *)string_fetchs(s1);
	pc2=(char *)string_fetchs(s2);
	compare_sig=strcmp(pc1,pc2);
	pfree(pool,pc1,s1->elem_sum+1);
	pfree(pool,pc2,s2->elem_sum+1);

	return compare_sig;
}

/*返回字符串长度*/
size_t string_len(string_t *s){
	if(s==NULL)
		return ZERO;
	return array_len(s)-1;
}

/*实现字符串的反转*/
void string_reverse(string_t *s){
	if(s==NULL)
		return ;
	char *elems=NULL;

	elems=string_fetchs(s);
	string_assign(s,elems);	/*用反转后的数组对s重新赋值*/

	return ;
}

/*字符串的二位数组封装,起名string2，本质是一个array_t里面元素是string_t指针*/

void string2_assign(string2_t *s2,char **elems,size_t n){
	if(s2==NULL || elems==NULL)
		return ;
	string2_fresh(s2);	/*s2回到刚初始化时的状态*/
	string_t *s=NULL;
	signal_t sig=-1;
	pool_t *pool=NULL;
	char *ch=NULL;

	pool=s2->pool;
	for(sig=0;sig<n;sig++){
		s=String(pool,elems[sig]);
		string2_push(s2,&s);
	}

	return ;
}

/*有elem创建一个string_t,把string_t push到string2_t结构中*/
void string2_pushs(string2_t *s2,char *elem){
	if(s2==NULL || elem==NULL)
		return ;
	string_t *s=NULL;
	pool_t *pool=NULL;

	pool=s2->pool;
	s=String(pool,elem);
	string2_push(s2,&s);

	return ;
}

char *string2_pops(string2_t *s2){
	if(s2==NULL)
		return NULL;
	if(s2->elem_sum<=0)
		return NULL;
	string_t **ps=NULL;
	char *elem=NULL;

	ps=(string_t **)string2_pop(s2);
	if(ps==NULL)		/*已无字符串*/
		return NULL;
	elem=(char *)string_fetchs(*ps);
	string_free(*ps);

	return elem;
}

/*通过次序号返回字符串*/
char *string2_fetchs(string2_t *s2,sequence_t sequence){
	if(s2==NULL)
		return NULL;
	if(sequence<0 || sequence>string2_len(s2)-1)
		return NULL;
	size_t elem_sum=-1;
	string_t **ps=NULL;
	char *pc=NULL;

	ps=array_get(s2,sequence);
	pc=(char *)string_fetchs(*ps);
	return pc;
}

string_t *string2_get(string2_t *s2,sequence_t sequence){
	if(s2==NULL)
		return NULL;
	if(sequence<0 || sequence>string2_len(s2)-1)
		return NULL;
	string_t **ps=NULL;
	ps=(string_t **)array_get(s2,sequence);
	return *ps;
}

/*寻找elem，返回此元素次序，未找到返回-1*/
sequence_t string2_finds(string2_t *s2,char *elem){
	if(s2==NULL || elem==NULL)
		return -1;
	char *pc=NULL;
	signal_t findsig=FALSE;
	signal_t sig=-1;
	size_t elem_sum=s2->elem_sum;
	pool_t *pool=s2->pool;

	for(sig=ZERO;sig<elem_sum;sig++){
		pc=string2_fetchs(s2,sig);
		if(strcmp(pc,elem)==STRCMP_EQUAL){
			findsig=TRUE;
			pfree(pool,pc,strlen(pc)+1);
			break;
		}
		pfree(pool,pc,strlen(pc)+1);
	}
	if(findsig==FALSE)
		sig=-1;
	return sig;
}

/*根据内容删除*/
void string2_remove(string2_t *s2,char *elem){
	if(s2==NULL || elem==NULL)
		return ;
	sequence_t seq=-1;

	seq=string2_find(s2,elem);
	if(seq==-1)
		return ;

	return ;
}


/*free每一个string_t ,再free string2_t*/
void string2_free(string2_t *s2){
	if(s2==NULL)
		return ;
	signal_t sig=-1;
	size_t elem_sum=-1;
	string_t *s=NULL;

	elem_sum=s2->elem_sum;
	for(sig=ZERO;sig<elem_sum;sig++){
		s=(string_t *)string2_get(s2,sig);
		string_free(s);
	}
	array_free(s2);
}

















