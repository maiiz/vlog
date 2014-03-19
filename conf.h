#ifndef __vlog_conf_h
#define __vlog_conf_h

#include <stdio.h>
#include "util.h"
#include "rule.h"
#include "format.h"

#define DEFAULT_CONF_DIR "/etc/vlog.conf"

#define DEFAULT_ARCHIVE_MAX_SIZE 20M

typedef struct vlog_conf_s
{
	char conf_file[MAX_LEN];
	
	size_t reload_conf_peroid;

	vlog_format_t *formats;	
 		
	vlog_rule_t *rules;
}vlog_conf_t;

vlog_conf_t *vlog_conf_new(const char *confpath);
int load_conf(vlog_conf_t *conf);
int reload_conf(const char *confpath);
void vlog_conf_del(vlog_conf_t *conf);

vlog_rule_t *get_rule_byname(vlog_conf_t *conf, const char *name);
vlog_format_t *get_format_byname(vlog_conf_t *conf, const char *name);
#endif
