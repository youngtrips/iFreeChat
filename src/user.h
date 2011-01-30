/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: user.h
 * Description: 
 *
 */

#ifndef __USER_H
#define __USER_H

#include "dlist.h"
#include "gtk_common.h"

enum {
	AVATAR_DEFAULT,
	AVATAR_CUSTOM
};

struct user_t {
	void *chatdlg;
	void *ifreechat;

	char nickname[64];
	char avatar[128];
	char ip_addr[32];
	char mac_addr[32];
	char signature[128];
	char group_name[64];

	GtkTreeIter parent_iter;
	GtkTreeIter my_iter;

	struct dlist_t ulist_node;
	struct dlist_t glist_node;
};


#endif
