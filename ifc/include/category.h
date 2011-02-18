/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-14
 * File Name: category.h
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

#ifndef __CATEGORY_H
#define __CATEGORY_H

#include "mem_pool.h"
#include "hash.h"
#include "dlist.h"

typedef struct category_entry_t {
	char name[128];
	int count;
	dlist_t node;
	void *pos;

	dlist_t centry;
}category_entry_t;


typedef struct category_t {
	dlist_t clist;
	hash_t *hash;

	mem_pool_t *pool;
}category_t;


category_entry_t *new_catgory_entry(mem_pool_t *pool, const char *name);

void free_catgory_entry(mem_pool_t *pool, category_entry_t *entry);

category_t *create_category(mem_pool_t *pool);

void destroy_category(category_t *cat);

int category_insert_entry(category_t *cat, const char *key, category_entry_t *entry);

int category_del_entry(category_t *cat, const char *key, category_entry_t *entry);

int category_find_entry(category_t *cat, const char *key, category_entry_t **entry);

#endif



