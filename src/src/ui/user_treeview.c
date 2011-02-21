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
#include "pchatbox.h"

enum {
	PIXBUF_COL,
	NICKNAME_COL,
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
			G_TYPE_STRING,
			G_TYPE_POINTER
			);
	return store;
}

int add_category_to_treeview(ifreechat_t *ifc, category_entry_t *entry) {
	GtkTreeView *treeview;
	GtkTreeModel *model;
	GtkTreeIter *iter;


	treeview = (GtkTreeView*)((ifc->main_window).contact_treeview);
	model = (GtkTreeModel*)gtk_tree_view_get_model(treeview);

	iter = (GtkTreeIter*)mem_pool_alloc(ifc->pool, sizeof(GtkTreeIter));
	entry->pos = (void*)iter;
	gdk_threads_enter();
	gtk_tree_store_append((GtkTreeStore*)model, iter, NULL);
	gtk_tree_store_set((GtkTreeStore*)model, iter,
			PIXBUF_COL, NULL,
			NICKNAME_COL, entry->name,
			-1
			);
	gdk_threads_leave();
	return 0;
}

int add_user_to_treeview(ifreechat_t *ifc, user_entry_t *user_entry) { 
	GtkTreeIter user_iter;
	GtkTreeView *treeview;
	GtkTreeModel *model;
	GdkPixbuf *pixbuf;
	category_entry_t *cat_entry;
	dlist_t *p;
	GtkTreeIter *citer;
	GtkTreeIter *uiter;

	char title[128];

	treeview = (GtkTreeView*)((ifc->main_window).contact_treeview);
	if (treeview == NULL || user_entry == NULL)
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
				NICKNAME_COL, cat_entry->name,
				-1);
		cat_entry->pos = (void*)citer;
	}

	sprintf(title, "%s[%d]", cat_entry->name, cat_entry->count);
	gtk_tree_store_set((GtkTreeStore*)model, citer,
			NICKNAME_COL, title,
			-1);

	uiter = (GtkTreeIter*)mem_pool_alloc(ifc->pool, sizeof(GtkTreeIter));
	pixbuf = (GdkPixbuf*)gdk_pixbuf_new_from_file(user_entry->avatar, NULL);
	gtk_tree_store_append((GtkTreeStore*)model, uiter, citer);
	gtk_tree_store_set((GtkTreeStore*)model, uiter,
			PIXBUF_COL, 	pixbuf,
			NICKNAME_COL, 	user_entry->nickname,
			IP_COL, 		user_entry->ipaddr,
			MAC_COL, 		user_entry->macaddr,
			URI_COL, 		(void*)user_entry,
			-1
			);
	gdk_pixbuf_unref(pixbuf);
	user_entry->pos = (void*)uiter;
	return 0;
}

int insert_chat_msg(ifreechat_t *ifc, user_entry_t *user_entry, msg_t *msg) {
	chatbox_t *chatbox;

	chatbox = (chatbox_t*)user_entry->chatbox;
	if (chatbox == NULL) {
		msg->user_data = (void*)user_entry;
		init_dlist_node(&(msg->list_node));
		dlist_add_tail(&(msg->list_node), &(ifc->mlist));
		set_message_alert(ifc);
		return 0;
	}

	printf("nickname:[%s]\n", user_entry->nickname);
	printf("data: [%s]\n", msg->data);
	chatbox_insert_msg(chatbox, user_entry->nickname, &(msg->mtime), msg->data);
	mem_pool_free(ifc->pool, msg);
	return 0;
}

int update_user_to_treeview(GtkTreeView *treeview, user_entry_t *user_entry) {
	GtkTreeModel *model;
	GtkTreeIter *uiter;
	GdkPixbuf *pixbuf;

	if (treeview == NULL || user_entry == NULL)
		return -1;

	uiter = (GtkTreeIter*)user_entry->pos;
	model = gtk_tree_view_get_model(treeview);
	pixbuf = (GdkPixbuf*)gdk_pixbuf_new_from_file(user_entry->avatar, NULL);
	gtk_tree_store_set((GtkTreeStore*)model, uiter,
			PIXBUF_COL, 	pixbuf,
			NICKNAME_COL,	user_entry->nickname,
			IP_COL, 		user_entry->ipaddr,
			MAC_COL, 		user_entry->macaddr,
			-1
			);
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
	user_entry_t *user_entry;

	ifc = (ifreechat_t*)data;

	if (gtk_tree_path_get_depth(path) == 1)
		return;
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
	gtk_tree_model_get_iter(model, &iter, path);
	gtk_tree_model_get(model, &iter, 
			URI_COL, &user_entry,
			-1);

	chatbox = (chatbox_t*)user_entry->chatbox;

	if (chatbox != NULL) {
		/* show chatbox */
		gtk_window_present((GtkWindow*)chatbox->window);
	} else {
		/* create new private chatbox */
		new_pchatbox(ifc, user_entry);
	}
}
