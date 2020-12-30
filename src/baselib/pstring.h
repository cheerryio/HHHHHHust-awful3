

/**
 * string类型整体基于bysize=1的array_t类型
 * 所有操作都基于array_t
 *
 * 践行string模块基于array这句话，但是array整体是一个大模块，总体概念，
 * 例如把array的bysize为1的所有小的string东西还是要自己写函数来说
 */


#ifndef PSTRING_H_
#define PSTRING_H_

#include <stdint.h>

#include "array.h"
#include "linklist.h"
#include "string.h"

typedef array_t string_t;	/*string基于bysize=1的array*/
typedef array_t string2_t;	/*二维数组，存字符串的数组*/


/*array所有函数里面的名字array改成了string*/
#define string_init(_pool) (init_array(_pool,1))
#define string_fresh(_s) (array_fresh(_s))
#define string_get(_s,_sequence) (array_get(_s,_sequence))
#define string_free(_s) (array_free(_s))
#define string_empty(_s) (array_empty(_s))
#define string_pop(_s) (array_pop(_s))
#define string_push(_s,_pc) (array_push(_s,_pc))
void string_giveelem(string_t *s,char *elem);

/*string2的宏*/
#define string2_init(_pool) (init_array(_pool,sizeof(string_t *)))
#define string2_fresh(_s2) (array_fresh(_s2))
#define string2_push(_s2,_s) (array_push(_s2,_s))
#define string2_pop(_s2) (array_pop(_s2))
#define string2_len(_s2) (array_len(_s2))
#define string2_find(_s2,_s) (array_find(_s2,_s))

/*快速为一个string赋值*/
#define String(_pool,_elem) ({	\
	string_t *s=string_init(_pool);	\
	string_assign(s,_elem);	\
	s;	\
})

/*快速为string2赋值*/
#define String2(_pool,_elems,_n) ({	\
	string2_t *s2=string2_init(_pool);	\
	string2_assign(s2,_elems,_n);	\
	s2;	\
})


/*--------------------函数声明------------------------*/
void string_assign(string_t *s,void *elems);
char *string_fetchs(string_t *s);
void string_reverse(string_t *s);
void string_pushc(string_t *s,char *pc);
char *string_popc(string_t *s);
char *string_concat(string_t *s1,string_t *s2);
char *string_concat_fetch(string_t *s1,string_t *s2);

void string2_assign(string2_t *s2,char **elems,size_t n);
void string2_pushs(string2_t *s2,char *elem);
char *string2_pops(string2_t *s2);
void string2_free(string2_t *s2);
string_t *string2_get(string2_t *s2,sequence_t sequence);
char *string2_fetchs(string2_t *s2,sequence_t sequence);
sequence_t string2_finds(string2_t *s2,char *elem);







#endif
