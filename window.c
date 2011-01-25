/*
 * Author: youngtrips
 * Created Time:  Mon 24 Jan 2011 08:58:43 PM CST
 * File Name: window.c
 * Description: 
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "window.h"
#include "win-config.h"

enum {
	PIXBUF_COL,
	TEXT_COL
};

char *stockNames[] = {
        "WARNING",
        "STOP",
        "NEW",
        "GTK_STOCK_CLEAR",
        "GTK_STOCK_OPEN"
};


  char *stocks[] = {
        GTK_STOCK_DIALOG_WARNING,
        GTK_STOCK_STOP,
        GTK_STOCK_NEW,
        GTK_STOCK_CLEAR,
        GTK_STOCK_OPEN    
    };
gboolean selection_changed(GtkTreeSelection *selection, GtkLabel *label) {

	GtkTreeView *tv;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *active;

	tv = gtk_tree_selection_get_tree_view(selection);
	model = gtk_tree_view_get_model(tv);
	gtk_tree_selection_get_selected(selection, &model, &iter);
	gtk_tree_model_get(model, &iter, 1, &active, -1);
	printf("active: %s\n", active);
}
void row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data) {

	printf("double clicked...\n");
}

void test (GtkTreeView *tv) {

	GtkWidget *cell_view;
	GdkPixbuf *pixbuf;
	GtkTreeIter iter1;
	GtkTreeIter iter2;
	GtkTreeStore *store;
	int i, j, s;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *selection;


	store = gtk_tree_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);
	cell_view = gtk_cell_view_new();
	for(i = 0;i < 1; i++) {
		pixbuf = gdk_pixbuf_new_from_file("online.png", NULL);
		gtk_tree_store_append(store, &iter1, NULL);
		gtk_tree_store_set(store, &iter1, PIXBUF_COL, pixbuf, TEXT_COL, "Ungrouped", -1);
		gdk_pixbuf_unref(pixbuf);

		for(j = 0;j < 3; j++) {
			s = rand() % 5;
			pixbuf = gtk_widget_render_icon(cell_view, stocks[s], GTK_ICON_SIZE_BUTTON, NULL);
			gtk_tree_store_append(store, &iter2, &iter1);
			gtk_tree_store_set(store, &iter2, PIXBUF_COL, pixbuf, TEXT_COL, stockNames[s], -1);
		}
	}

	gtk_tree_view_set_model(tv, store);
	gtk_tree_view_set_headers_visible(tv, FALSE);

	renderer = gtk_cell_renderer_pixbuf_new();
	column = gtk_tree_view_column_new_with_attributes("", renderer,
														"pixbuf", PIXBUF_COL, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tv), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("", renderer,
														"text", TEXT_COL, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tv), column);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv));
	g_signal_connect(G_OBJECT(selection), "changed", G_CALLBACK(selection_changed), NULL);
	g_signal_connect(G_OBJECT(tv), "row-activated", G_CALLBACK(row_activated), NULL);

}
/*
void InitTreeView(GtkTreeView *tv, int ncol, char *col_type[]) {
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *col;
	for(int i = 0;i < ncol; i++) {
		if (!strcmp(col_type[i], "pixbuf"))
			renderer = gtk_cell_renderer_pixbuf_new();
		else if (!strcmp(col_type[i], "text"))
			renderer = gtk_cell_renderer_text_new();
		col = gtk_tree_view_column_new_with_attributes("", renderer,
				col_type[i], i, NULL);
		gtk_tree_view_append_column(GTK_TREE_VIEW(tv), column);
	}

}

*/

window_t * create_window(const char *xml_file) {
	window_t *win = NULL;
	GladeXML *ui;

	if ((win = (window_t*)malloc(sizeof(window_t))) == NULL) {
		fprintf(stderr, "malloc for mainwindow error...\n");
		return NULL;
	}

	ui = glade_xml_new(xml_file, NULL, NULL);

	win->window = glade_xml_get_widget(ui, MAIN_WIN);
	win->contactlist_view = glade_xml_get_widget(ui, MAIN_WIN_CONTACTLIST_TREEVIEW);
	if (win->contactlist_view == NULL) {
		fprintf(stderr, "get tree view error...\n");
	}

	test(win->contactlist_view);

	glade_xml_signal_autoconnect(ui);
	g_signal_connect(GTK_OBJECT(win->window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	return win;
}

void show_window(window_t *win) {
	gtk_widget_show_all(win->window);
}

void destroy_window(window_t *win) {

	free(win);
}
