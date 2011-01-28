/*
 * Author: youngtrips
 * Created Time:  2011-01-26
 * File Name: main_window.c
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

#include "gtk_common.h"
#include "main_window.h"
#include "group.h"
#include "user.h"
#include "dlist.h"

enum {
	PIXBUF_COL,
	TEXT_COL,
	NUM_COL
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
//	GtkWidget *cell_view;
//	GdkPixbuf *pixbuf;
//	GtkTreeIter parent; 
//	GtkTreeIter child;
	GtkTreeStore *store;


	store = gtk_tree_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);
	/*
	cell_view = gtk_cell_view_new();


	dlist_foreach(i, glist) {
		group = (struct group_t*)dlist_entry(i, struct group_t, glist_node);
		pixbuf = gdk_pixbuf_new_from_file("pixmaps/online.png", NULL);
		gtk_tree_store_append(store, &parent, NULL);
		gtk_tree_store_set(store, &parent, PIXBUF_COL, pixbuf, TEXT_COL, group->group_name, -1);
		gdk_pixbuf_unref(pixbuf);

		/ * store treeview iter * /
		group->my_iter = &parent;
		
		dlist_foreach(j, &(group->mlist_head)) {
			user = (struct user_t*)dlist_entry(j, struct user_t, glist_node);
			pixbuf = gdk_pixbuf_new_from_file("pixmaps/online.png", NULL);
			gtk_tree_store_append(store, &child, &parent);
			gtk_tree_store_set(store, &child, PIXBUF_COL, pixbuf, TEXT_COL, user->nickname, -1);
			gdk_pixbuf_unref(pixbuf);

			/ * store treeview iter  * /
			user->parent_iter = &parent;
			user->my_iter = &child;
		}	

	}	
	*/
	return store;
}


int init_main_window(window_t *win , /*struct dlist_t *glist,*/ const char *uifile) {

	GladeXML *xml;
	GtkTreeStore *contact_store;

	if (win == NULL || uifile == NULL) {
		printf("wrong args...\n");
		return -1;
	}

	xml = glade_xml_new(uifile, NULL, NULL);
	if (xml == NULL) {
		return -1;
	}

	glade_xml_signal_autoconnect(xml);


	/* load widgets from xml */
	win->window 			= glade_xml_get_widget(xml, "main_window");
	win->avatar 			= (GtkImage*)glade_xml_get_widget(xml, "avatar");
	win->nickname 			= (GtkEntry*)glade_xml_get_widget(xml, "nickname");
	win->signature 			= (GtkEntry*)glade_xml_get_widget(xml, "signature");
	win->search	 			= (GtkEntry*)glade_xml_get_widget(xml, "search_entry");
	win->contact_treeview 	= (GtkTreeView*)glade_xml_get_widget(xml, "contact_treeview");
	win->group_treeview 	= (GtkTreeView*)glade_xml_get_widget(xml, "group_treeview");
	win->history_treeview 	= (GtkTreeView*)glade_xml_get_widget(xml, "history_treeview");

	/* initial avatar */

	/* initial nickname */

	/* initial personal message (signature) */

	/* initial contact treeview model */
	contact_store = create_contact_treevie_model();
	gtk_tree_view_set_model(win->contact_treeview, (GtkTreeModel*)contact_store);

	gtk_tree_view_set_headers_visible(win->contact_treeview, FALSE);
	gtk_tree_view_set_level_indentation(win->contact_treeview, -30);

	/* initial contact_treeview */
	init_treeview(win->contact_treeview);
//	init_treeview(win->group_treeview, 3, {"pixbuf", "text", "int" });
//	init_treeview(win->history_treeview, 3, {"pixbuf", "text", "int" });


//	g_signal_connect(GTK_OBJECT(win->window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	return 0;
}
//	gtk_widget_show_all(win->window);
//	return 0;
//}

void show_main_window(window_t *win) {
	g_signal_connect(GTK_OBJECT(win->window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show_all(win->window);
}


