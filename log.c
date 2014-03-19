#include <unistd.h>
#include "log.h"

void vlog_work_thread();
vlog_info_t *log_init(const char *conf_path, const char *rule_name)
{
	vlog_info_t *vlog_info = (vlog_info_t*)calloc(1, sizeof(vlog_info_t));
	vlog_info->conf = vlog_conf_new(conf_path);
	vlog_info->qmsg = vlog_queue_new();
	vlog_info->rule = get_rule_byname(vlog_info->conf, rule_name);
	if(!vlog_info->rule)
	{
		inner_log("no such rule:%s\n",rule_name);
		return NULL;
	}
	vlog_info->flag = 1;
	pthread_cond_init(&vlog_info->thread_cond,NULL);
	pthread_create(&vlog_info->thread_log, NULL, (void*)vlog_work_thread, vlog_info);	
	return vlog_info;
}

void vlog_work_thread(vlog_info_t *vlog_info)
{
	char *msg = NULL;
	int i = 0;
	while(vlog_info->flag)
	{
		pthread_mutex_lock(&vlog_info->mutex_queue);
		if(vlog_queue_out(vlog_info->qmsg, &msg))
		{
			pthread_cond_wait(&vlog_info->thread_cond, &vlog_info->mutex_queue);
		}
		pthread_mutex_unlock(&vlog_info->mutex_queue);
		if(msg)
		{
			if(vlog_info->rule)
			{
				while(vlog_info->rule->record_fn[i])
				{
					vlog_info->rule->record_fn[i](vlog_info->rule, msg);
					i++;
				}
				i = 0;
			}
			free(msg);	
			msg = NULL;
		}
	}
	pthread_exit(0);
}
void vlog(vlog_info_t *vlog_info, vlog_level level,int pid, const char* file, const char *func, long line,const char *format,...)
{	
	char *msg = NULL; 
        char *buf = NULL;	
	char time_str[30];
	vlog_format_t *format_t;
	va_list args;
	if(!vlog_info)
	{
		return;
	}
	if(vlog_info->rule)
	{
		if(level < vlog_info->rule->level)
		{
			return;
		}
		format_t = get_format_byname(vlog_info->conf, vlog_info->rule->format_name);
	}
	va_start(args, format);
	msg = get_msg(format,args);
	va_end(args);
	get_time(time_str);	
	buf = (char*)calloc(strlen(msg)+MAX_LEN, sizeof(char));
	sprintf(buf, "[%s] [%s] [%d] (%s:%s:%d)-%s", time_str,keywords[level].level_name , pid, file, func, line, msg);
	pthread_mutex_lock(&vlog_info->mutex_queue);
	vlog_queue_in(vlog_info->qmsg, buf);
    	pthread_cond_broadcast(&vlog_info->thread_cond);
	pthread_mutex_unlock(&vlog_info->mutex_queue);
	if(msg)
	{
		free(msg);
	}
	if(buf)
	{
		free(buf);
	}
}

void vlog_close(vlog_info_t *vlog_info)
{
	char *msg = NULL;
	int i = 0;
	vlog_info->flag = 0;
	pthread_cond_broadcast(&vlog_info->thread_cond);
	pthread_join(vlog_info->thread_log, NULL);
	pthread_mutex_destroy(&vlog_info->mutex_queue);
	pthread_cond_destroy(&vlog_info->thread_cond);

	while(!vlog_queue_out(vlog_info->qmsg, &msg))
	{
		if(vlog_info->rule)
		{
			while(vlog_info->rule->record_fn[i])
			{
				vlog_info->rule->record_fn[i](vlog_info->rule, msg);
				i++;
			}
			i = 0;
		}
		free(msg);
		msg = NULL;
	}

	//vlog_rule_del(vlog_info->rule);
	vlog_conf_del(vlog_info->conf);
	vlog_queue_destroy(vlog_info->qmsg);
}
