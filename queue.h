#ifndef __vlog_queue_h
#define __vlog_queue_h

typedef struct vlog_queue_node_s
{
	char *buf;
	struct vlog_queue_node_s *next;
}vlog_queue_node_t;

typedef struct vlog_queue_s
{
	vlog_queue_node_t *head, *tail;
}vlog_queue_t;

vlog_queue_node_t *vlog_queue_node_new(const char *str);
int vlog_queue_node_del(vlog_queue_node_t *node);

vlog_queue_t* vlog_queue_new();
int vlog_queue_in(vlog_queue_t *q, const char* msg);
int vlog_queue_out(vlog_queue_t *q, char **msg);
int vlog_queue_destroy(vlog_queue_t *q);

#endif
