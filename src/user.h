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

typedef struct user_t {
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
	dlist_t gnode; /* group list node */
}user_t;



user_t *new_user(const char *nickname, const char *username,
		const char *hostname, const char *avatar, 
		const char *ipaddr, const char *macaddr,
		const char *signature, const char *category,
		const char *encoding);

int add_user(dlist_t *ulist, user_t *user);

int del_user(dlist_t *ulist, user_t *user);

user_t *find_user(dlist_t *ulist, const char *ip);

#endif
