/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-12
 * File Name: emotion_box.c
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

#include "gtk_common.h"
#include "pchatbox.h"

static gboolean emotion_ok_clicked(GtkWidget *widget, 
		GdkEventButton *event , gpointer data)
{
	pchatbox_t *dlg = (pchatbox_t*)data;
	gtk_widget_destroy(dlg->emo);
	return FALSE;
}

void emotion_chose_dialg_init(pchatbox_t *dlg, int x, int y) {

	GtkWidget *dialog;
	GtkWidget *table;
	GtkWidget *frame , *subframe;
	GtkWidget *img , *eventbox;

	int i = 0 , j = 0 , k = 0;
	char path[1024];

	dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	dlg->emo = dialog;
	gtk_window_set_decorated(GTK_WINDOW(dialog) , FALSE);
	gtk_window_set_type_hint(GTK_WINDOW(dialog), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_widget_set_name(dialog , "mainwindow");
	gtk_window_set_default_size(GTK_WINDOW(dialog) , 300 , 180);
    gtk_window_set_skip_taskbar_hint (GTK_WINDOW(dialog), TRUE);
	gtk_window_move(GTK_WINDOW(dialog) , x , y);

	table = gtk_table_new(10 , 6 , FALSE);
	gtk_widget_set_usize(table , 300 , 180);

	frame = gtk_frame_new(NULL);

	gtk_widget_set_events(dialog , GDK_ALL_EVENTS_MASK);
//	g_signal_connect(dialog , "focus-out-event"
//			, GTK_SIGNAL_FUNC(x_emotion_focus_out) , fxemotion);

	for( j = 0 ; j < 6 ; j ++){
		for( i = 0 ; i < 10 ; i ++){
			subframe = gtk_frame_new(NULL);
			gtk_frame_set_shadow_type(GTK_FRAME(subframe) , GTK_SHADOW_ETCHED_IN);
			bzero(path , sizeof(path));
			//if( k < 52 )
			{
				sprintf(path , "pixmaps/faces/%d.gif" , (k++) + 1);
				img = gtk_image_new_from_file(path);
				if(!img)
					continue;
				eventbox = gtk_event_box_new();
				gtk_widget_set_tooltip_markup(eventbox , "test");
				gtk_container_add(GTK_CONTAINER(eventbox) , img);
				g_signal_connect(eventbox , "button-press-event" 
						, GTK_SIGNAL_FUNC(emotion_ok_clicked) , dlg);
				gtk_container_add(GTK_CONTAINER(subframe) , eventbox);
			}
			gtk_table_attach_defaults(GTK_TABLE(table) , subframe , i , i + 1 , j , j + 1);
		}
	}
	gtk_container_add(GTK_CONTAINER(frame) , table);
	gtk_container_add(GTK_CONTAINER(dialog) , frame);

	gtk_widget_show_all(dialog);
}

