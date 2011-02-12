/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-10
 * File Name: pchatbox.h
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

#ifndef __PCHATBOX_H
#define __PCHATBOX_H

#include "gtk_common.h"
#include "dlist.h"
#include "user.h"

typedef struct pchatbox_t {
	GtkWidget *window;
	GtkTextView *display_textview;
	GtkTextView *input_textview;
	GtkButton *send_button;
	GtkButton *close_button;
	GtkToolButton *chose_face_button;

	GtkImage *avatar_image;
	GtkLabel *nickname_label;
	GtkLabel *signature_label;

	GtkWindow *emo;


	user_t *remote;
	void *ifreechat;

	dlist_t pchatbox_node;

}pchatbox_t;

#endif
