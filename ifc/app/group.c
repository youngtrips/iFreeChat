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

#include "group.h"

group_t *new_group(const char *gpname, const char *gpinfo,
		uint32_t gpid) {
	size_t size;
	char *base;
	group_t *group;

	size = 0;
	size += 1 + strlen(gpname);
	size += 1 + strlen(gpinfo);
	size += sizeof(group_t);

	base = (char*)malloc(size);
	if (base == NULL) {
		return NULL;
	}

	group = (group_t*)base; base += sizeof(group_t);
	group->group_name = base; base += 1 + strlen(gpname);
	group->group_info = base; base += 1 + strlen(gpinfo);

	strcpy(group->group_name, gpname);
	strcpy(group->group_info, gpinfo);

	group->group_id = gpid;

	init_dlist_node(&(group->gnode));
	init_dlist_node(&(group->ulist));

	return group;
}

int group_add_user(group_t *gp, user_t *user) {
	if (gp == NULL || user == NULL)
		return -1;
	dlist_add_tail(&(user->gnode), &(gp->ulist));
	return 0;
}

int group_del_user(group_t *gp, user_t *user) {
	if (gp == NULL || user == NULL)
		return -1;
	dlist_del(&(user->gnode));
	return 0;
}

user_t *group_find_user(group_t *gp, const char *ip) {
	user_t *user;
	dlist_t *p;

	if (gp == NULL || user == NULL)
		return NULL;
	dlist_foreach(p, &(gp->ulist)) {
		user = (user_t*)dlist_entry(p, user_t, gnode);
		if (!strcmp(user->ipaddr, ip))
			return user;
	}

	return NULL;
}


int add_group(dlist_t *glist, group_t *gp) {
	if (glist == NULL || gp == NULL) {
		return -1;
	}

	dlist_add_tail(&(gp->gnode), glist);

	return 0;
}

int del_group(dlist_t *glist, group_t *gp) {
	if (glist == NULL || gp == NULL) {
		return -1;
	}
	dlist_del(&(gp->gnode));
	free(gp);
	return 0;
}

group_t *find_group(dlist_t *glist, uint32_t gpid) {
	dlist_t *p;
	group_t *gp;

	dlist_foreach(p, glist) {
		gp = (group_t*)dlist_entry(p, group_t, gnode);
		if (gp->group_id == gpid)
			return gp;
	}
	return NULL;
}



