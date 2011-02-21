/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-10
 * File Name: group.h
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

#ifndef __GROUP_H
#define __GROUP_H

#include <stdint.h>

#include "mem_pool.h"
#include "dlist.h"
#include "user.h"
#include "hash.h"

typedef struct group_entry_t {
	char *group_name;
	char *group_info;
	uint32_t group_id;

	void *pos;
	void *chatbox;

	dlist_t gnode;
	dlist_t ulist;
}group_entry_t;

typedef struct group_t {
	dlist_t glist;
	hash_t *hash;
	mem_pool_t *pool;
}group_t;

typedef struct gpmember_t {
	user_entry_t *user;
	dlist_t node;
}gpmember_t;

group_entry_t *new_group_entry(mem_pool_t *pool, 
		const char *gpname, const char *gpinfo,
		uint32_t gpid);

void free_group_entry(mem_pool_t *pool, group_entry_t *entry);

group_t *create_group(mem_pool_t *pool);

void destroy_group(group_t *gp);

int group_add_entry(group_t *gp, group_entry_t *entry);

int group_del_entry(group_t *gp, group_entry_t *entry);

group_entry_t *group_find_entry(group_t *gp, uint32_t gpid);

int group_entry_add_user(mem_pool_t *pool, group_entry_t *gp_entry, 
		user_entry_t *user_entry);

int group_entry_del_user(mem_pool_t *pool, group_entry_t *gp_entry, 
		user_entry_t *user_entry);

user_entry_t *group_entry_find_user(group_entry_t *gp_entry, const char *ip);

#endif
