/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-15
 * File Name: hash.h
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

#ifndef __HASH_H
#define __HASH_H

#ifdef USE_LOCK
#include <pthread.h>
#endif
#include "mem_pool.h"
#include "dlist.h"

typedef struct hash_entry_t {
	void *key;
	void *val;

	dlist_t node;
}hash_entry_t;

enum {
	KEY_INT,
	KEY_STR
};


typedef struct hash_t {
	hash_entry_t **hash_entry;
	size_t hash_size;
	int ktype;
#ifdef USE_LOCK
	pthread_mutex_t lock;
#endif
}hash_t;

hash_t *create_hash(mem_pool_t *pool, size_t size, int ktype);

int hash_insert(mem_pool_t *pool, hash_t *h, const void *key, const void *val);

int hash_del(hash_t *h, const void *key, void **val);

int hash_find(hash_t *h, const void *key, void **val);

void destroy_hash(mem_pool_t *pool, hash_t *h);

#endif
