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

struct user_t {
	char nickname[32];
	char avatar[64];
	char ip_addr[32];
	char mac_addr[32];
	char signature[128];
	char group_name[32];

	GtkTreeIter parent_iter;
	GtkTreeIter my_iter;
	GtkWidget *chatdlg;

	struct dlist_t ulist_node;
	struct dlist_t glist_node;
};


#endif
