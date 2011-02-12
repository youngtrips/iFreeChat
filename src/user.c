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

#include "user.h"

user_t *new_user(const char *nickname, const char *username,
		const char *hostname, const char *avatar,
		const char *ipaddr, const char *macaddr,
		const char *signature, const char *category,
		const char *encoding) {
	char *base;
	user_t *user;
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
	size += sizeof(user_t);

	base = (char*)malloc(size);
	if (base == NULL) {
		return NULL;
	}
	user = (user_t*)base; 	base += sizeof(user_t);
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
	init_dlist_node(&(user->gnode));

	return user;
}

int add_user(dlist_t *ulist, user_t *user) {

	if (ulist == NULL || user == NULL)
		return -1;
	dlist_add_tail(&(user->unode), ulist);
	return 0;
}

int del_user(dlist_t *ulist, user_t *user) {
	if (ulist == NULL || user == NULL)
		return -1;

	dlist_del(&(user->unode));
	free(user);
	return 0;
}

user_t *find_user(dlist_t *ulist, const char *ip) {
	user_t *user;
	dlist_t *p;

	if (ulist == NULL || ip == NULL)
		return NULL;

	dlist_foreach(p, ulist) {
		user = (user_t*)dlist_entry(p, user_t, unode);
		if (!strcmp(user->ipaddr, ip))
			return user;
	}
	return NULL;
}
