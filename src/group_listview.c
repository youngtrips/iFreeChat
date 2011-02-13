/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-12
 * File Name: group_listview.c
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

#include <stdio.h>

#include "gtk_common.h"
#include "group.h"
#include "group_listview.h"

enum {
	PIXBUF_COL,
	TITLE_COL,
	URI_COL,
	COL_NUM
};


void add_group_to_listview(GtkTreeView *listview, group_t *gp) {
	GtkListStore *model;
	GtkTreeIter iter;
	GdkPixbuf *pixbuf;

	model = (GtkListStore*)gtk_tree_view_get_model(listview);

	pixbuf = gdk_pixbuf_new_from_file("pixmaps/icon.png", NULL);
	gtk_list_store_append(model, &iter);
	gtk_list_store_set(model, &iter,
			PIXBUF_COL, pixbuf,
			TITLE_COL, gp->group_name,
			URI_COL, (gpointer)gp,
			-1);
}

void del_group_from_listview(GtkTreeView *listview, group_t *gp) {
}

void group_treeview_ondoubleclicked(GtkTreeView *tree_view,
		GtkTreePath *path, GtkTreeViewColumn *col, gpointer data) {

	GtkTreeModel *model;
	GtkTreeIter iter;
	ifreechat_t *ifc;
	dlist_t *p;
	gchatbox_t *chatbox;
	group_t *group;

	ifc = (ifreechat_t*)data;

	model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
	gtk_tree_model_get_iter(model, &iter, path);
	gtk_tree_model_get(model, &iter, 
			URI_COL, &group,
			-1);

	dlist_foreach(p, &(ifc->gchatbox)) {
		chatbox = (gchatbox_t*)dlist_entry(p, gchatbox_t, gchatbox_node);
		if (group->group_id == chatbox->group->group_id) {
			gtk_window_present((GtkWindow*)chatbox->window);
			return;
		}
	}

	/* create new private chatbox */
	new_gchatbox(ifc, group);
}
