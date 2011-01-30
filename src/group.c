/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: group.c
 * Description: 
 *
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "gtk_common.h"

#include "dlist.h"
#include "user.h"
#include "group.h"

#include "main_window.h"
#include "ifreechat.h"


void init_group(struct dlist_t *glist) {
	struct group_t *group;
	init_dlist_node(glist);
	group = (struct group_t*)malloc(sizeof(struct group_t));
	if (group == NULL) {
		fprintf(stderr, "malloc() error: %s\n", strerror(errno));
		return;
	}
	strcpy(group->group_name, "No Group");
	init_dlist_node(&(group->glist_node));
	init_dlist_node(&(group->mlist_head));

	dlist_add(&(group->glist_node), glist);
}

void add_group(struct dlist_t *glist, struct group_t *group) {
}

void group_add_user(struct ifreechat_t *ifc, 
		struct dlist_t *glist, struct user_t *user) {
	struct dlist_t *pos;
	struct group_t *group;
	struct window_t *win = (struct window_t*)(ifc->win);
	char title[64];
	GtkTreeView *tv = (GtkTreeView*)(win->contact_treeview);
	GtkTreeIter parent;
	GtkTreeIter child;
	GtkTreeStore *store;
	GtkWidget *cell_view;
	GdkPixbuf *pixbuf;

	store = (GtkTreeStore*)gtk_tree_view_get_model(GTK_TREE_VIEW(tv));

	user->ifreechat = (void*)ifc;

	dlist_foreach(pos, glist) {
		group = (struct group_t*)dlist_entry(pos, struct group_t, glist_node);
		if (!strcmp(group->group_name, user->group_name)) {
			dlist_add(&(user->glist_node), &(group->mlist_head));

			/* update treeview model */
			parent = group->my_iter;
			group->num_member++;
			sprintf(title, "%s[%d]", group->group_name, group->num_member);
//			pixbuf = gdk_pixbuf_new_from_file("pixmaps/online.png", NULL);
			gtk_tree_store_set(store, &parent, 0, NULL, 1, title, -1);
//			gdk_pixbuf_unref(pixbuf);

			pixbuf = gdk_pixbuf_new_from_file(user->avatar, NULL);
			gtk_tree_store_append(store, &child, &parent);
			gtk_tree_store_set(store, &child, 
					0, pixbuf, 
					1, user->nickname, 
					2, user->ip_addr,
					3, user->mac_addr,
					4, (void*)user,
					-1);
			gdk_pixbuf_unref(pixbuf);
			user->parent_iter = group->my_iter; 
			user->my_iter = child;
			return;
		}
	}

	group = (struct group_t*)malloc(sizeof(struct group_t));
	strcpy(group->group_name, user->group_name);
	init_dlist_node(&(group->mlist_head));
	init_dlist_node(&(group->glist_node));

	dlist_add(&(group->glist_node), glist);

	printf("update treeview model\n");
	/* update treeview model */

	sprintf(title, "%s[1]", group->group_name);
//	pixbuf = gdk_pixbuf_new_from_file("pixmaps/online.png", NULL);
	gtk_tree_store_append(store, &parent, NULL);
	gtk_tree_store_set(store, &parent, 0, NULL, 1, title, -1);
//	gdk_pixbuf_unref(pixbuf);
	group->my_iter = parent;
	group->num_member = 1;

	pixbuf = gdk_pixbuf_new_from_file(user->avatar, NULL);
	gtk_tree_store_append(store, &child, &parent);
	gtk_tree_store_set(store, &child, 
			0, pixbuf, 
			1, user->nickname,
			2, user->ip_addr,
			3, user->mac_addr,
			4, (void*)user,
		   	-1);
	gdk_pixbuf_unref(pixbuf);
	user->parent_iter = parent;
	user->my_iter = child;
}

struct user_t *group_find_user(struct dlist_t *glist, int type, const char *key) {
	struct group_t *group;
	struct user_t *user;
	struct dlist_t *i;
	struct dlist_t *j;
	char *p;

	dlist_foreach(i, glist) {
		group = (struct group_t*)dlist_entry(i, struct group_t, glist_node);
		
		dlist_foreach(j, &(group->mlist_head)) {
			user = (struct user_t*)dlist_entry(j, struct user_t, glist_node);
			switch(type) {
				case SEARCH_BY_NAME:
					p = user->nickname; break;
				case SEARCH_BY_IPV4:
					p = user->ip_addr; 	break;
				case SEARCH_BY_MAC:
					p = user->mac_addr;	break;
			}
			if (!strcmp(p, key)) {
				return user;
			}
		}
	}
	return NULL;
}

















