/*
 * Author: youngtrips
 * Created Time:  2011-02-15
 * File Name: queue.c
 * Description: 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <stdlib.h>

#include "queue.h"

queue_t *create_queue(mem_pool_t *pool, size_t size) {

	size_t tot_size;
	char *base;
	queue_t *q;

	tot_size = sizeof(queue_t) + sizeof(void*) * size;
	base = (char*)mem_pool_alloc(pool, tot_size);

	if (base == NULL) {
		return NULL;
	}

	q = (queue_t*)base; base += sizeof(queue_t);
	q->entry = (void**)base;

	q->pool = pool;
	q->size = size;
	q->head = 0;
	q->tail = 0;

	return q;
}


int queue_push(queue_t *q, const void *data) {
	if ((q->tail + 1) % q->size == q->head) 
		return -1;
	q->entry[q->tail] = (void*)data;
	q->tail = (q->tail + 1) % q->size;
	return 0;
}

int queue_pop(queue_t *q, void **data) {
	if (q->head == q->tail)
		return -1;
	*data = q->entry[q->head];
	q->head = (q->head + 1) % q->size;
	return 0;
}

void destroy_queue(queue_t *q) {
	mem_pool_t *pool;
	if (q) {
		pool = q->pool;
		mem_pool_free(pool, q);
	}
}
