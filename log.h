#ifndef __vlog_log_h
#define __vlog_log_h
#include "conf.h"
#include "queue.h"
#include "rule.h"
#include <pthread.h>

#define log(vlog_info, level, fmt,...) vlog(vlog_info, level, getpid(), __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define logt(vlog_info, level, tid, fmt,...) vlog(vlog_info, level, (tid), __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_init_default(conf_path) log_init(conf_path, "default")
#define log_close(vlog_info) vlog_close(vlog_info)

typedef struct vlog_info_S
{
	pthread_mutex_t mutex_queue;
	pthread_t thread_log;
	pthread_cond_t thread_cond;

	vlog_queue_t *qmsg;
	vlog_conf_t *conf;
	vlog_rule_t *rule;
	volatile int flag;
}vlog_info_t;

typedef vlog_info_t log_info_t;

vlog_info_t *log_init(const char *confpath, const char *rule_name);
void vlog(vlog_info_t *vlog_info, vlog_level level,int pid, const char* file, const char *func,long line,const char *format,...);
void vlog_close(vlog_info_t *vlog_info);

#endif
