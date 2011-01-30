#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gtk_common.h"

typedef struct chatdlg_t {
	GtkWidget *window;
	GtkTextView *recv_textview;
	GtkTextView *send_textview;
	GtkButton *send_button;
	GtkToolButton *chose_face_button;
	GtkWindow *emo;
}chatdlg_t;

void send_msg(GtkWidget *widget, gpointer data);
void chose_face(GtkWidget *widget, gpointer data);

int main(int argc, char *argv[]) {

	GladeXML *xml;
	chatdlg_t *chatdlg;
	GtkTextBuffer *recv_buffer;
	GtkTextBuffer *send_buffer;

	gtk_init(&argc, &argv);
	xml = glade_xml_new("chat.glade", NULL, NULL);

	glade_xml_signal_autoconnect(xml);
	chatdlg = (chatdlg_t*)malloc(sizeof(chatdlg_t));

	chatdlg->window 			= glade_xml_get_widget(xml, "chat");
	chatdlg->recv_textview		= (GtkTextView*)glade_xml_get_widget(xml, "recv_textview");
	chatdlg->send_textview		= (GtkTextView*)glade_xml_get_widget(xml, "send_textview");
	chatdlg->send_button		= (GtkButton*)glade_xml_get_widget(xml, "send_button");
	chatdlg->chose_face_button	= (GtkToolButton*)glade_xml_get_widget(xml, "chose_face_button");


	recv_buffer = gtk_text_view_get_buffer(chatdlg->recv_textview);
	send_buffer = gtk_text_view_get_buffer(chatdlg->send_textview);


	gtk_text_buffer_create_tag(recv_buffer, "lmarg",
			"left_margin", 5, NULL);
	gtk_text_buffer_create_tag(recv_buffer, "blue_fg",
			"foreground", "blue", NULL);
	gtk_text_buffer_create_tag(recv_buffer, "title_font",
			"font", "Sans Italic 12", NULL);



	g_signal_connect(G_OBJECT(chatdlg->window),
			"destroy", G_CALLBACK(gtk_main_quit), NULL);

	g_signal_connect(G_OBJECT(chatdlg->send_button), 
			"clicked", G_CALLBACK(send_msg), chatdlg);

	g_signal_connect(G_OBJECT(chatdlg->chose_face_button), 
			"clicked", G_CALLBACK(chose_face), chatdlg);

	gtk_widget_show_all(chatdlg->window);
	gtk_main();

	free(chatdlg);
	return 0;
}

static gboolean emotion_ok_clicked(GtkWidget *widget, 
		GdkEventButton *event , gpointer data)
{
	chatdlg_t *dlg = (chatdlg_t*)data;
	gtk_widget_destroy(dlg->emo);
	return FALSE;
}

void emotion_chose_dialg_init(chatdlg_t *dlg, int x, int y) {

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
			if( k < 52 ){
				sprintf(path , "faces/%d.gif" , (k++) + 1);
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

void send_msg(GtkWidget *widget, gpointer data) {

	chatdlg_t *dlg = (chatdlg_t*)data;
	GtkTextView *recv_textview;
	GtkTextView *send_textview;
	GtkTextBuffer *send_buffer;
	GtkTextBuffer *recv_buffer;
	GtkTextMark *mark;
	GtkTextIter start;
	GtkTextIter end;
	gchar *msg;
	gchar buf[1024];

	recv_textview = dlg->recv_textview;
	send_textview = dlg->send_textview;

	recv_buffer = gtk_text_view_get_buffer(recv_textview);
	send_buffer = gtk_text_view_get_buffer(send_textview);

	gtk_text_buffer_get_start_iter(send_buffer, &start);
	gtk_text_buffer_get_end_iter(send_buffer, &end);
	if (gtk_text_iter_equal(&start, &end)) {
	
		printf("Please enter message!!!\n");
		return;
	}

	msg = gtk_text_buffer_get_text(send_buffer, &start, &end, TRUE);
	gtk_text_buffer_delete(send_buffer, &start, &end);
//	printf("msg: %s\n", msg);


	sprintf(buf, "Me:\n");
	gtk_text_buffer_get_end_iter(recv_buffer, &end);
//	gtk_text_buffer_insert(recv_buffer, &end, buf, strlen(buf));
	gtk_text_buffer_insert_with_tags_by_name(recv_buffer, &end,
			buf, -1, "blue_fg", "lmarg", "title_font", NULL);

	sprintf(buf, "%s\n", msg);
	gtk_text_buffer_get_end_iter(recv_buffer, &end);
	gtk_text_buffer_insert(recv_buffer, &end, buf, strlen(buf));
	
	gtk_text_buffer_get_end_iter(recv_buffer, &end);
	mark = gtk_text_buffer_create_mark(recv_buffer, NULL, &end, FALSE);
	gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(recv_textview), mark,
		0.0, TRUE, 0.0, 0.0);
	gtk_text_buffer_delete_mark(recv_buffer, mark);
}

void chose_face(GtkWidget *widget, gpointer data) {

	chatdlg_t *dlg = (chatdlg_t*)data;
	int x , y , ex , ey , root_x , root_y;

	gtk_widget_translate_coordinates(widget , dlg->window , 0 , 0 , &ex , &ey );
	gtk_window_get_position(GTK_WINDOW(dlg->window) , &root_x , &root_y);

	x = root_x + ex + 3;
	y = root_y + ey + 46;

	emotion_chose_dialg_init(dlg, x, y);
}

