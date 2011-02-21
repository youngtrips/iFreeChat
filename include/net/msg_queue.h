/*
 * Author: youngtrips
 * Created Time:  2011-02-15
 * File Name: msg_queue.h
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

#ifndef __MSG_QUEUE_H
#define __MSG_QUEUE_H

#include <pthread.h>

#include "mem_pool.h"
#include "queue.h"

typedef struct msg_queue_t {
	queue_t *que;
	mem_pool_t *pool;

#ifdef __USE_SIGNAL
	pthread_mutex_t lock;
	pthread_cond_t cond;
#endif

}msg_queue_t;

msg_queue_t *create_msg_queue(mem_pool_t *pool, size_t size);

void msg_queue_get(msg_queue_t *que, void **data);

void msg_queue_put(msg_queue_t *que, const void *data);

void destroy_msg_queue(msg_queue_t *que);


#endif
