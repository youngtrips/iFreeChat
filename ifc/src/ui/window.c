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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "gtk_common.h"
#include "window.h"
#include "group.h"
#include "user.h"
#include "dlist.h"
#include "ifreechat.h"
#include "msg.h"
#include "pchatbox.h"
#include "treeview.h"
#include "group_listview.h"
#include "pair.h"

void on_nickname_btn_clicked(GtkWidget *widget, gpointer data) {
	window_t *win;
	
	win = (window_t*)data;

	gtk_widget_hide((GtkWidget*)win->nickname_button);
	gtk_widget_show((GtkWidget*)win->nickname_entry);
}

gboolean on_nickname_entry_focus_out(GtkWidget* widget, 
		GdkEventFocus* event , gpointer data) {
	window_t *win;
	
	win = (window_t*)data;
	gtk_widget_hide((GtkWidget*)win->nickname_entry);
	gtk_widget_show((GtkWidget*)win->nickname_button);

	return FALSE; 
}

gboolean on_nickname_entry_activate(GtkWidget* widget , gpointer data) {

	pair_t *update_nickname_arg;
	ifreechat_t *ifc;
	window_t *win;
	gchar *nickname;

	update_nickname_arg = (pair_t*)data;

	ifc = (ifreechat_t*)update_nickname_arg->first;
	win = (window_t*)update_nickname_arg->second;

	nickname = (gchar*)gtk_entry_get_text(win->nickname_entry);
	/* save to config file */
	update_cfg(ifc, "user", "nickname", nickname);
	strcpy(ifc->nickname, nickname);

	/* to broadcast others with my new nickname*/
//	online_broadcast(ifc);

	gtk_button_set_label(win->nickname_button, nickname);
	gtk_widget_hide((GtkWidget*)win->nickname_entry);
	gtk_widget_show((GtkWidget*)win->nickname_button);

	free(update_nickname_arg);
	return TRUE;
}


void tray_icon_activated(GtkWidget *widget, gpointer data) {
	ifreechat_t *ifc;
	pchatbox_t *chatbox;
	msg_t *msg;
	dlist_t *msg_node;
	dlist_t *p;
	group_t *group;
	user_t *user;
	time_t tm;

	ifc = (ifreechat_t*)data;

	/*
	msg_node = (ifc->mlist).next;
	if (msg_node == &(ifc->mlist)) {
		gtk_status_icon_set_blinking((ifc->main_window).icon, FALSE);
		gtk_widget_show(GTK_WIDGET((ifc->main_window).window));
		gtk_window_deiconify(GTK_WINDOW((ifc->main_window).window));
		return;
	}
	pthread_mutex_lock(&(ifc->mlist_lock));
	dlist_del(msg_node);
	pthread_mutex_unlock(&(ifc->mlist_lock));
	
	msg = (msg_t*)dlist_entry(msg_node, msg_t, node);
//	tm = (time_t)atoi(msg->packet_no);
	tm = time(NULL);

	if (msg->gpid == 0) {
		chatbox = NULL;
		pthread_mutex_lock(&(ifc->pchatbox_lock));
		dlist_foreach(p, &(ifc->pchatbox)) {
			chatbox = (pchatbox_t*)dlist_entry(p, pchatbox_t, chatbox_node);
			user = (user_t*)chatbox->data;
			if (!strcmp(user->ipaddr, msg->ip)) {
				gtk_window_present((GtkWindow*)chatbox->window);
				break;
			}
		}
		pthread_mutex_unlock(&(ifc->pchatbox_lock));

		if (chatbox == NULL) {
			user = (user_t*)msg->user;
			chatbox = (pchatbox_t*)new_pchatbox(ifc, user);
			if (chatbox == NULL) {
				printf("create chatbox error...\n");
			} else {
				chatbox_insert_msg(chatbox, user->nickname, &tm, msg->data);
			}
		}
	} else {
		chatbox = NULL;
		pthread_mutex_lock(&(ifc->gchatbox_lock));
		dlist_foreach(p, &(ifc->gchatbox)) {
			chatbox = (gchatbox_t*)dlist_entry(p, gchatbox_t, chatbox_node);
			group = (group_t*)chatbox->data;
			if (group->group_id == msg->gpid) {
				gtk_window_present((GtkWindow*)chatbox->window);
				break;
			}
		}
		pthread_mutex_unlock(&(ifc->gchatbox_lock));

		if (chatbox == NULL) {

			dlist_foreach(p, &(ifc->glist)) {
				group = (group_t*)dlist_entry(p, group_t, gnode);
				if (group->group_id == msg->gpid)
					break;
			}
			user = (user_t*)msg->user;
			chatbox = (gchatbox_t*)new_gchatbox(ifc, group);
			chatbox_insert_msg(chatbox, user->nickname, &tm, msg->data);
		}
	}
	
	if (&(ifc->mlist) == (ifc->mlist).next) {
		gtk_status_icon_set_blinking((ifc->main_window).icon, FALSE);
	}

	
	free(msg);
	*/
}

int init_window(ifreechat_t *ifc , const char *uifile) {

	window_t *win;
	GladeXML *xml;
	GtkTreeStore *contact_store;
	GtkListStore *group_store;
	GtkListStore *history_store;

	GtkButton *btn1;
	GtkButton *btn2;
	char file[1024];
	pair_t *update_nickname_arg;

	dlist_t *p;
	group_t *gp;

	if (ifc == NULL || uifile == NULL) {
		printf("wrong args...\n");
		return -1;
	}
	win = (window_t*)&(ifc->main_window);

	xml = glade_xml_new(uifile, NULL, NULL);
	if (xml == NULL) {
		return -1;
	}

	glade_xml_signal_autoconnect(xml);


	/* load widgets from xml */
	win->window 			= glade_xml_get_widget(xml, 				"main_window");
	win->avatar 			= (GtkImage*)glade_xml_get_widget(xml, 		"avatar");
	win->nickname_entry		= (GtkEntry*)glade_xml_get_widget(xml, 		"nickname");
	win->signature_entry 	= (GtkEntry*)glade_xml_get_widget(xml, 		"signature");
	win->topbox				= (GtkVBox*)glade_xml_get_widget(xml, 		"topbox");
	win->search	 			= (GtkEntry*)glade_xml_get_widget(xml,		"search_entry");
	win->contact_treeview 	= (GtkTreeView*)glade_xml_get_widget(xml, 	"contact_treeview");
	win->group_treeview 	= (GtkTreeView*)glade_xml_get_widget(xml,	"group_treeview");
	win->history_treeview 	= (GtkTreeView*)glade_xml_get_widget(xml, 	"history_treeview");

	/* initial avatar */
	if (ifc->avatar_type == 0) {
		/* use internal avatar */
		sprintf(file, "../pixmaps/avatar/%d.bmp", ifc->avatar_id);
		gtk_image_set_from_file(win->avatar, file);
	} else {
		gtk_image_set_from_file(win->avatar, ifc->custom_avatar);
	}

	/* initial nickname */
	win->nickname_button = (GtkButton*)gtk_button_new_with_label(ifc->nickname);
	gtk_button_set_relief(win->nickname_button, GTK_RELIEF_NONE);

	/* initial personal message (signature) */
	win->signature_button = (GtkButton*)gtk_button_new_with_label(ifc->signature);
	gtk_button_set_relief(win->signature_button, GTK_RELIEF_NONE);

	/* trayicon */
	win->icon = gtk_status_icon_new_from_file("../pixmaps/icon.png");
	g_signal_connect(GTK_STATUS_ICON (win->icon), 
			"activate", 
			G_CALLBACK(tray_icon_activated), 
			(gpointer)ifc);

	gtk_widget_hide((GtkWidget*)win->nickname_entry);
	gtk_widget_hide((GtkWidget*)win->signature_entry);

	gtk_box_pack_start((GtkBox*)win->topbox, (GtkWidget*)win->nickname_button, 
			FALSE, TRUE, 0);
	gtk_box_pack_start((GtkBox*)win->topbox, (GtkWidget*)win->signature_button, 
			FALSE, TRUE, 0);

	gtk_widget_show((GtkWidget*)win->nickname_button);
	gtk_widget_show((GtkWidget*)win->signature_button);

	g_signal_connect(G_OBJECT(win->nickname_button),
			"clicked",
			G_CALLBACK(on_nickname_btn_clicked), (gpointer)win);

	g_signal_connect(G_OBJECT(win->nickname_entry),
			"focus-out-event",
			GTK_SIGNAL_FUNC(on_nickname_entry_focus_out),
			(gpointer)win);

	update_nickname_arg = (pair_t*)malloc(sizeof(pair_t));
	make_pair(update_nickname_arg, (void*)ifc, (void*)win);

	g_signal_connect(G_OBJECT(win->nickname_entry),
			"activate",
			G_CALLBACK(on_nickname_entry_activate),
			(gpointer)update_nickname_arg);

	/* initial contact treeview model */
	contact_store = create_contact_treevie_model();
	gtk_tree_view_set_model(win->contact_treeview, (GtkTreeModel*)contact_store);

	gtk_tree_view_set_headers_visible(win->contact_treeview, FALSE);
	gtk_tree_view_set_level_indentation(win->contact_treeview, -30);

	/* initial contact_treeview */
	init_treeview(win->contact_treeview);


	group_store = create_listview_model();
	gtk_tree_view_set_model(win->group_treeview, (GtkTreeModel*)group_store);
	gtk_tree_view_set_headers_visible(win->group_treeview, FALSE);
	init_listview(win->group_treeview);

	/* add default group to listview */
//	dlist_foreach(p, &(ifc->glist)) {
//		gp = (group_t*)dlist_entry(p, group_t, gnode);
//		add_group_to_listview(win->group_treeview, gp);
//	}


	history_store = create_listview_model();
	gtk_tree_view_set_model(win->history_treeview, (GtkTreeModel*)history_store);
	gtk_tree_view_set_headers_visible(win->history_treeview, FALSE);
	init_listview(win->history_treeview);

	return 0;
}



static gboolean window_state_event (GtkWidget *widget, GdkEventWindowState *event, gpointer trayIcon)
{
    if(event->changed_mask == GDK_WINDOW_STATE_ICONIFIED && 
			(event->new_window_state == GDK_WINDOW_STATE_ICONIFIED || 
			 event->new_window_state == (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED))) {
        gtk_widget_hide (GTK_WIDGET(widget));
        gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), TRUE);
    } else if(event->changed_mask == GDK_WINDOW_STATE_WITHDRAWN &&
			(event->new_window_state == GDK_WINDOW_STATE_ICONIFIED || 
			 event->new_window_state == (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED))) {
        gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), TRUE);
    }
    return TRUE;
}


void show_window(ifreechat_t *ifc) {

	window_t *win;

	win = &(ifc->main_window);
	g_signal_connect(G_OBJECT(win->window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(win->window), "window-state-event", G_CALLBACK (window_state_event), win->icon);

	
	g_signal_connect(GTK_OBJECT(win->contact_treeview), 
			"row_activated", 
			GTK_SIGNAL_FUNC(contact_treeview_ondoubleclicked), (gpointer)ifc);
	g_signal_connect(GTK_OBJECT(win->group_treeview), 
			"row_activated", 
			GTK_SIGNAL_FUNC(group_treeview_ondoubleclicked), (gpointer)ifc);
	gtk_widget_show(win->window);
}


