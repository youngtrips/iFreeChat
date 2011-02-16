/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-10
 * File Name: user.h
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

#ifndef __USER_H
#define __USER_H

#include "dlist.h"
#include "hash.h"
#include "mem_pool.h"

typedef struct user_entry_t {
	char *nickname;
	char *username;
	char *hostname;
	char *avatar;
	char *ipaddr;
	char *macaddr;
	char *signature;
	char *category;
	char *encoding;

	dlist_t unode; /* user list node */
}user_entry_t;

typedef struct user_t {
	dlist_t ulist;
	hash_t *hash;
	mem_pool_t *pool;
}user_t;

user_entry_t *new_user_entry(mem_pool_t *pool,
		const char *nickname, const char *username,
		const char *hostname, const char *avatar, 
		const char *ipaddr, const char *macaddr,
		const char *signature, const char *category,
		const char *encoding);
void free_user_entry(mem_pool_t *pool, user_entry_t *entry);

user_t *create_user(mem_pool_t *pool);

void destroy_user(user_t *user);

int user_add_entry(user_t *user, user_entry_t *entry);

int user_del_entry(user_t *user, user_entry_t *entry);

user_entry_t *user_find_entry(user_t *user, const char *key);

#endif
