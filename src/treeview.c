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

#include <string.h>

#include "gtk_common.h"
#include "ifreechat.h"
#include "user.h"

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

typedef struct get_category_arg_t {
	char *category;
	GtkTreeIter *iter;
	int flag;
}get_category_arg_t;

gboolean gtk_treeview_get_category(GtkTreeModel *model,
		GtkTreePath *path, GtkTreeIter *iter, gpointer data) {
	get_category_arg_t *arg;
	char *category;

	arg = (get_category_arg_t*)data;

	if (gtk_tree_path_get_depth(path) == 1) {
		gtk_tree_model_get(model, iter, TEXT_COL, &category, -1);
		if (!strcmp(category, arg->category)) {
			arg->flag = 1;
			memcpy(arg->iter, iter, sizeof(GtkTreeIter));
			return TRUE;
		}
	}
	return FALSE;
}

int add_user_to_treeview(GtkTreeView *treeview, user_t *user) {
	GtkTreeIter category_iter;
	GtkTreeIter user_iter;
	GtkTreeModel *model;
	GdkPixbuf *pixbuf;
	get_category_arg_t arg;

	if (treeview == NULL || user == NULL)
		return -1;

	arg.category = user->category;
	arg.flag = 0;
	arg.iter = &category_iter;

	model = gtk_tree_view_get_model(treeview);

	gtk_tree_model_foreach(model, gtk_treeview_get_category, (gpointer)&arg);

	if (arg.flag == 0) {
		/* new category */
		gtk_tree_store_append((GtkTreeStore*)model, &category_iter, NULL);
		gtk_tree_store_set((GtkTreeStore*)model, &category_iter,
				PIXBUF_COL, NULL,
				TEXT_COL, user->category,
				-1);
	}

	printf("avatar: [%s]\n", user->avatar);
	pixbuf = (GdkPixbuf*)gdk_pixbuf_new_from_file(user->avatar, NULL);
	gtk_tree_store_append((GtkTreeStore*)model, &user_iter, &category_iter);
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
		chatbox = (pchatbox_t*)dlist_entry(p, pchatbox_t, pchatbox_node);
		if (!strcmp(user->ipaddr, chatbox->remote->ipaddr)) {
			gtk_window_present((GtkWindow*)chatbox->window);
			return;
		}
	}

	/* create new private chatbox */
	new_pchatbox(ifc, user);
}
