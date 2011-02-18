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
#include "blowfish.h"

void on_send_gpmessage(GtkWidget *widget, gchatbox_t *chatbox);

gchatbox_t *new_gchatbox(ifreechat_t *ifc, group_t *group) {
	gchatbox_t *gchatbox;
	dlist_t *gchatbox_list;

	char title[128];

	gchatbox = (gchatbox_t*)malloc(sizeof(gchatbox_t));

	init_chatbox(ifc, gchatbox, GCHATBOX, (void*)group);

	pthread_mutex_lock(&(ifc->gchatbox_lock));
	dlist_add_tail(&(gchatbox->chatbox_node), &(ifc->gchatbox));
	pthread_mutex_unlock(&(ifc->gchatbox_lock));

	sprintf(title, "Group Room %s", group->group_name);
	gtk_window_set_title((GtkWindow*)gchatbox->window, title);

//	gtk_image_set_from_file(gchatbox->avatar_image, user->avatar);
	gtk_label_set_text(gchatbox->first_label, group->group_name);
	gtk_label_set_text(gchatbox->second_label, group->group_info);

	chatbox_set_sendmsg_func(gchatbox, on_send_gpmessage);
	show_chatbox(gchatbox);
	return gchatbox;
}

void on_send_gpmessage(GtkWidget *widget, gchatbox_t *chatbox) {

	gchatbox_t *gchatbox;
	ifreechat_t *ifc;
	group_t *group;
	char buf[65535];
	char plain[1024];
	char cipher[1024];
	unsigned char ivec[8];
	BF_KEY key;
	size_t size;
	int len;

	char *msg;
	time_t pno;

	GtkTextView *input_textview;
	GtkTextView *output_textview;
	CBlowFish *bf;
	

	GtkTextBuffer *input_buffer;
	GtkTextBuffer *output_buffer;
	GtkTextIter start;
	GtkTextIter end;
	GtkTextMark *mark;
	GdkPixbuf *pixbuf;

	gchatbox = (gchatbox_t*)(chatbox);
	ifc = (ifreechat_t*)gchatbox->ifreechat;
	group = (group_t*)gchatbox->data;

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

	bf = CreateBlowFish(ifc->macaddr, strlen(ifc->macaddr));

//	BF_set_key(&key, 12, ifc->macaddr);
	sprintf(plain, "QUNMSGMARK#%lx#%s",
			group->group_id,
			msg);
	len = strlen(plain);
	len = BlowFish_Encrypt(bf, plain, cipher, len);

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
	memcpy(buf + size, cipher, len + 1);
	size += len + 1;
	udp_send_msg((ifreechat_t*)chatbox->ifreechat, ifc->multicast_ip, ifc->port, buf, size);
	DestroyBlowFish(bf);
}
