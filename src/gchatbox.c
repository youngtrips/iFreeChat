/*
 * Author: youngtrips
 * Created Time:  2011-02-13
 * File Name: gchatbox.c
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
#include <openssl/blowfish.h>

#include "group.h"
#include "user.h"
#include "gchatbox.h"
#include "ifreechat.h"
#include "emotion_box.h"

static void close_gchatbox(GtkWidget *widget, gpointer arg);
static void on_close_button(GtkWidget *widget, gpointer data); 
static void on_send_message(GtkWidget *widget, gchatbox_t *chatbox);

static void chose_face(GtkWidget *widget, gpointer data) {

	gchatbox_t *dlg = (gchatbox_t*)data;
	int x , y , ex , ey , root_x , root_y;

	gtk_widget_translate_coordinates(widget , dlg->window , 0 , 0 , &ex , &ey );
	gtk_window_get_position(GTK_WINDOW(dlg->window) , &root_x , &root_y);

	x = root_x + ex + 3;
	y = root_y + ey + 46;

	emotion_chose_dialg_init(dlg, x, y);
}

gchatbox_t *new_gchatbox(ifreechat_t *ifc, group_t *group) {
	gchatbox_t *gchatbox;
	dlist_t *gchatbox_list;

	char title[128];
	GladeXML *xml;
	GtkTextBuffer *display_buffer;

	xml = glade_xml_new("glade/gchatbox.glade", NULL, NULL);
	if (xml == NULL) {
		return NULL;
	}

	glade_xml_signal_autoconnect(xml);

	gchatbox = (gchatbox_t*)malloc(sizeof(gchatbox_t));

	/* load widgets from xml */
	gchatbox->window 			= glade_xml_get_widget(xml, "chat_window");
	gchatbox->display_textview 	= (GtkTextView*)glade_xml_get_widget(xml, "display_textview");
	gchatbox->input_textview 	= (GtkTextView*)glade_xml_get_widget(xml, "input_textview");
	gchatbox->avatar_image 		= (GtkImage*)glade_xml_get_widget(xml, "avatar_image");
	gchatbox->send_button 		= (GtkButton*)glade_xml_get_widget(xml, "send_btn");
	gchatbox->close_button 		= (GtkButton*)glade_xml_get_widget(xml, "close_btn");
	gchatbox->gpname_label	 	= (GtkLabel*)glade_xml_get_widget(xml, "nickname_label");
	gchatbox->gpinfo_label 		= (GtkLabel*)glade_xml_get_widget(xml, "signature_label");
	gchatbox->chose_face_button	= (GtkToolButton*)glade_xml_get_widget(xml, "chose_face_button");
	gchatbox->ifreechat			= (void*)ifc;
	gchatbox->group				= group;

	init_dlist_node(&(gchatbox->gchatbox_node));

	pthread_mutex_lock(&(ifc->gchatbox_lock));
	dlist_add_tail(&(gchatbox->gchatbox_node), &(ifc->glist));
	pthread_mutex_unlock(&(ifc->gchatbox_lock));

	sprintf(title, "Group Room %s", group->group_name);
	gtk_window_set_title((GtkWindow*)gchatbox->window, title);

//	gtk_image_set_from_file(gchatbox->avatar_image, user->avatar);
	gtk_label_set_text(gchatbox->gpname_label, group->group_name);
	gtk_label_set_text(gchatbox->gpinfo_label, group->group_info);


	display_buffer = gtk_text_view_get_buffer(gchatbox->display_textview);

	gtk_text_buffer_create_tag(display_buffer, "lmarg",
			"left_margin", 5, NULL);
	gtk_text_buffer_create_tag(display_buffer, "blue_fg",
			"foreground", "blue", NULL);
	gtk_text_buffer_create_tag(display_buffer, "title_font",
			"font", "Sans 9", NULL);

	g_signal_connect(GTK_OBJECT(gchatbox->window), 
			"destroy", G_CALLBACK(close_gchatbox), (gpointer)gchatbox);

	g_signal_connect(GTK_OBJECT(gchatbox->close_button), 
			"clicked", G_CALLBACK(on_close_button), (gpointer)gchatbox);

	g_signal_connect(GTK_OBJECT(gchatbox->send_button),
			"clicked", GTK_SIGNAL_FUNC(on_send_message), gchatbox);

	g_signal_connect(G_OBJECT(gchatbox->chose_face_button), 
			"clicked", G_CALLBACK(chose_face), gchatbox);

	gtk_widget_show_all(gchatbox->window);	
	return gchatbox;
}

static void on_close_button(GtkWidget *widget, gpointer data) {
	gchatbox_t *gchatbox;
	gchatbox = (gchatbox_t*)data;
	gtk_widget_destroy(gchatbox->window);
}

static void close_gchatbox(GtkWidget *widget, gpointer data) {
	gchatbox_t *gchatbox;
	ifreechat_t *ifc;
	GtkWidget *p;

	gchatbox = (gchatbox_t*)data;
	ifc = (ifreechat_t*)gchatbox->ifreechat;
	gtk_widget_destroyed(gchatbox->window, &p);
	if (p != NULL) {
		gtk_widget_destroy(gchatbox->window);
	}
	pthread_mutex_lock(&(ifc->gchatbox_lock));
	dlist_del(&(gchatbox->gchatbox_node));
	pthread_mutex_unlock(&(ifc->gchatbox_lock));
	free(gchatbox);
}

void gchatbox_insert_msg(gchatbox_t *chatbox, char *sender, time_t *tm, char *msg) {
	char buf[65535];
	gchatbox_t *gchatbox;
	GtkTextView *output_textview;
	

	GtkTextBuffer *output_buffer;
	GtkTextIter start;
	GtkTextIter end;
	GtkTextMark *mark;

	gchatbox = (gchatbox_t*)(chatbox);

	output_textview = gchatbox->display_textview;

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

void ginsert_msg_with_emotion_to_textview(GtkTextView *textview, const char *msg) {
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

static void on_send_message(GtkWidget *widget, gchatbox_t *chatbox) {

	gchatbox_t *gchatbox;
	ifreechat_t *ifc;
	group_t *group;
	char buf[65535];
	char plain[1024];
	char crypt[1024];
	unsigned char ivec[8];
	BF_KEY key;
	size_t size;
	int len;

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

	gchatbox = (gchatbox_t*)(chatbox);
	ifc = (ifreechat_t*)gchatbox->ifreechat;
	group = gchatbox->group;

	pno = time(NULL);
	input_textview = gchatbox->input_textview;
	output_textview = gchatbox->display_textview;

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


	BF_set_key(&key, 12, ifc->macaddr);
	sprintf(plain, "QUNMSGMARK#%lx#%s",
			group->group_id,
			msg);
	printf("plain: [%s]\n", plain);
	len = strlen(plain);
	BF_cbc_encrypt(plain, crypt, len, &key, ivec, BF_ENCRYPT);

//	printf("msg: [%s]\n", msg);
	sprintf(buf, "1_lbt4_%d#128#%s#0#0#%d:%lu:%s:%s:%u:",
			ifc->avatar_id,
			ifc->macaddr,
			len,
			pno,
			ifc->username,
			ifc->hostname,
			4194339);
	size = strlen(buf);
	memcpy(buf + size, crypt, len + 1);
	size += len + 1;
	printf("buf: %s\n", buf);
	udp_send_msg((ifreechat_t*)chatbox->ifreechat, "226.81.9.8", ifc->port, buf, size);
}
