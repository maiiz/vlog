#ifndef __Vlog_format_h
#define __vlog_format_h
#include "util.h"
typedef struct vlog_format_s vlog_format_t;
struct vlog_format_s
{
	char *name;
	char *pattern;	
	vlog_format_t *next;
};

vlog_format_t *vlog_format_new(const char *name, const char *pattern);
void vlog_format_del(vlog_format_t *format);
#endif
