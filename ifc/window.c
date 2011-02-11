/*
 * Author: youngtrips
 * Created Time:  2011-01-26
 * File Name: window.c
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
#include "window.h"
#include "group.h"
#include "user.h"
#include "dlist.h"
#include "ifreechat.h"

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


int del_user_from_treeview(GtkTreeView *treeview, user_t *user) {
	if (treeview == NULL || user == NULL)
		return -1;
	return 0;
}

int add_group_to_treeview(GtkTreeView *treeview, group_t *group) {

	return 0;
}

int del_group_from_treeview(GtkTreeView *treeview, group_t *group) {
	
	return 0;
}


int init_window(ifreechat_t *ifc , const char *uifile) {

	window_t *win;
	GladeXML *xml;
	GtkTreeStore *contact_store;

	if (ifc == NULL || uifile == NULL) {
		printf("wrong args...\n");
		return -1;
	}
	win = &(ifc->main_window);

	xml = glade_xml_new(uifile, NULL, NULL);
	if (xml == NULL) {
		return -1;
	}

	glade_xml_signal_autoconnect(xml);


	/* load widgets from xml */
	win->window 			= glade_xml_get_widget(xml, 				"main_window");
	win->avatar 			= (GtkImage*)glade_xml_get_widget(xml, 		"avatar");
	win->nickname 			= (GtkEntry*)glade_xml_get_widget(xml, 		"nickname");
	win->signature 			= (GtkEntry*)glade_xml_get_widget(xml, 		"signature");
	win->search	 			= (GtkEntry*)glade_xml_get_widget(xml,		"search_entry");
	win->contact_treeview 	= (GtkTreeView*)glade_xml_get_widget(xml, 	"contact_treeview");
	win->group_treeview 	= (GtkTreeView*)glade_xml_get_widget(xml,	"group_treeview");
	win->history_treeview 	= (GtkTreeView*)glade_xml_get_widget(xml, 	"history_treeview");

	/* initial avatar */
	gtk_image_set_from_file(win->avatar, ifc->avatar);

	/* initial nickname */
	gtk_entry_set_text(win->nickname, ifc->nickname);

	/* initial personal message (signature) */
	gtk_entry_set_text(win->signature, ifc->signature);


	gtk_widget_hide_all(win->nickname);
	gtk_widget_hide_all(win->signature);

	/* initial contact treeview model */
	contact_store = create_contact_treevie_model();
	gtk_tree_view_set_model(win->contact_treeview, (GtkTreeModel*)contact_store);

	gtk_tree_view_set_headers_visible(win->contact_treeview, FALSE);
	gtk_tree_view_set_level_indentation(win->contact_treeview, -30);

	/* initial contact_treeview */
	init_treeview(win->contact_treeview);

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

void show_window(ifreechat_t *ifc) {

	window_t *win;

	win = &(ifc->main_window);
	g_signal_connect(G_OBJECT(win->window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	g_signal_connect(GTK_OBJECT(win->contact_treeview), 
			"row_activated", 
			GTK_SIGNAL_FUNC(contact_treeview_ondoubleclicked), (gpointer)ifc);
	gtk_widget_show_all(win->window);
}
