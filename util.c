#include "util.h"
#include <time.h>
#include <sys/time.h>

//should free the memory after call this function
char *get_msg(const char* fmt, va_list args)
{
	int nread,size;
	int n =0;
	char *buf;	
	va_list ap;
	size = MAX_LEN;
	if((buf = (char*)calloc(size, sizeof(char)))==NULL)
	{
		return NULL;
	}	
	
	while(1)
	{
		va_copy(ap, args);
		nread = vsnprintf(buf, MAX_LEN, fmt, ap);
		if(nread>-1&&nread<size)
		{
			return buf;
		}
		else
		{
			size += MAX_LEN;
			if((buf = (char*)realloc(buf, size))==NULL)
			{
				return NULL;
			}
			memset(buf, 0x00, sizeof(buf));
		}
	}
}
int get_level(const char* level_name)
{
	int i;
	for(i=0; keywords[i].level_name; i++)
	{
		if(!strcasecmp(keywords[i].level_name, level_name))
			return keywords[i].level;
	}
	return -1;
}

void get_time(char *time_str)
{
	struct timeval tms;
	char buf[21];
	timerclear(&tms);
	gettimeofday(&tms,NULL);
	
	strftime(buf, sizeof(buf), "%F %T", localtime(&tms.tv_sec));
	sprintf(time_str, "%s.%d", buf, tms.tv_usec/1000);
	return;
}

char *trim(char *str)
{
    char *str_last,*str_cur;
    if(str==NULL)
        return;
    for(;*str==0x20 || *str=='\t'; ++str);
    for(str_last=str_cur=str;*str_cur!='\0';++str_cur)
        if(*str_cur!=0x20&&*str_cur!='\t')
            str_last=str_cur;
    *++str_last=0;
    return str;
} 

//parse the size according the number depending on the suffix M,Kb
size_t parse_byte_size(char *str)
{
	size_t sz;
	long res;
	int c,m;
	trim(str);
	sz = strlen(str);
	res = strtol(str, (char**)NULL, 10);
	if(res <= 0)
	{
		return 0;
	}
	if(str[sz-1] == 'B' || str[sz-1] == 'b')
	{
		c = str[sz-2];
		m = 1024;
	}
	else
	{
		c = str[sz-1];
		m = 1000;
	}
	switch(c)
	{
		case 'K':
		case 'k':
			res *= m;
			break;
		case 'm':
		case 'M':
			res *= m * m;
			break;
		case 'G':
		case 'g':
			res *= m * m * m;
			break;
		default:
			if (!isdigit(c))
			{
				inner_log("suffix wrong\n");
			}
			break;
	}
	return res;
}
void vlog_inner_log(const char *file, long line, const char *fmt,...)
{
	va_list args;
	char time_str[30];
	FILE *fp = NULL;
	
	fp = fopen("/var/log/vlog_debug.log", "a");
	get_time(time_str);

	if(fp)
	{
		fprintf(fp, "%s DEBUG (%d:%s%ld) ", time_str, getpid(), file, line);
		va_start(args, fmt);
		vfprintf(fp, fmt, args);
		va_end(args);
		fclose(fp);
	}
	
}
