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

#include "dlist.h"
#include "user.h"
#include "hash.h"

typedef struct group_node_t {
	char *group_name;
	char *group_info;
	uint32_t group_id;

	dlist_t gnode;
	dlist_t ulist;
}group_node_t;

typedef struct group_t {
	dlist_t glist;
	hash_t *hash;
}group_t;

typedef struct gpmember_t {
	void *data;
	dlist_t node;
}gpmember_t;

group_t *new_group(const char *gpname, const char *gpinfo,
		uint32_t gpid);

int add_group(dlist_t *glist, group_t *gp);

int del_group(dlist_t *glist, group_t *gp);

group_t *find_group(dlist_t *glist, uint32_t gpid);


int group_add_user(group_t *gp, user_t *user);

int group_del_user(group_t *gp, user_t *user);

user_t *group_find_user(group_t *gp, const char *ip);



#endif
