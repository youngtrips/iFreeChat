/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-12
 * File Name: listview.c
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

enum {
	PIXBUF_COL,
	TITLE_COL,
	URI_COL,
	COL_NUM
};

GtkListStore *create_listview_model() {
	GtkListStore *model;
	GtkTreeIter iter;
	GdkPixbuf *pixbuf;
	int i;

	model = gtk_list_store_new(COL_NUM,
			GDK_TYPE_PIXBUF,
			G_TYPE_STRING,
			G_TYPE_POINTER
			);

	/* just for test*/
	pixbuf = gdk_pixbuf_new_from_file("pixmaps/icon.png", NULL);
	for(i = 0;i < 30; i++) {
		gtk_list_store_append(model, &iter);
		gtk_list_store_set(model, &iter,
				PIXBUF_COL, pixbuf,
				TITLE_COL, "test group",
				-1);
	}

	return model;
}

void init_listview(GtkTreeView *tv) {

	GtkCellRenderer *renderer;
	GtkTreeViewColumn *col;
	
	renderer = gtk_cell_renderer_pixbuf_new();
	col = gtk_tree_view_column_new_with_attributes("", renderer,
			"pixbuf", PIXBUF_COL, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tv), col);

	renderer = gtk_cell_renderer_text_new();
	col = gtk_tree_view_column_new_with_attributes("", renderer,
			"text", TITLE_COL, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tv), col);
}


