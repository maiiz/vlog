#ifndef __vlog_util_h
#define __vlog_util_h

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#define MAX_LEN 1024
#define MAX_CFG_LINE MAX_LEN*2
#define MAX_FUNC_NUM 4
#define ARRAY_DEFAULT_SIZE MAX_FUNC_NUM*8

#define inner_log(fmt, args...)\
	vlog_inner_log(__FILE__, __LINE__, fmt, ##args)

#define DEFAULT_OUTPUT_FORMAT "%d[%F %X.%ms][%p](%c:%F:%l)-%m%n"

typedef enum
{
	LOG_DEBUG,
	LOG_INFO,
	LOG_NOTICE,
	LOG_WARN, 
	LOG_ERROR,
	LOG_FATAL
}vlog_level;

static struct
{
	vlog_level level;
	char *level_name;
} keywords []= {
{LOG_DEBUG, "debug"},
{LOG_INFO, "info"},
{LOG_NOTICE, "notice"},
{LOG_WARN, "warn"},
{LOG_ERROR, "error"},
{LOG_FATAL, "fatal"},
{-1, NULL}};

char *get_msg(const char *fmt,va_list args);
int get_level(const char *level_name);
//get the time str
void get_time(char *time_str);
//str trim
char *trim(char *str);
//str split
char *split(char *str);
//parse the size according the number depending on the suffix M,Kb
size_t parse_byte_size(char *str);
//for debug
void vlog_inner_log(const char *file, long line, const char *fmt,...);

#endif
