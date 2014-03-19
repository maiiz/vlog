#include "queue.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
vlog_queue_node_t *vlog_queue_node_new(const char *msg)
{
	vlog_queue_node_t *n = (vlog_queue_node_t*)calloc(1, sizeof(vlog_queue_node_t));
	n->buf = (char*)calloc(1, strlen(msg)+1);
	strncpy(n->buf,msg , strlen(msg));
	n->next = NULL;
	return n;
}

int vlog_queue_node_del(vlog_queue_node_t *n)
{
	if(n)
	{
		//free(n->buf);
		free(n);
		n = NULL;
	}
}

vlog_queue_t* vlog_queue_new()
{
	vlog_queue_t *q = (vlog_queue_t*)calloc(1, sizeof(vlog_queue_t));
	q->head = q->tail = NULL;	
	return q;
}

int vlog_queue_in(vlog_queue_t *q, const char *msg)
{	
	if(msg&&q) 
	{		
		vlog_queue_node_t *n = vlog_queue_node_new(msg);		
		if(q->head == NULL)
		{	
			q->head = q->tail = n; 	
		}
		else
		{		
			q->tail->next = n;
			q->tail = n;
		}
		return 0;
	}
	return -1;
}

int vlog_queue_out(vlog_queue_t *q, char **msg)
{
	if(q->head != NULL && q)
	{
		vlog_queue_node_t *n;
		n = q->head;
		
		q->head = q->head->next;
		//*msg = strdup(n->buf); //unsafe so we should free the msg after use
		*msg = n->buf;
		vlog_queue_node_del(n);
		return 0;
	}
	return -1;
}

int vlog_queue_destroy(vlog_queue_t *q)
{
	char *msg;
	while(q)
	{	
		if(!vlog_queue_out(q,&msg))
		{
			free(msg);
		}		
		if(q->head == NULL)
		{
			free(q);
			return 0;
		}
	}
	return -1;
}
