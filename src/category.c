/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-14
 * File Name: category.c
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

#include "category.h"


category_entry_t *new_category_entry(mem_pool_t *pool, const char *name) {
	category_entry_t *entry;

	entry = (category_entry_t*)mem_pool_alloc(pool, sizeof(category_entry_t));
	if (entry == NULL)
		return NULL;
	strcpy(entry->name, name);
	entry->count = 0;
	entry->pos = NULL;
	init_dlist_node(&(entry->node));

	return entry;
}

void free_category_entry(mem_pool_t *pool, category_entry_t *entry) {
	if (entry) {
		if (entry->pos)
			mem_pool_free(pool, entry->pos);
		mem_pool_free(pool, entry);
	}
}

category_t *create_category(mem_pool_t *pool) {
	category_t *cat;

	cat = (category_t*)mem_pool_alloc(pool, sizeof(category_t));
	if (cat == NULL)
		return NULL;
	cat->hash = create_hash(pool, 0xff, KEY_STR);
	cat->pool = pool;
	init_dlist_node(&(cat->clist));
	return cat;
}

void destroy_category(category_t *cat) {
	category_entry_t *entry;
	dlist_t *p;
	dlist_t *q;
	if (cat == NULL)
		return;
	dlist_foreach_safe(p, q, &(cat->clist)) {
		entry = (category_entry_t*)dlist_entry(p, category_entry_t, node);
		free_category_entry(cat->pool, entry);
	}
	destroy_hash(cat->pool, cat->hash);
	mem_pool_free(cat->pool, cat);
}

int category_insert_entry(category_t *cat, const char *key, category_entry_t *entry) {
	hash_insert(cat->pool, cat->hash, key, (void*)entry);
	dlist_add_tail(&(entry->node), &(cat->clist));
	return 0;
}

int category_del_entry(category_t *cat, const char *key, category_entry_t *entry) {
	void *res;
	hash_del(cat->pool, cat->hash, key, &res);
	dlist_del(&(entry->node));
	return 0;
}

int category_find_entry(category_t *cat, const char *key, category_entry_t **entry) {
	return hash_find(cat->hash, key, (void**)entry);
}
