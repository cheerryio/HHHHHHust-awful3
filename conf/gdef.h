


#ifndef GDEF_H_
#define GDEF_H_

#include <stdint.h>
#include <stdio.h>
#include "zlog.h"

#define TRUE 1
#define OK 1
#define FALSE 0
#define ERROR 0
#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define ABNORMAL -1
#define STRCMP_EQUAL 0



#define ZLOG_CONF "conf/zlog.conf"
#define ZLOG_CATEGORY "elegen"

#define AST_MAIN_FILENAME "csrc"
#define AST_MAIN_C "csrc.c"
#define AST_MAIN_I "csrc.i"

#define AST_FILENAMES {"csrc","aaa"}
#define AST_SOURCE {"csrc.c","aaa.c"}	/*务必将main函数文件放在第一个*/
#define AST_FILENAMES_SIZE 2
#define AST_SOURCE_SIZE 2

#define LOG_ON
#ifdef LOG_ON
#define LOG_DEBUG(log,fmt,args...) zlog_debug(log,fmt,##args)
#define LOG_INFO(log,fmt,args...) zlog_info(log,fmt,##args)
#define LOG_NOTICE(log,fmt,args...) zlog_notice(log,fmt,##args)
#define LOG_WARN(log,fmt,args...) zlog_warn(log,fmt,##args)
#define LOG_ERROR(log,fmt,args...) zlog_error(log,fmt,##args)
#define LOG_FATAL(log,fmt,args...) zlog_warn(log,fmt,##args)

#else
#define LOG_DEBUG(log,fmt,args...)
#define LOG_INFO(log,fmt,args...)
#define LOG_NOTICE(log,fmt,args...)
#define LOG_WARN(log,fmt,args...)
#define LOG_ERROR(log,fmt,args...)
#define LOG_FATAL(log,fmt,args...)

#endif



typedef char signal_t;
typedef uint8_t status_t;
typedef unsigned char u_char;
typedef size_t memsize_t;
typedef uint8_t bool_t;
typedef bool_t bool;	/*更简便使用，少一些字符*/
typedef char sequence_t;

typedef zlog_category_t log_t;


#endif















