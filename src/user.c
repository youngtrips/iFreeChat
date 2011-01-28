/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: user.c
 * Description: 
 *
 */

#include <stdlib.h>
#include <string.h>

#include "user.h"

struct dlist_t user_list;

#define init_user_list(head) \
	init_dlist_node(head)

struct user_t *add_user(struct dlist_t *ulist, 
		char *nickname, char *avatar,
		char *ip, char *mac,
		char *signature, char *grpname) {
	struct user_t *user;

	user = (struct user_t*)malloc(sizeof(struct user_t));
	if (user == NULL) {
		return NULL;
	}
	strcpy(user->nickname, nickname);
	strcpy(user->avatar, avatar);
	strcpy(user->ip_addr, ip);
	strcpy(user->mac_addr, mac);
	strcpy(user->signature, signature);
	strcpy(user->group_name, grpname);

	init_dlist_node(&(user->ulist_node));
	init_dlist_node(&(user->glist_node));

	dlist_add(&(user->ulist_node), ulist);

	return (void*)user;
}



