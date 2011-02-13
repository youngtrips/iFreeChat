/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-10
 * File Name: pchatbox.c
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

#include "user.h"
#include "pchatbox.h"
#include "ifreechat.h"
#include "emotion_box.h"

void close_pchatbox(GtkWidget *widget, gpointer arg);
void on_send_message(GtkWidget *widget, pchatbox_t *chatbox);

void chose_face(GtkWidget *widget, gpointer data) {

	pchatbox_t *dlg = (pchatbox_t*)data;
	int x , y , ex , ey , root_x , root_y;

	gtk_widget_translate_coordinates(widget , dlg->window , 0 , 0 , &ex , &ey );
	gtk_window_get_position(GTK_WINDOW(dlg->window) , &root_x , &root_y);

	x = root_x + ex + 3;
	y = root_y + ey + 46;

	emotion_chose_dialg_init(dlg, x, y);
}

pchatbox_t *new_pchatbox(ifreechat_t *ifc, user_t *user) {
	pchatbox_t *pchatbox;
	dlist_t *pchatbox_list;

	char title[128];
	GladeXML *xml;
	GtkTextBuffer *display_buffer;

	xml = glade_xml_new("glade/pchatbox.glade", NULL, NULL);
	if (xml == NULL) {
		return NULL;
	}

	glade_xml_signal_autoconnect(xml);

	pchatbox = (pchatbox_t*)malloc(sizeof(pchatbox_t));

	/* load widgets from xml */
	pchatbox->window 			= glade_xml_get_widget(xml, "chat_window");
	pchatbox->display_textview 	= (GtkTextView*)glade_xml_get_widget(xml, "display_textview");
	pchatbox->input_textview 	= (GtkTextView*)glade_xml_get_widget(xml, "input_textview");
	pchatbox->avatar_image 		= (GtkImage*)glade_xml_get_widget(xml, "avatar_image");
	pchatbox->send_button 		= (GtkButton*)glade_xml_get_widget(xml, "send_btn");
	pchatbox->close_button 		= (GtkButton*)glade_xml_get_widget(xml, "close_btn");
	pchatbox->nickname_label 	= (GtkLabel*)glade_xml_get_widget(xml, "nickname_label");
	pchatbox->signature_label 	= (GtkLabel*)glade_xml_get_widget(xml, "signature_label");
	pchatbox->chose_face_button	= (GtkToolButton*)glade_xml_get_widget(xml, "chose_face_button");
	pchatbox->remote			= user;
	pchatbox->ifreechat			= (void*)ifc;

	init_dlist_node(&(pchatbox->pchatbox_node));

	pthread_mutex_lock(&(ifc->pchatbox_lock));
	dlist_add_tail(&(pchatbox->pchatbox_node), &(ifc->pchatbox));
	pthread_mutex_unlock(&(ifc->pchatbox_lock));

	sprintf(title, "Chat with %s", user->nickname);
	gtk_window_set_title((GtkWindow*)pchatbox->window, title);
	gtk_image_set_from_file(pchatbox->avatar_image, user->avatar);
	gtk_label_set_text(pchatbox->nickname_label, user->nickname);
	gtk_label_set_text(pchatbox->signature_label, ""/*user->signature*/);


	display_buffer = gtk_text_view_get_buffer(pchatbox->display_textview);

	gtk_text_buffer_create_tag(display_buffer, "lmarg",
			"left_margin", 5, NULL);
	gtk_text_buffer_create_tag(display_buffer, "blue_fg",
			"foreground", "blue", NULL);
	gtk_text_buffer_create_tag(display_buffer, "title_font",
			"font", "Sans 9", NULL);



	g_signal_connect(GTK_OBJECT(pchatbox->window), 
			"destroy", GTK_SIGNAL_FUNC(close_pchatbox), (gpointer)pchatbox);

	g_signal_connect(GTK_OBJECT(pchatbox->send_button),
			"clicked", GTK_SIGNAL_FUNC(on_send_message), pchatbox);

	g_signal_connect(G_OBJECT(pchatbox->chose_face_button), 
			"clicked", G_CALLBACK(chose_face), pchatbox);

	gtk_widget_show_all(pchatbox->window);	
	return pchatbox;
}

void close_pchatbox(GtkWidget *widget, void *arg) {
	pchatbox_t *pchatbox;
	ifreechat_t *ifc;

	pchatbox = (pchatbox_t*)arg;
	ifc = (ifreechat_t*)pchatbox->ifreechat;

	gtk_widget_destroy(pchatbox->window);

	pthread_mutex_lock(&(ifc->pchatbox_lock));
	dlist_del(&(pchatbox->pchatbox_node));
	pthread_mutex_unlock(&(ifc->pchatbox_lock));

	free(pchatbox);
}

void pchatbox_insert_msg(pchatbox_t *chatbox, char *sender, time_t *tm, char *msg) {
	char buf[65535];
	pchatbox_t *pchatbox;
	GtkTextView *output_textview;
	

	GtkTextBuffer *output_buffer;
	GtkTextIter start;
	GtkTextIter end;
	GtkTextMark *mark;

	pchatbox = (pchatbox_t*)(chatbox);

	output_textview = pchatbox->display_textview;

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
						sprintf(file, "pixmaps/faces/%d.gif", id + 1);
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

void on_send_message(GtkWidget *widget, pchatbox_t *chatbox) {

	pchatbox_t *pchatbox;
	ifreechat_t *ifc;
	user_t *user;
	char buf[65535];
	char *msg;
	time_t pno;

	GtkTextView *input_textview;
	GtkTextView *output_textview;
	

	GtkTextBuffer *input_buffer;
	GtkTextBuffer *output_buffer;
	GtkTextIter start;
	GtkTextIter end;
	GtkTextMark *mark;
	GdkPixbuf *pixbuf;

	pchatbox = (pchatbox_t*)(chatbox);
	ifc = (ifreechat_t*)pchatbox->ifreechat;
	user = pchatbox->remote;

	pno = time(NULL);
	input_textview = pchatbox->input_textview;
	output_textview = pchatbox->display_textview;

	input_buffer = gtk_text_view_get_buffer(input_textview);
	output_buffer = gtk_text_view_get_buffer(output_textview);

	gtk_text_buffer_get_start_iter(input_buffer, &start);
	gtk_text_buffer_get_end_iter(input_buffer, &end);
	if (gtk_text_iter_equal(&start, &end)) {
		printf("Please enter message!!!\n");
		return;
	}

	msg = gtk_text_buffer_get_text(input_buffer, &start, &end, TRUE);
	gtk_text_buffer_delete(input_buffer, &start, &end);

	sprintf(buf, "%s(%s):\n", ifc->nickname, my_ctime(&pno));
	gtk_text_buffer_get_end_iter(output_buffer, &end);
	gtk_text_buffer_insert_with_tags_by_name(output_buffer, &end,
			buf, -1, "blue_fg", "lmarg", "title_font", NULL);

	insert_msg_with_emotion_to_textview(output_textview, msg);

	gtk_text_buffer_get_end_iter(output_buffer, &end);
	mark = gtk_text_buffer_create_mark(output_buffer, NULL, &end, FALSE);
	gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(output_textview), mark,
		0.0, TRUE, 0.0, 0.0);
	gtk_text_buffer_delete_mark(output_buffer, mark);

//	printf("msg: [%s]\n", msg);
	sprintf(buf, "1_lbt4_13#128#0016D31F56A6#0#0#0:%lu:%s:%s:%u:%s",
			pno,
			user->username,
			user->hostname,
			0x120,
			msg);
//	printf("buf: %s\n", buf);
	udp_send_msg((ifreechat_t*)chatbox->ifreechat, user->ipaddr, ifc->port, buf, strlen(buf));
}
