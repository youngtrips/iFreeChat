/*
 * Author: youngtrips
 * Created Time:  2011-01-29
 * File Name: chatdlg.c
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

#include "user.h"

#include "chatdlg.h"


struct chatdlg_t *init_chatdlg(struct user_t *user) {
	struct chatdlg_t *chatdlg;
	GladeXML *xml;

	xml = glade_xml_new("glade/peerchat_dlg.glade", NULL, NULL);
	if (xml == NULL) {
		return NULL;
	}

	glade_xml_signal_autoconnect(xml);

	chatdlg = (struct chatdlg_t*)malloc(sizeof(struct chatdlg_t));
	/* load widgets from xml */
	chatdlg->window 		= glade_xml_get_widget(xml, "chatdlg");
	chatdlg->display_textview = (GtkTextView*)glade_xml_get_widget(xml, "display_textview");
	chatdlg->input_textview = (GtkTextView*)glade_xml_get_widget(xml, "input_textview");
	chatdlg->avatar_image = (GtkImage*)glade_xml_get_widget(xml, "avatar_image");
	chatdlg->send_button = (GtkButton*)glade_xml_get_widget(xml, "send_btn");
	chatdlg->close_button = (GtkButton*)glade_xml_get_widget(xml, "close_btn");
	chatdlg->nickname_label = (GtkLabel*)glade_xml_get_widget(xml, "nickname_label");
	chatdlg->signature_label = (GtkLabel*)glade_xml_get_widget(xml, "signature_label");
	
}
