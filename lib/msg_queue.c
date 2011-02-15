/*
 * Author: youngtrips
 * Created Time:  2011-02-15
 * File Name: msg_queue.c
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

#include "msg_queue.h"


msg_queue_t *create_msg_queue(mem_pool_t *pool, size_t size) {
	msg_queue_t *msgque;

	msgque = (msg_queue_t*)mem_pool_alloc(pool, sizeof(msg_queue_t));
	if (msgque == NULL)
		return NULL;

	msgque->que = (queue_t*)create_queue(pool, size);
	msgque->pool = pool;
	pthread_mutex_init(&(msgque->lock), NULL);
	pthread_cond_init(&(msgque->cond), NULL);

	return msgque;
}

void msg_queue_get(msg_queue_t *que, void **data) {
	pthread_mutex_lock(&(que->lock));
	while(queue_pop(que->que, data) < 0) {
		pthread_cond_wait(&(que->cond), &(que->lock));
	}
	pthread_mutex_unlock(&(que->lock));
}

void msg_queue_put(msg_queue_t *que, const void *data) {
	pthread_mutex_lock(&(que->lock));
	queue_push(que->que, data);
	pthread_cond_signal(&(que->cond));
	pthread_mutex_unlock(&(que->lock));
}

void destroy_msg_queue(msg_queue_t *que) {
	mem_pool_t *pool;

	pool = que->pool;
	pthread_cond_broadcast(&(que->cond));
	destroy_queue(que->que);
	mem_pool_free(pool, que);
}
