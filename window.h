/*
 * Author: youngtrips
 * Created Time:  Mon 24 Jan 2011 08:52:55 PM CST
 * File Name: window.h
 * Description: 
 */

#ifndef __WINDOW_H
#define __WINDOW_H

#include "gtk_common.h"

typedef struct window_t {
	GtkImage *avatar_entry;
	GtkEntry *nickname_entry;
	GtkEntry *moto_entry;
	GtkEntry *search_entry;
	GtkTreeView *contactlist_view;
	GtkTreeView *grouplist_view;
	GtkTreeView *history_view;

	GtkWidget *window;
}window_t;

#endif
