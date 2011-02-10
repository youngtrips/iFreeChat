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

#include "user.h"
#include "pchatbox.h"
#include "ifreechat.h"

void close_pchatbox(GtkWidget *widget, gpointer arg);
void on_send_message(GtkWidget *widget, pchatbox_t *chatbox);

int new_pchatbox(ifreechat_t *ifc, user_t *user) {
	pchatbox_t *pchatbox;
	dlist_t *pchatbox_list;

	char title[128];
	GladeXML *xml;
	GtkTextBuffer *display_buffer;

	xml = glade_xml_new("glade/pchatbox.glade", NULL, NULL);
	if (xml == NULL) {
		return -1;
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
	pchatbox->remote			= user;
	pchatbox->ifreechat			= (void*)ifc;

	init_dlist_node(&(pchatbox->pchatbox_node));
	dlist_add_tail(&(pchatbox->pchatbox_node), &(ifc->pchatbox));

	sprintf(title, "Chat with %s", user->nickname);
	gtk_window_set_title((GtkWindow*)pchatbox->window, title);
	gtk_image_set_from_file(pchatbox->avatar_image, user->avatar);
	gtk_label_set_text(pchatbox->nickname_label, user->nickname);
	gtk_label_set_text(pchatbox->signature_label, user->signature);


	display_buffer = gtk_text_view_get_buffer(pchatbox->display_textview);

	gtk_text_buffer_create_tag(display_buffer, "lmarg",
			"left_margin", 5, NULL);
	gtk_text_buffer_create_tag(display_buffer, "blue_fg",
			"foreground", "blue", NULL);
	gtk_text_buffer_create_tag(display_buffer, "title_font",
			"font", "Sans Italic 12", NULL);



	g_signal_connect(GTK_OBJECT(pchatbox->window), 
			"destroy", GTK_SIGNAL_FUNC(close_pchatbox), (gpointer)pchatbox);

	g_signal_connect(GTK_OBJECT(pchatbox->send_button),
			"clicked", GTK_SIGNAL_FUNC(on_send_message), pchatbox);

	gtk_widget_show_all(pchatbox->window);	
	return 0;
}

void close_pchatbox(GtkWidget *widget, void *arg) {
	pchatbox_t *pchatbox;

	pchatbox = (pchatbox_t*)arg;
	gtk_widget_destroy(pchatbox->window);
	dlist_del(&(pchatbox->pchatbox_node));
	free(pchatbox);
}


void on_send_message(GtkWidget *widget, pchatbox_t *chatbox) {

	pchatbox_t *pchatbox;
	char buf[65535];
	char *msg;

	GtkTextView *input_textview;
	GtkTextView *output_textview;
	

	GtkTextBuffer *input_buffer;
	GtkTextBuffer *output_buffer;
	GtkTextIter start;
	GtkTextIter end;
	GtkTextMark *mark;

	pchatbox = (pchatbox_t*)(chatbox);

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

	sprintf(buf, "Me:\n");
	gtk_text_buffer_get_end_iter(output_buffer, &end);
	gtk_text_buffer_insert_with_tags_by_name(output_buffer, &end,
			buf, -1, "blue_fg", "lmarg", "title_font", NULL);

	sprintf(buf, "%s\n", msg);
	gtk_text_buffer_get_end_iter(output_buffer, &end);
	gtk_text_buffer_insert(output_buffer, &end, buf, strlen(buf));
	
	gtk_text_buffer_get_end_iter(output_buffer, &end);
	mark = gtk_text_buffer_create_mark(output_buffer, NULL, &end, FALSE);
	gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(output_textview), mark,
		0.0, TRUE, 0.0, 0.0);
	gtk_text_buffer_delete_mark(output_buffer, mark);
}
