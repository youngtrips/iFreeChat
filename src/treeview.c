/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-12
 * File Name: treeview.c
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

#include "gtk_common.h"
#include "ifreechat.h"
#include "user.h"
#include "pair.h"
#include "category.h"

enum {
	PIXBUF_COL,
	TEXT_COL,
	IP_COL,
	MAC_COL,
	URI_COL,
	COL_NUM
};

void init_treeview(GtkTreeView *tv) {

	GtkCellRenderer *renderer;
	GtkTreeViewColumn *col;
	
	renderer = gtk_cell_renderer_pixbuf_new();
	col = gtk_tree_view_column_new_with_attributes("", renderer,
			"pixbuf", PIXBUF_COL, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tv), col);

	renderer = gtk_cell_renderer_text_new();
	col = gtk_tree_view_column_new_with_attributes("", renderer,
			"text", TEXT_COL, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tv), col);
}

GtkTreeStore *create_contact_treevie_model() {
	GtkTreeStore *store;


	store = gtk_tree_store_new(COL_NUM, 
			GDK_TYPE_PIXBUF, 
			G_TYPE_STRING,
			G_TYPE_STRING,
			G_TYPE_STRING,
			G_TYPE_POINTER);
	return store;
}

int add_user_to_treeview(ifreechat_t *ifc, GtkTreeView *treeview, user_t *user) {
	GtkTreeIter user_iter;
	GtkTreeModel *model;
	GdkPixbuf *pixbuf;
	category_t *cat;
	dlist_t *p;
	char title[128];

	if (treeview == NULL || user == NULL)
		return -1;

	model = (GtkTreeModel*)gtk_tree_view_get_model(treeview);
	cat == NULL;
	dlist_foreach(p, &(ifc->clist)) {
		cat = (category_t*)dlist_entry(p, category_t, cnode);
		if (!strcmp(cat->name, user->category))
			break;
	}

	if (p == &(ifc->clist)) {
		/* new category */
		cat = (category_t*)new_category(user->category);
		gtk_tree_store_append((GtkTreeStore*)model, &(cat->iter), NULL);
		gtk_tree_store_set((GtkTreeStore*)model, &(cat->iter),
				PIXBUF_COL, NULL,
				TEXT_COL, cat->name,
				-1);
		dlist_add_tail(&(cat->cnode), &(ifc->clist));
	}

	cat->count++;
	sprintf(title, "%s[%d]", cat->name, cat->count);
	gtk_tree_store_set((GtkTreeStore*)model, &(cat->iter),
			TEXT_COL, title,
			-1);

	pixbuf = (GdkPixbuf*)gdk_pixbuf_new_from_file(user->avatar, NULL);
	gtk_tree_store_append((GtkTreeStore*)model, &user_iter, &(cat->iter));
	gtk_tree_store_set((GtkTreeStore*)model, &user_iter,
			PIXBUF_COL, pixbuf,
			TEXT_COL, user->nickname,
			IP_COL, user->ipaddr,
			MAC_COL, user->macaddr,
			URI_COL, (void*)user,
			-1);
	gdk_pixbuf_unref(pixbuf);
	return 0;
}

gboolean update_userinfo_func(GtkTreeModel *model,
		GtkTreePath *path, GtkTreeIter *iter, gpointer data) {
	pair_t *arg;
	user_t *user;

	char *old_category;
	char *new_category;
	char *ipaddr;
	GdkPixbuf *pixbuf;

	arg = (pair_t*)data;
	user = (user_t*)arg->first;

	if (gtk_tree_path_get_depth(path) > 1) {
		gtk_tree_model_get(model, iter, 
				TEXT_COL, 	&old_category,
			   	IP_COL,		&ipaddr,
				-1);
		if (!strcmp(ipaddr, user->ipaddr)) {
			pixbuf = (GdkPixbuf*)gdk_pixbuf_new_from_file(user->avatar, NULL);
			gtk_tree_store_set((GtkTreeStore*)model, iter,
					PIXBUF_COL, pixbuf,
					TEXT_COL, user->nickname,
					IP_COL, user->ipaddr,
					MAC_COL, user->macaddr,
					-1);
			gdk_pixbuf_unref(pixbuf);
			return TRUE;
		}
	}
	return FALSE;
}

int update_user_to_treeview(GtkTreeView *treeview, user_t *user) {
	GtkTreeModel *model;
	pair_t arg;

	if (treeview == NULL || user == NULL)
		return -1;

	make_pair(&arg, (void*)user, NULL);

	model = gtk_tree_view_get_model(treeview);
	gtk_tree_model_foreach(model, update_userinfo_func, (gpointer)&arg);

	return 0;
}


void contact_treeview_ondoubleclicked(GtkTreeView *tree_view,
		GtkTreePath *path, GtkTreeViewColumn *col, gpointer data) {

	GtkTreeModel *model;
	GtkTreeIter iter;
	ifreechat_t *ifc;
	dlist_t *p;
	pchatbox_t *chatbox;
	user_t *user;

	ifc = (ifreechat_t*)data;

	if (gtk_tree_path_get_depth(path) == 1)
		return;
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
	gtk_tree_model_get_iter(model, &iter, path);
	gtk_tree_model_get(model, &iter, 
			URI_COL, &user,
			-1);

	dlist_foreach(p, &(ifc->pchatbox)) {
		chatbox = (pchatbox_t*)dlist_entry(p, pchatbox_t, chatbox_node);
		if (!strcmp(user->ipaddr, ((user_t*)chatbox->data)->ipaddr)) {
			gtk_window_present((GtkWindow*)chatbox->window);
			return;
		}
	}

	/* create new private chatbox */
	new_pchatbox(ifc, user);
}
