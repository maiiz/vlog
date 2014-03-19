#include "rule.h"
#include <fcntl.h>

void vlog_rule_del(vlog_rule_t *rule)
{
	if(rule)
	{	
		if(rule->fp)
		{
			close(rule->fp);
		}
		if(rule->next)
		{
			vlog_rule_del(rule->next);
		}
		free(rule);
	}
}
void size_check(vlog_rule_t *rule)
{
	struct stat info;
	while(1)
	{
		if(stat(rule->archive_file, &info))
		{
			break;
		}
		if(info.st_size >= rule->archive_max_size)
		{
			rule->rate_num ++;
			
			sprintf(rule->archive_file, "%s_%d", rule->base_path, rule->rate_num);

			rule->fp = open(rule->archive_file,  O_WRONLY | O_APPEND | O_CREAT);
			continue;
		}
		break;
	}

}
static int vlog_rule_output_file(vlog_rule_t *rule, const char *msg)
{
	write(rule->fp, msg, strlen(msg));
	size_check(rule);
}
static int vlog_rule_output_stdout(vlog_rule_t *rule, const char *msg)
{
	write(STDOUT_FILENO, msg, strlen(msg));
}
static int vlog_rule_output_stderr(vlog_rule_t *rule, const char *msg)
{
	write(STDERR_FILENO, msg, strlen(msg));
}
vlog_rule_t *vlog_rule_new(char *rule_str)
{
	vlog_rule_t *a_rule;
	int nscan;
	char selector[MAX_LEN], action[MAX_LEN];
	char catagory[MAX_LEN], level[MAX_LEN];
	char output[MAX_LEN], format_name[MAX_LEN];
	char temp[MAX_LEN];
	char file_path[MAX_LEN];
	char *del = "|";
	char *p;
	char *file_limit;
	int i = 0;
	a_rule = calloc(1, sizeof(vlog_rule_t));
	nscan = sscanf(rule_str, "%1000[^ \t]%1000s", selector, action);
	if(nscan !=2)
	{
		inner_log("%d%s[%s]", nscan, "sscanf selector error",rule_str);
		free(a_rule);
		return NULL;
	}
	nscan = sscanf(selector, "%[^.].%s", catagory, level);
	if(nscan !=2)
	{
		inner_log("sscanf catatory error");
		free(a_rule);
		return NULL;
	}
	strcpy(a_rule->catagory, catagory);
	a_rule->level = get_level(level);
	
	nscan = sscanf(action, "%[^;];%s", output, format_name);
	if(nscan<1)
	{
		inner_log("sscanf %s error", action);
		free(a_rule);
		return NULL;
	}
	strcpy(a_rule->format_name, format_name);
	
	p = strtok(output, del);
	while(p!=NULL)
	{	
		if(p[0]=='>')
		{
			if(!strcasecmp(p, ">stdout"))
			{
				a_rule->record_fn[i] = vlog_rule_output_stdout;
				i++;
			}
			else if(!strcasecmp(p, ">stderr"))
			{
				a_rule->record_fn[i] = vlog_rule_output_stderr;
				i++;
			}
		}
				
		if(p[0] == '"')
		{
			memset(temp, 0x00, sizeof(temp));
			memcpy(temp, p, strlen(p));
			file_limit = strchr(temp, ',');
			if(file_limit)
			{
				file_limit++;
				trim(file_limit);
			}
			a_rule->archive_max_size = parse_byte_size(file_limit);			
			strncpy(a_rule->archive_file,p+1 ,strlen(p)-strlen(file_limit)-3);
			strcpy(a_rule->base_path, a_rule->archive_file);
			a_rule->fp = open(a_rule->archive_file,  O_WRONLY | O_APPEND | O_CREAT);
			if(!a_rule->fp)
			{
				inner_log("create/open log file error");
			}
			size_check(a_rule);
			a_rule->record_fn[i] = vlog_rule_output_file;
			i++;
		}
		p = strtok(NULL, del);	
			
	}
	a_rule->next = NULL;
	return a_rule;
}
