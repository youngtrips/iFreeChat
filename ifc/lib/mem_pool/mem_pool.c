/*
 * Author: youngtrips
 * Created Time:  2011-02-04
 * File Name: mem_pool.c
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
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "mem_pool.h"

struct align_test {
	void *foo;
	int64_t bar;
};

#define MINALIGN offsetof(struct align_test, bar)
#define CHUNK_DEFAULT_SIZE (32 * 1024)


mem_pool_t *mem_pool_init(size_t size, size_t quantum)
{
	mem_pool_t *pool;

	pool = (mem_pool_t*)malloc(sizeof(mem_pool_t));
	if (pool == NULL) {
		fprintf(stderr, "malloc() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		return NULL;
	}

	pool->size = size ? (size + MINALIGN - 1) & ~(MINALIGN - 1) :
		CHUNK_DEFAULT_SIZE;
	pool->quantum = quantum ? quantum : MINALIGN;
	pool->chunk = NULL;
	pool->mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	
	pthread_mutex_init(pool->mutex, NULL);
	
	return pool;
}

void mem_pool_destroy(mem_pool_t* p) {
	mem_chunk_t *next;
	mem_chunk_t *cur;

	pthread_mutex_lock(p->mutex);
	for(cur = p->chunk;cur; cur = next) {
		next = cur->next;
		free(cur->start);
		free(cur);
	}
	pthread_mutex_unlock(p->mutex);
	pthread_mutex_destroy(p->mutex);
	free(p->mutex);
	free(p);
}

void *mem_pool_alloc(mem_pool_t *pool, size_t size) {
	mem_chunk_t *chunk;
	void *start;
	
	if (pool == NULL) {
		return NULL;
	}

	if (!size) {
		size = pool->quantum;
	}
	size += sizeof(size_t);
	
	pthread_mutex_lock(pool->mutex);
	if (size > pool->size) {
		fprintf(stderr, "require too much memory...\n");
		return NULL;
	}
	if (pool->chunk == NULL || size > pool->chunk->free) {
		chunk = (mem_chunk_t*)malloc(sizeof(mem_chunk_t));
		if (chunk == NULL) {
			fprintf(stderr, "malloc() error: %s@%s:%d\n", 
					strerror(errno), __FILE__, __LINE__);
			return NULL;
		}

		start = malloc(pool->size);
		if (start == NULL) {
			fprintf(stderr, "cannot alloc memory for memory chuncks: %s@%s:%d\n",
					strerror(errno), __FILE__, __LINE__);
		}
		chunk->start = start;
		chunk->free = pool->size;
		chunk->bound = 0;
		chunk->next = NULL;
		
		pool->chunk = chunk;
	}

	pool->chunk->free -= size;
	start = (void*)(pool->chunk->start + pool->chunk->free);
	pthread_mutex_unlock(pool->mutex);

	*((size_t*)start) = size;

	return start + sizeof(size_t);
}

void mem_pool_free(mem_pool_t *pool,  void *start) {
	mem_chunk_t *prev;
	mem_chunk_t *cur;
	void *addr;
	size_t size;

	if (!pool) {
		return;
	}
	addr = start - sizeof(size_t);

	size = *((size_t*)start);

	if (!size) {
		size = pool->quantum;
	} else if (pool->quantum > 1 && size % pool->quantum) {
		size += pool->quantum - size % pool->quantum;
	}

	pthread_mutex_lock(pool->mutex);
	for(prev = NULL,cur = pool->chunk;cur; prev = cur, cur = cur->next) {
		if (addr >= cur->start && addr < cur->start + pool->size)
			break;
	}
	if (!cur)
		return;
	if (!prev) {
		if (cur->free + cur->bound + size >= pool->size) {
			cur->free = pool->size;
			cur->bound = 0;
		} else if (addr == cur->start + cur->free) {
			cur->free += size;
		} else {
			cur->bound += size;
		}
	} else {
		cur->bound += size;
		if (cur->free + cur->bound >= pool->size) {
			prev->next = cur->next;
			free(cur->start);
			free(cur);
		} else if (prev != pool->chunk) {
			prev->next = cur->next;
			cur->next = pool->chunk->next;
			pool->chunk->next = cur;
		}
	}
	pthread_mutex_unlock(pool->mutex);
}


