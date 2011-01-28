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

#include <pthread.h>

#include "gtk_common.h"
#include "main_window.h"

#include "dlist.h"
#include "group.h"
#include "user.h"
#include "udp_socket.h"

#include "ifreechat.h"

static window_t win;

struct dlist_t glist;
struct dlist_t ulist;
struct udp_socket_t usock;
pthread_t tid;
struct ifreechat_t ifreechat;

int main(int argc, char *argv[]) {

	struct dlist_t *p;
	struct dlist_t *j;
	struct group_t *group;
	struct user_t *user;
	int i;

	ifreechat.win = &win;
	ifreechat.glist = &glist;
	ifreechat.ulist = &ulist;
	ifreechat.usock = &usock;

	gtk_init(&argc, &argv);

	if (init_udp_socket(&usock, "0.0.0.0", 2425) < 0)
		return 1;
	if (udp_start_listen(&usock) < 0)
		return 1;

	init_group(&glist);
	init_dlist_node(&ulist);

	start_recv_msg(&tid, &ifreechat);


	/*
	init_main_window(&win, &glist, "glade/ui.glade");


	show_main_window(&win);

	gtk_main();
	*/
	udp_stop_listen(&usock);
	stop_recv_msg(tid);

	return 0;
}
