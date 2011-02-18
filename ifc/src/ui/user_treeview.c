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
			"text", NICKNAME_COL, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tv), col);
}

GtkTreeStore *create_contact_treevie_model() {
	GtkTreeStore *store;


	store = gtk_tree_store_new(COL_NUM, 
			GDK_TYPE_PIXBUF, 
			G_TYPE_STRING,
			G_TYPE_STRING,
			G_TYPE_STRING
			);
	return store;
}

int add_user_to_treeview(ifreechat_t *ifc, GtkTreeView *treeview, user_entry_t *user_entry) { 
	GtkTreeIter user_iter;
	GtkTreeModel *model;
	GdkPixbuf *pixbuf;
	category_entry_t *cat_entry;
	dlist_t *p;
	GtkTreeIter *citer;
	GtkTreeIter *uiter;

	char title[128];

	if (treeview == NULL || user == NULL)
		return -1;

	cat_entry = (category_entry_t*)user_entry->category_entry;
	citer = (GtkTreeIter*)cat_entry->pos;
	model = (GtkTreeModel*)gtk_tree_view_get_model(treeview);

	if (citer == NULL) {
		/* new category */
		citer = (GtkTreeIter*)mem_pool_alloc(ifc->pool, sizeof(GtkTreeIter));
		gtk_tree_store_append((GtkTreeStore*)model, citer, NULL);
		gtk_tree_store_set((GtkTreeStore*)model, citer,
				PIXBUF_COL, NULL,
				TEXT_COL, cat_entry->name,
				-1);
		cat_entry->pos = (void*)citer;
	}

	sprintf(title, "%s[%d]", cat_entry->name, cat_entry->count);
	gtk_tree_store_set((GtkTreeStore*)model, citer,
			TEXT_COL, title,
			-1);

	uiter = (GtkTreeIter*)mem_pool_alloc(ifc->pool, sizeof(GtkTreeIter));
	pixbuf = (GdkPixbuf*)gdk_pixbuf_new_from_file(user->avatar, NULL);
	gtk_tree_store_append((GtkTreeStore*)model, uiter, citer);
	gtk_tree_store_set((GtkTreeStore*)model, uiter,
			PIXBUF_COL, pixbuf,
			TEXT_COL, user->nickname,
			IP_COL, user->ipaddr,
			MAC_COL, user->macaddr,
			URI_COL, (void*)user,
			-1
			);
	gdk_pixbuf_unref(pixbuf);
	user_entry->pos = (void*)uiter;
	return 0;
}

int update_user_to_treeview(GtkTreeView *treeview, user_entry_t *user_entry) {
	GtkTreeModel *model;
	GtkTreeIter *uiter;
	GdkPixbuf *pixbuf;

	if (treeview == NULL || user == NULL)
		return -1;

	uiter = (GtkTreeIter*)user->pos;
	model = gtk_tree_view_get_model(treeview);
	pixbuf = (GdkPixbuf*)gdk_pixbuf_new_from_file(user->avatar, NULL);
	gtk_tree_store_set((GtkTreeStore*)model, iter,
			PIXBUF_COL, pixbuf,
			TEXT_COL, user->nickname,
			IP_COL, user->ipaddr,
			MAC_COL, user->macaddr,
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
		chatbox = (pchatbox_t*)dlist_entry(p, pchatbox_t, chatbox_node);
		if (!strcmp(user->ipaddr, ((user_t*)chatbox->data)->ipaddr)) {
			gtk_window_present((GtkWindow*)chatbox->window);
			return;
		}
	}

	/* create new private chatbox */
	new_pchatbox(ifc, user);
}
