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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "gtk_common.h"
#include "pchatbox.h"

#include "emotion_box.h"

emotion_t emotion_list[96] = {
	{ 0,	"/:)" 		},
	{ 1,	"/:~" 		},
	{ 2,	"/:*" 		},
	{ 3,	"/:|" 		},
	{ 4,	"/8-)"		},
	{ 5,	"/:<"		},
	{ 6,	"/:$"		},
	{ 7,	"/:X"		},
	{ 8,	"/:Z"		},
	{ 9,	"/:'("		},
	{10,	"/:-|"		},
	{11,	"/:-@"		},
	{12,	"/:P"		},
	{13,	"/:D"		},
	{14,	"/:O"		},
	{15,	"/<rotate>"	},

	{16,	"/:("		},
	{17,	"/:+"		},
	{18,	"/:lenhan"	},
	{19,	"/:Q"		},
	{20,	"/:T"		},
	{21,	"/;P"		},
	{22,	"/;-D"		},
	{23,	"/;d"		},
	{24,	"/;o"		},
	{25,	"/:g"		},
	{26,	"/|-)"		},
	{27,	"/:!"		},
	{28,	"/:L"		},
	{29,	"/:>"		},
	{30,	"/;bin"		},
	{31,	"/:fw"		},
	
	{32,	"/;fd"		},
	{33,	"/:-S"		},
	{34,	"/;?"		},
	{35,	"/;x"		},
	{36,	"/;@"		},
	{37,	"/:8"		},
	{38,	"/;!"		},
	{39,	"/!!!"		},
	{40,	"/:xx"		},
	{41,	"/:bye"		},
	{42,	"/:csweat"	},
	{43,	"/:knose"	},
	{44,	"/:applause"},
	{45,	"/:cdale"	},
	{46,	"/:huaixiao"},
	{47,	"/:shake"	},

	{48,	"/:lhenhen"	},
	{49,	"/:rhenhen"	},
	{50,	"/:yawn"	},
	{51,	"/:snooty"	},
	{52,	"/:chagrin"	},
	{53,	"/:kcry"	},
	{54,	"/:yinxian"	},
	{55,	"/:qinqin"	},
	{56,	"/:xiaren"	},
	{57,	"/:kelin"	},
	{58,	"/:caidao"	},
	{59,	"/:xig"		},
	{60,	"/:bj"		},
	{61,	"/:basketball"},
	{62,	"/:pingpong"},
	{63,	"/:jump"	},

	{64,	"/:coffee"	},
	{65,	"/:eat"		},
	{66,	"/:pig"		},
	{67,	"/:rose"	},
	{68,	"/:fade"	},
	{69,	"/:kiss"	},
	{70,	"/:heart"	},
	{71,	"/:break"	},
	{72,	"/:cake"	},
	{73,	"/:shd"		},
	{74,	"/:bomb"	},
	{75,	"/:dao"		},
	{76,	"/:footb"	},
	{77,	"/:piaocon"	},
	{78,	"/:shit"	},
	{79,	"/:oh"		},

	{80,	"/:moon"	},
	{81,	"/:sun"		},
	{82,	"/;gift"	},
	{83,	"/:hug"		},
	{84,	"/:strong"	},
	{85,	"/;weak"	},
	{86,	"/:share"	},
	{87,	"/:shl"		},
	{88,	"/:baoquan"	},
	{89,	"/:cajole"	},
	{90,	"/:quantou"	},
	{91,	"/:chajin"	},
	{92,	"/:aini"	},
	{93,	"/:sayno"	},
	{94,	"/:sayok"	},
	{95,	"/:love"	},
};

char *str_replace(char *str, const char *oldstr, const char *newstr)
{
    gchar *out = NULL;
    gchar **tokens = NULL;

    tokens = g_strsplit(str , oldstr , -1);
    out = g_strjoinv(newstr , tokens);

    g_strfreev(tokens);
    return out;
}

char *replace_emotion(char *str) {
	char emo[8];
	int i;
	for(i = 0;i < 96; i++) {
		if ((g_strstr_len(str, -1, emotion_list[i].desc)) != NULL) {
			sprintf(emo, "#%d#", i);
			str = str_replace(str, emotion_list[i].desc, emo);
		}
	}
	return str;
}

static gboolean emotion_focus_out(GtkWidget *widget, 
		GdkEventFocus *event , gpointer data)
{
	pchatbox_t *chatbox = (pchatbox_t*)data;
	gtk_widget_destroy((GtkWidget*)chatbox->emo);
	return TRUE;
}

static gboolean emotion_ok_clicked(GtkWidget *widget, 
		GdkEventButton *event , gpointer data)
{
	struct args {
		pchatbox_t *chatbox;
		int id;
	}*emotion_arg;
	pchatbox_t *chatbox;
	int id;
	GtkTextBuffer *buffer;
	GtkTextIter iter;

	emotion_arg = (struct args*)data;
	chatbox = (pchatbox_t*)emotion_arg->chatbox;
	id = emotion_arg->id;
	free(emotion_arg);

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chatbox->input_textview));
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert(buffer, &iter, emotion_list[id].desc, -1);

	gtk_widget_destroy((GtkWidget*)chatbox->emo);
	return FALSE;
}

void emotion_chose_dialg_init(pchatbox_t *chatbox, int x, int y) {

	GtkWidget *dialog;
	GtkWidget *table;
	GtkWidget *frame , *subframe;
	GtkWidget *img , *eventbox;

	int i = 0 , j = 0 , k = 0;
	char path[1024];

	struct args {
		pchatbox_t *chatbox;
		int id;
	}*emotion_arg;

	dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	chatbox->emo = (GtkWindow*)dialog;
	gtk_window_set_decorated(GTK_WINDOW(dialog) , FALSE);
	gtk_window_set_type_hint(GTK_WINDOW(dialog), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_widget_set_name(dialog , "mainwindow");
	gtk_window_set_default_size(GTK_WINDOW(dialog) , 300 , 180);
    gtk_window_set_skip_taskbar_hint (GTK_WINDOW(dialog), TRUE);
	gtk_window_move(GTK_WINDOW(dialog) , x , y);

	table = gtk_table_new(16 , 6 , FALSE);
	gtk_widget_set_usize(table , 480 , 180);

	frame = gtk_frame_new(NULL);

	gtk_widget_set_events(dialog , GDK_ALL_EVENTS_MASK);
	g_signal_connect(dialog , "focus-out-event"
			, GTK_SIGNAL_FUNC(emotion_focus_out) , chatbox);

	for( j = 0 ; j < 6 ; j ++){
		for( i = 0 ; i < 16 ; i ++){
			subframe = gtk_frame_new(NULL);
			gtk_frame_set_shadow_type(GTK_FRAME(subframe) , GTK_SHADOW_ETCHED_IN);
			bzero(path , sizeof(path));
			sprintf(path , "pixmaps/faces/%d.gif" , k + 1);
			img = gtk_image_new_from_file(path);
			if(!img)
				continue;
			eventbox = gtk_event_box_new();
			gtk_widget_set_tooltip_text(eventbox , emotion_list[k].desc);
			gtk_container_add(GTK_CONTAINER(eventbox) , img);

			emotion_arg = (struct args*)malloc(sizeof(struct args));
			emotion_arg->chatbox = chatbox;
			emotion_arg->id = k++;

			g_signal_connect(eventbox , "button-press-event" 
					, GTK_SIGNAL_FUNC(emotion_ok_clicked) , (gpointer)emotion_arg);

			gtk_container_add(GTK_CONTAINER(subframe) , eventbox);
			gtk_table_attach_defaults(GTK_TABLE(table) , subframe , i , i + 1 , j , j + 1);
		}
	}
	gtk_container_add(GTK_CONTAINER(frame) , table);
	gtk_container_add(GTK_CONTAINER(dialog) , frame);

	gtk_widget_show_all(dialog);
}

