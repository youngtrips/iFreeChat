/*
 * Author: youngtrips
 * Created Time:  2011-02-04
 * File Name: mem_pool.h
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

#ifndef __MEM_POOL_H
#define __MEM_POOL_H

#include <pthread.h>

typedef struct mem_chunk_t {
	void *start;
	size_t free;
	size_t bound;

	struct mem_chunk_t *next;
}mem_chunk_t;

typedef struct mem_pool_t {
	size_t size;
	size_t quantum;
	mem_chunk_t *chunk;

	pthread_mutex_t *mutex;
}mem_pool_t;

mem_pool_t *mem_pool_init(size_t size, size_t quantum);

void mem_pool_destroy(mem_pool_t* p);

void *mem_pool_alloc(mem_pool_t *pool, size_t size);

void mem_pool_free(mem_pool_t *pool, void *addr);
	
	
#endif	
