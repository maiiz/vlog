#include <stdlib.h>
#include "conf.h"
#include "rule.h"
#include <sys/stat.h>

vlog_conf_t *vlog_conf_new(const char *confpath)
{
	vlog_conf_t *conf = NULL;
	conf = calloc(1, sizeof(vlog_conf_t));
	if(!conf)
	{
	   	inner_log("calloc fail, errno[%d]", errno);
		return NULL;
	}
  	if(confpath && confpath[0] != '\0')
	{
		snprintf(conf->conf_file, sizeof(conf->conf_file), "%s", confpath);
	}	
	
	load_conf(conf);
	return conf;
}
void parse_conf_line(vlog_conf_t *conf, char *line, int *section)
{
	char name[MAX_LEN];
	char value[MAX_LEN];
	vlog_rule_t *rule;
	if(*line == '[')
	{
		sscanf(line, "[%[^] \t]", name);
		if(!strcasecmp(name, "rules"))
		{
			*section = 3;			
		}
		else if(!strcasecmp(name, "formats"))
		{
			*section = 2;
		}
		else if(!strcasecmp(name, "global")){	
			*section = 1;
		}
		return;
	}	
	switch(*section)
	{
		case 1:			
		case 2:
			sscanf(line, "%1000[^\t]%1000s", name, value); //set the length limit,so we use strcpy
		
			if(conf->formats)
			{
				conf->formats->next = vlog_format_new(name, value);	
			}
			else
			{
				conf->formats = vlog_format_new(name, value);
			}
			break;
		case 3:
			if(conf->rules)
			{
				rule = conf->rules;
				while(rule->next != NULL)
				{
					rule = rule->next;
				}
				rule->next = vlog_rule_new(line);
			}
			else
			{
				conf->rules = vlog_rule_new(line);
			}
			break;
		default:
			inner_log("parse_conf_line error, config file format error!line %d-%s\n", *section, line);
	}
}

vlog_rule_t *get_rule_byname(vlog_conf_t *conf, const char *name)
{
	vlog_rule_t *rule = conf->rules;
	while(rule)
	{
		if(!strcasecmp(name, rule->catagory))
		{
			break;
		}
		rule = rule->next;
	}
	return rule;
}
vlog_format_t *get_format_byname(vlog_conf_t *conf, const char *name)
{
	vlog_format_t *format = conf->formats;
	while(format)
	{
		if(!strcasecmp(name, format->name))
		{
			break;
		}
		format = format->next;
	}
	return format;
}
int load_conf(vlog_conf_t *conf)
{
	FILE *fp = NULL;
	struct stat a_stat;
	char line[MAX_CFG_LINE+1];
	char *pline;
	size_t line_len;
	int section = 0;
	if(lstat(conf->conf_file, &a_stat))
	{
		inner_log("lstat conf file[%s] fail, errno[%d]", conf->conf_file, errno);
		return -1;
	}
	if((fp = fopen(conf->conf_file, "r")) == NULL)
	{
		inner_log("open configure file [%s] fail", conf->conf_file);
		return -1;
	}
	pline = line;
	memset(&line, 0x00, sizeof(line));
	fgets(pline, sizeof(line), fp);
	while(fgets((char*)pline, sizeof(line), fp) != NULL)
	{
		line_len = strlen(pline);
		if(pline[line_len - 1] == '\n')
		{
			pline [line_len - 1 ] = '\0';
		}
		trim(pline);	
		if(*pline == '\0' || *pline=='#')
		{
			continue;	
		}
		parse_conf_line(conf, pline, &section);
	}
	return 0;
}

int reload_conf(const char *confpath)
{
	return 0;
}

void vlog_conf_del(vlog_conf_t *conf)
{
	if(conf)
	{
		if(conf->formats)
		{
			vlog_format_del(conf->formats);
		}
		if(conf->rules)
		{
			vlog_rule_del(conf->rules);
		}	
		free(conf);
	}
	
}
