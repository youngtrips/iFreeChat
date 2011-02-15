/*
 * Author: youngtrips
 * Created Time:  2011-02-15
 * File Name: queue.h
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

#ifndef __QUEUE_H
#define __QUEUE_H

#include "mem_pool.h"

typedef struct queue_t {
	void **entry;
	mem_pool_t *pool;

	size_t size;
	size_t head;
	size_t tail;
}queue_t;

queue_t *create_queue(mem_pool_t *pool, size_t size);

int queue_emtpy(queue_t *q);

int queue_push(queue_t *q, const void *data);

int queue_pop(queue_t *q, void **data);

void destroy_queue(queue_t *q);

#endif
