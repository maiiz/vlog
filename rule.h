#ifndef __vlog_rule_h
#define __vlog_rule_h
#include "util.h"
#include <stdio.h>
#include <unistd.h>

typedef struct vlog_rule_s vlog_rule_t;
typedef int (*vlog_record_fn)(vlog_rule_t *rule, const char *msg);

struct vlog_rule_s
{
	char catagory[MAX_LEN];			//rule catagory
	vlog_level level;			//log level
	char archive_file[MAX_LEN];		//archive file path
	char base_path[MAX_LEN];
	long archive_max_size;			//archive file max size	
	char format_name[MAX_LEN];		//format name
	int fp;				//the pointer of the log file
	vlog_record_fn record_fn[MAX_FUNC_NUM]; //record function
	int rate_num;
	vlog_rule_t *next;
};

vlog_rule_t *vlog_rule_new(char *rule_str);
void vlog_rule_del(vlog_rule_t *rule);

#endif
