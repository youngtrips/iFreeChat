/*
 * Author: youngtrips
 * Created Time:  2011-01-26
 * File Name: ifreechat.c
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
#include "main_window.h"

#include "dlist.h"
#include "group.h"
#include "user.h"
#include "udp_socket.h"

#include "ifreechat.h"

struct ifreechat_t ifreechat;

GThread *udp_listen_th;
GThread *ui_update_th;

void ifreechat_init(struct ifreechat_t *ifc) {
	ifc->glist = (struct dlist_t*)malloc(sizeof(struct dlist_t));
	ifc->ulist = (struct dlist_t*)malloc(sizeof(struct dlist_t));
	ifc->usock = (struct udp_socket_t*)malloc(sizeof(struct udp_socket_t));
	ifc->win = (struct window_t*)malloc(sizeof(struct window_t));

	init_dlist_node(ifc->glist);
	init_dlist_node(ifc->ulist);
}

void ifreechat_destroy(struct ifreechat_t *ifc) {
	free(ifc->glist);
	free(ifc->ulist);
	free(ifc->usock);
	free(ifc->win);
}

int main(int argc, char *argv[]) {

	ifreechat_init(&ifreechat);

	init_udp_socket(ifreechat.usock, "0.0.0.0", 9090);
	udp_start_listen(ifreechat.usock);
//	start_recv_msg(&ifreechat);

	if (!g_thread_supported()) {
		g_thread_init(NULL);
	}
	gdk_threads_init();
	gtk_init(&argc, &argv);

	init_main_window(ifreechat.win, "glade/ui.glade");
	show_main_window(ifreechat.win);
	start_recv_msg(&ifreechat);

	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();

	udp_stop_listen(ifreechat.usock);
	stop_recv_msg(&ifreechat);
	ifreechat_destroy(&ifreechat);

	return 0;
}

