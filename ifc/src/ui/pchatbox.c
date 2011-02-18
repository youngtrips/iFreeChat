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

void on_send_message(GtkWidget *widget, pchatbox_t *chatbox);

pchatbox_t *new_pchatbox(ifreechat_t *ifc, user_entry_t *user) {
	pchatbox_t *pchatbox;
	dlist_t *pchatbox_list;

	char title[128];
	GladeXML *xml;
	GtkTextBuffer *display_buffer;

	pchatbox = (pchatbox_t*)mem_pool_alloc(ifc->pool, sizeof(pchatbox_t));
	user->chatbox = (void*)pchatbox;

	init_chatbox(ifc, pchatbox, PCHATBOX, (void*)user);

	sprintf(title, "Chat with %s", user->nickname);
	gtk_window_set_title((GtkWindow*)pchatbox->window, title);
	gtk_image_set_from_file(pchatbox->avatar_image, user->avatar);
	gtk_label_set_text(pchatbox->first_label, user->nickname);
	gtk_label_set_text(pchatbox->second_label, ""/*user->signature*/);

	chatbox_set_sendmsg_func(pchatbox, on_send_message);
	show_chatbox(pchatbox);
	return pchatbox;
}

void on_send_message(GtkWidget *widget, pchatbox_t *chatbox) {

	pchatbox_t *pchatbox;
	ifreechat_t *ifc;
	user_entry_t *user_entry;
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
	user_entry = (user_entry_t*)pchatbox->data;

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

	printf("msg: [%s]\n", msg);
//	sprintf(buf, "1_lbt4_%d#128#%s#0#0#0:%lu:%s:%s:%u:%s",
//			ifc->avatar_id,
//			ifc->macaddr,
//			pno,
//			ifc->username,
//			ifc->hostname,
//			0x120,
//			msg);
//	printf("buf: %s\n", buf);
}
