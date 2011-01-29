/*
 * Author: youngtrips
 * Created Time:  2011-01-29
 * File Name: chatdlg.c
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

#include "user.h"

#include "chatdlg.h"

void close_chatdialog(void *arg);

struct chatdlg_t *init_chatdlg(struct user_t *user) {
	struct chatdlg_t *chatdlg;
	GladeXML *xml;

	xml = glade_xml_new("glade/peerchat_dlg.glade", NULL, NULL);
	if (xml == NULL) {
		return NULL;
	}

	glade_xml_signal_autoconnect(xml);

	chatdlg = (struct chatdlg_t*)malloc(sizeof(struct chatdlg_t));
	/* load widgets from xml */
	chatdlg->window 			= glade_xml_get_widget(xml, "chatdlg");
	chatdlg->display_textview 	= (GtkTextView*)glade_xml_get_widget(xml, "display_textview");
	chatdlg->input_textview 	= (GtkTextView*)glade_xml_get_widget(xml, "input_textview");
	chatdlg->avatar_image 		= (GtkImage*)glade_xml_get_widget(xml, "avatar_image");
	chatdlg->send_button 		= (GtkButton*)glade_xml_get_widget(xml, "send_btn");
	chatdlg->close_button 		= (GtkButton*)glade_xml_get_widget(xml, "close_btn");
	chatdlg->nickname_label 	= (GtkLabel*)glade_xml_get_widget(xml, "nickname_label");
	chatdlg->signature_label 	= (GtkLabel*)glade_xml_get_widget(xml, "signature_label");

	user->chatdlg = (void*)chatdlg;
	g_signal_connect(GTK_OBJECT(chatdlg->window), 
			"destroy", G_CALLBACK(close_chatdialog), (void*)user);

	g_signal_connect(GTK_OBJECT(chatdlg->send_button),
			"clicked", G_CALLBACK(on_send_message), (void*)user);

	gtk_widget_show_all(chatdlg->window);	
}

void close_chatdialog(void *arg) {
	struct user_t *user;
	struct chatdlg_t *chatdlg;

	user = (struct user_t*)arg;
	chatdlg = (struct chatdlg_t*)(user->chatdlg);

	g_object_unref(chatdlg->window);
	g_object_unref(chatdlg->display_textview);
	g_object_unref(chatdlg->input_textview);
	g_object_unref(chatdlg->avatar_image);
	g_object_unref(chatdlg->send_button);
	g_object_unref(chatdlg->close_button);
	g_object_unref(chatdlg->nickname_label);
	g_object_unref(chatdlg->signature_label);
	free(chatdlg);
	user->chatdlg = NULL;
}
