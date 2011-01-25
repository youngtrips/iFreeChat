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

class Window {
	public:
		Window(GladeXML *xml);
		~window();

		bool Init();
		bool Show();
	private:
		GtkTreeStore *InitContactListTreeViewModel();
		GtkListStore *InitGroupListTreeViewModel();
		GtkListStore *InitHistoryListTreeViewModel();

	private:
		GtkImage *m_avatar_entry;
		GtkEntry *m_nickname_entry;
		GtkEntry *m_moto_entry;
		GtkEntry *m_search_entry;
		GtkTreeView *m_contactlist_view;
		GtkTreeView *m_grouplist_view;
		GtkTreeView *m_history_view;
		GtkWidget *m_win;
		GladeXML *m_xml;

		GtkTreeStore *m_contactlist_model;
		GtkListStore *m_grouplist_model;
		GtkListStore *m_historylist_model;
};

#endif
