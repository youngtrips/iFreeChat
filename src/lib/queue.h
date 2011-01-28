/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: queue.h
 * Description: 
 *
 */

#ifndef __QUEUE_H
#define __QUEUE_H

#define MAXN_QUEUE_SIZE	65535

#include <stdlib.h>

struct queue_t {
	void **data;
	size_t head;
	size_t tail;
	size_t size;
};

struct queue_t *queue_init(size_t size);

void queue_destroy(struct queue_t *q);

int queue_push(struct queue_t *q, const void *data);

void *queue_pop(struct queue_t *q);


#endif
