/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-10
 * File Name: group.c
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
#include "group.h"

group_entry_t *new_group_entry(mem_pool_t *pool,
		const char *gpname, const char *gpinfo,
		uint32_t gpid) {
	size_t size;
	char *base;
	group_entry_t *group;

	size = 0;
	size += 1 + strlen(gpname);
	size += 1 + strlen(gpinfo);
	size += sizeof(group_entry_t);

	base = (char*)mem_pool_alloc(pool, size);
	if (base == NULL) {
		return NULL;
	}

	group = (group_entry_t*)base; base += sizeof(group_t);
	group->group_name = base; base += 1 + strlen(gpname);
	group->group_info = base; base += 1 + strlen(gpinfo);

	strcpy(group->group_name, gpname);
	strcpy(group->group_info, gpinfo);

	group->group_id = gpid;

	init_dlist_node(&(group->gnode));
	init_dlist_node(&(group->ulist));

	return group;
}

void free_group_entry(mem_pool_t *pool, group_entry_t *entry) {
	gpmember_t *gpmemb;
	dlist_t *p;
	dlist_t *q;
	dlist_foreach_safe(p, q, &(entry->ulist)) {
		gpmemb = (gpmember_t*)dlist_entry(p, gpmember_t, node);
		dlist_del(&(gpmemb->node));
		mem_pool_free(pool, gpmemb);
	}
	dlist_del(&(entry->gnode));
	mem_pool_free(pool, entry);
}

group_t *create_group(mem_pool_t *pool) {
	group_t *gp;

	gp = (group_t*)mem_pool_alloc(pool, sizeof(group_t));
	if (gp == NULL)
		return NULL;
	gp->pool = pool;
	gp->hash = create_hash(pool, 0xffff, KEY_INT);
	init_dlist_node(&(gp->glist));

	return gp;
}

void destroy_group(group_t *gp) {
	group_entry_t *entry;
	mem_pool_t *pool;
	dlist_t *p;
	dlist_t *q;

	if (gp != NULL) {
		pool = gp->pool;
		dlist_foreach_safe(p, q, &(gp->glist)) {
			entry = (group_entry_t*)dlist_entry(p, group_entry_t, gnode);
			free_group_entry(pool, entry);
		}
		destroy_hash(pool, gp->hash);
		mem_pool_free(pool, gp);
	}
}

int group_add_entry(group_t *gp, group_entry_t *entry) {
	if (hash_insert(gp->pool, gp->hash, &(entry->group_id), (void*)entry) < 0)
		return -1;
	dlist_add_tail(&(entry->gnode), &(gp->glist));
	return 0;
}

int group_del_entry(group_t *gp, group_entry_t *entry) {
	void *res;
	if (hash_del(gp->pool, gp->hash, &(entry->group_id), &res) < 0)
	   return -1;	
	free_group_entry(gp->pool, entry);
	return 0;
}

group_entry_t *group_find_entry(group_t *gp, uint32_t gpid) {
	group_entry_t *entry;
	if (hash_find(gp->hash, &gpid, (void**)&entry) < 0)
		return NULL;
	return entry;
}

int group_entry_add_user(mem_pool_t *pool, group_entry_t *gp_entry, 
		user_entry_t *user_entry) {
	gpmember_t *gpmemb;
	user_entry_t *u;
	dlist_t *p;

	dlist_foreach(p, &(gp_entry->ulist)) {
		gpmemb = (gpmember_t*)dlist_entry(p, gpmember_t, node);
		u = gpmemb->user;
		if (!strcmp(u->ipaddr, user_entry->ipaddr))
			return -1;
	}

	gpmemb = (gpmember_t*)mem_pool_alloc(pool, sizeof(gpmember_t));
	gpmemb->user = user_entry;
	init_dlist_node(&(gpmemb->node));
	dlist_add_tail(&(gpmemb->node), &(gp_entry->ulist));
	return 0;
}

int group_entry_del_user(mem_pool_t *pool, group_entry_t *gp_entry, 
		user_entry_t *user_entry) {
	gpmember_t *gpmemb;
	user_entry_t *u;
	dlist_t *p;
	dlist_t *q;

	dlist_foreach_safe(p, q, &(gp_entry->ulist)) {
		gpmemb = (gpmember_t*)dlist_entry(p, gpmember_t, node);
		u = gpmemb->user;
		if (!strcmp(u->ipaddr, user_entry->ipaddr)) {
			dlist_del(&(gpmemb->node));
			mem_pool_free(pool, gpmemb);
			return 0;
		}
	}

	return -1;
}

user_entry_t *group_entry_find_user(group_entry_t *gp_entry, const char *ip) {
	gpmember_t *gpmemb;
	user_entry_t *u;
	dlist_t *p;

	dlist_foreach(p, &(gp_entry->ulist)) {
		gpmemb = (gpmember_t*)dlist_entry(p, gpmember_t, node);
		u = gpmemb->user;
		if (!strcmp(u->ipaddr, ip))
			return u; 
	}
	return NULL;
}

