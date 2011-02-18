/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-10
 * File Name: chatbox.c
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
#include <pthread.h>
#include <time.h>

#include "chatbox.h"
#include "ifreechat.h"
#include "utils.h"
#include "emotion_box.h"

static void close_chatbox(GtkWidget *widget, gpointer arg);
static void on_close_button(GtkWidget *widget, gpointer data); 
void insert_msg_with_emotion_to_textview(GtkTextView *textview, const char *msg); 

typedef void (*on_sendmsg_func)(GtkWidget *widget, chatbox_t *chatbox);

static void chose_face(GtkWidget *widget, gpointer data) {

	chatbox_t *dlg = (chatbox_t*)data;
	int x , y , ex , ey , root_x , root_y;

	gtk_widget_translate_coordinates(widget , dlg->window , 0 , 0 , &ex , &ey );
	gtk_window_get_position(GTK_WINDOW(dlg->window) , &root_x , &root_y);

	x = root_x + ex + 3;
	y = root_y + ey + 46;

	emotion_chose_dialg_init(dlg, x, y);
}

int init_chatbox(ifreechat_t *ifc, chatbox_t *chatbox, chatbox_type_t type, 
		const void *data) {
	char title[128];
	GladeXML *xml;
	GtkTextBuffer *display_buffer;

	xml = glade_xml_new("../glade/chatbox.glade", NULL, NULL);
	if (xml == NULL) {
		return -1;
	}

	glade_xml_signal_autoconnect(xml);

	/* load widgets from xml */
	chatbox->window 			= glade_xml_get_widget(xml, "chat_window");
	chatbox->display_textview 	= (GtkTextView*)glade_xml_get_widget(xml, "display_textview");
	chatbox->input_textview 	= (GtkTextView*)glade_xml_get_widget(xml, "input_textview");
	chatbox->avatar_image 		= (GtkImage*)glade_xml_get_widget(xml, "avatar_image");
	chatbox->send_button 		= (GtkButton*)glade_xml_get_widget(xml, "send_btn");
	chatbox->close_button 		= (GtkButton*)glade_xml_get_widget(xml, "close_btn");
	chatbox->first_label 		= (GtkLabel*)glade_xml_get_widget(xml, "first_label");
	chatbox->second_label 		= (GtkLabel*)glade_xml_get_widget(xml, "second_label");
	chatbox->chose_face_button	= (GtkToolButton*)glade_xml_get_widget(xml, "chose_face_button");
	chatbox->ifreechat			= (void*)ifc;
	chatbox->data				= (void*)data;
	chatbox->type				= type;

	init_dlist_node(&(chatbox->chatbox_node));

//	sprintf(title, "Chat with %s", user->nickname);
//	gtk_window_set_title((GtkWindow*)chatbox->window, title);
//	gtk_image_set_from_file(chatbox->avatar_image, user->avatar);
//	gtk_label_set_text(chatbox->nickname_label, user->nickname);
//	gtk_label_set_text(chatbox->signature_label, ""/*user->signature*/);


	display_buffer = gtk_text_view_get_buffer(chatbox->display_textview);

	gtk_text_buffer_create_tag(display_buffer, "lmarg",
			"left_margin", 5, NULL);
	gtk_text_buffer_create_tag(display_buffer, "blue_fg",
			"foreground", "blue", NULL);
	gtk_text_buffer_create_tag(display_buffer, "title_font",
			"font", "Sans 9", NULL);

	g_signal_connect(GTK_OBJECT(chatbox->window), 
			"destroy", G_CALLBACK(close_chatbox), (gpointer)chatbox);

	g_signal_connect(GTK_OBJECT(chatbox->close_button), 
			"clicked", G_CALLBACK(on_close_button), (gpointer)chatbox);

	g_signal_connect(G_OBJECT(chatbox->chose_face_button), 
			"clicked", G_CALLBACK(chose_face), chatbox);

	return 0;
}

void chatbox_set_sendmsg_func(chatbox_t *chatbox, on_sendmsg_func func) {
	g_signal_connect(GTK_OBJECT(chatbox->send_button),
			"clicked", GTK_SIGNAL_FUNC(func), chatbox);
}


void show_chatbox(chatbox_t *chatbox) {
	gtk_widget_show_all(chatbox->window);	
}

static void on_close_button(GtkWidget *widget, gpointer data) {
	chatbox_t *chatbox;
	chatbox = (chatbox_t*)data;
	gtk_widget_destroy(chatbox->window);
}

static void close_chatbox(GtkWidget *widget, gpointer data) {
	chatbox_t *chatbox;
	ifreechat_t *ifc;
	GtkWidget *p;
	user_entry_t *user_entry;
	group_entry_t *group_entry;

	chatbox = (chatbox_t*)data;
	ifc = (ifreechat_t*)chatbox->ifreechat;
	gtk_widget_destroyed(chatbox->window, &p);
	if (p != NULL) {
		gtk_widget_destroy(chatbox->window);
	}

	if (chatbox->type == PCHATBOX) {
		user_entry = (user_entry_t*)chatbox->data;
		user_entry->chatbox = NULL;
	} else {
		group_entry = (group_entry_t*)chatbox->data;
		group_entry->chatbox = NULL;
	}

	free(chatbox);
}

void chatbox_insert_msg(chatbox_t *chatbox, char *sender, time_t *tm, char *msg) {
	char buf[65535];
	GtkTextView *output_textview;
	

	GtkTextBuffer *output_buffer;
	GtkTextIter start;
	GtkTextIter end;
	GtkTextMark *mark;

	chatbox = (chatbox_t*)(chatbox);

	output_textview = chatbox->display_textview;

	output_buffer = gtk_text_view_get_buffer(output_textview);

	sprintf(buf, "%s(%s):\n", sender, my_ctime(tm));
	gtk_text_buffer_get_end_iter(output_buffer, &end);
	gtk_text_buffer_insert_with_tags_by_name(output_buffer, &end,
			buf, -1, "blue_fg", "lmarg", "title_font", NULL);

	insert_msg_with_emotion_to_textview(output_textview, msg);
	
	gtk_text_buffer_get_end_iter(output_buffer, &end);
	mark = gtk_text_buffer_create_mark(output_buffer, NULL, &end, FALSE);
	gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(output_textview), mark,
		0.0, TRUE, 0.0, 0.0);
	gtk_text_buffer_delete_mark(output_buffer, mark);
}

void insert_msg_with_emotion_to_textview(GtkTextView *textview, const char *msg) {
	char file[1024];
	char tmp[4];
	char *buf;
	char *str;
	char *p;
	char *q;
	int id;
	int i;
	int j;
	int n;

	GtkTextChildAnchor *anchor;
	GtkTextBuffer *buffer;
	GtkTextIter iter;
	GtkWidget *img;

	buffer = gtk_text_view_get_buffer(textview);
	gtk_text_buffer_get_end_iter(buffer, &iter);

	buf = (char*)malloc(strlen(msg) + 1);
	strcpy(buf, msg);
	str = replace_emotion(buf);

	i = 0; j = 0;
	while(str[i] != '\0') {
		if (str[i] == '#') {
			p = str + i + 1;
			q = strchr(p, '#');
			if (q != NULL) {
				n = q - p;
				if (n == 1 || n == 2) {
					strncpy(tmp, p, n);
					tmp[n] = '\0';
					id = atoi(tmp);
					if (id >= 0 && id < 96) {
						gtk_text_buffer_insert(buffer, &iter, str + j, i - j);
						j = i + n + 2;
						i += n + 1;
						sprintf(file, "../pixmaps/faces/%d.gif", id + 1);
						img = gtk_image_new_from_file(file);
						gtk_widget_show(img);
						anchor = gtk_text_buffer_create_child_anchor(buffer, &iter);
						gtk_text_view_add_child_at_anchor(GTK_TEXT_VIEW(textview),
								img, anchor);
					}
				}
			}
		}
		i++;
	}
	if (j < strlen(str)) {
		gtk_text_buffer_insert(buffer, &iter, str + j, strlen(str + j));
	}
	gtk_text_buffer_insert(buffer, &iter, "\n",  1);
	free(buf);
}
