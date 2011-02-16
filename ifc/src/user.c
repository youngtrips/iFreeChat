/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-10
 * File Name: user.c
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
#include <string.h>

#include "mem_pool.h"
#include "user.h"

user_entry_t *new_user_entry(mem_pool_t *pool, 
		const char *nickname, const char *username,
		const char *hostname, const char *avatar,
		const char *ipaddr, const char *macaddr,
		const char *signature, const char *category,
		const char *encoding) {
	char *base;
	user_entry_t *user;
	size_t size;

	size = 0;
	size += 1 + strlen(nickname);
	size += 1 + strlen(username);
	size += 1 + strlen(hostname);
	size += 1 + strlen(avatar);
	size += 1 + strlen(ipaddr);
	size += 1 + strlen(macaddr);
	size += 1 + strlen(signature);
	size += 1 + strlen(category);
	size += 1 + strlen(encoding);
	size += sizeof(user_entry_t);

	base = (char*)mem_pool_alloc(pool, size);
	if (base == NULL) {
		return NULL;
	}
	user = (user_entry_t*)base; 	base += sizeof(user_entry_t);
	user->nickname 	= base; base += 1 + strlen(nickname);
	user->username 	= base;	base += 1 + strlen(username);
	user->hostname 	= base; base += 1 + strlen(hostname);
	user->avatar	= base; base += 1 + strlen(avatar);
	user->ipaddr 	= base; base += 1 + strlen(ipaddr);
	user->macaddr 	= base; base += 1 + strlen(macaddr);
	user->signature = base; base += 1 + strlen(signature);
	user->category 	= base; base += 1 + strlen(category);
	user->encoding 	= base; base += 1 + strlen(encoding);


	strcpy(user->nickname, 	nickname);
	strcpy(user->username, 	username);
	strcpy(user->hostname, 	hostname);
	strcpy(user->avatar, 	avatar);
	strcpy(user->ipaddr, 	ipaddr);
	strcpy(user->macaddr, 	macaddr);
	strcpy(user->signature, signature);
	strcpy(user->category, 	category);
	strcpy(user->encoding, 	encoding);

	init_dlist_node(&(user->unode));

	return user;
}

void free_user_entry(mem_pool_t *pool, user_entry_t *entry) {
	dlist_del(&(entry->unode));
	mem_pool_free(pool, entry);
}

user_t *create_user(mem_pool_t *pool) {
	user_t *user;

	user = (user_t*)mem_pool_alloc(pool, sizeof(user_t));
	if (user == NULL)
		return NULL;

	user->pool = pool;
	user->hash = create_hash(pool, 0xffff, KEY_STR);
	init_dlist_node(&(user->ulist));

	return user;
}

void destroy_user(user_t *user) {
	user_entry_t *entry;
	mem_pool_t *pool;
	dlist_t *p;
	dlist_t *q;
	if (user != NULL) {
		pool = user->pool;
		dlist_foreach_safe(p, q, &(user->ulist)) {
			entry = (user_entry_t*)dlist_entry(p, user_entry_t, unode);
			free_user_entry(pool, entry);
		}
		destroy_hash(pool, user->hash);
		mem_pool_free(pool, user);
	}
}

int user_add_entry(user_t *user, user_entry_t *entry) {

	if (hash_insert(user->pool, user->hash, entry->ipaddr, entry) < 0)
		return -1;
	dlist_add_tail(&(entry->unode), &(user->ulist));
	return 0;
}

int user_del_entry(user_t *user, user_entry_t *entry) {

	user_entry_t *res;
	if (hash_del(user->pool, user->hash, entry->ipaddr, (void**)&res) < 0)
		return -1;
	free_user_entry(user->pool, entry);
	return 0;
}

user_entry_t *find_user(user_t *user, const char *key) {
	user_entry_t *entry;

	if (hash_find(user->hash, key, (void**)&entry) < 0)
		return NULL;
	return entry;
}
