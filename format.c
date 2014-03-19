#include "format.h"

vlog_format_t *vlog_format_new(const char *name, const char *pattern)
{
 	vlog_format_t *a_format = NULL;
	a_format = (vlog_format_t *)calloc(1, sizeof(vlog_format_t));
	a_format->name = (char*)calloc(strlen(name)+1, sizeof(char));
	a_format->pattern = (char*)calloc(strlen(pattern)+1, sizeof(char));
	strncpy(a_format->name, name, strlen(name));
	strncpy(a_format->pattern, pattern, strlen(pattern));
	a_format->next = NULL;
	return a_format;
}

void vlog_format_del(vlog_format_t *a_format)
{
	if(a_format)
	{
		if(a_format->name)
		{
			free(a_format->name);
		}
		if(a_format->pattern)
		{
			free(a_format->pattern);
		}
		if(a_format->next)
		{
			vlog_format_del(a_format->next);
		}
		free(a_format);
	}
}
