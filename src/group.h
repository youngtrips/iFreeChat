/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: group.h
 * Description: 
 *
 */

#ifndef __GROUP_H
#define __GROUP_H

#include "dlist.h"
#include "user.h"

struct group_t {
	char group_name[32];

	struct dlist_t mlist_head;
	struct dlist_t glist_node;

	void *my_iter;
};


enum {
	SEARCH_BY_NAME,
	SEARCH_BY_IPV4,
	SEARCH_BY_MAC
};


void init_group(struct dlist_t *glist);

void group_add_user(struct dlist_t *glist, struct user_t *user);

struct user_t *group_find_user(struct dlist_t *glist, int type, const char *key);
	
#endif
