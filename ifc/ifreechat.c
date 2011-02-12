/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-10
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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "gtk_common.h"
#include "ifreechat.h"
#include "window.h"
#include "user.h"
#include "udp_socket.h"

ifreechat_t *init_ifreechat() {
	ifreechat_t *ifc;

	ifc = (ifreechat_t*)malloc(sizeof(ifreechat_t));
	if (ifc == NULL) {
		return NULL;
	}

	init_dlist_node(&(ifc->pchatbox));
	init_dlist_node(&(ifc->gchatbox));
	init_dlist_node(&(ifc->ulist));
	init_dlist_node(&(ifc->glist));
	init_dlist_node(&(ifc->mlist));

	pthread_mutex_init(&(ifc->pchatbox_lock), NULL);
	pthread_mutex_init(&(ifc->gchatbox_lock), NULL);
	pthread_mutex_init(&(ifc->ulist_lock), NULL);
	pthread_mutex_init(&(ifc->glist_lock), NULL);
	pthread_mutex_init(&(ifc->mlist_lock), NULL);

	return ifc;
}

void ifreechat_destroy(ifreechat_t *ifc) {
	if (ifc) 
		free(ifc);
}

int main(int argc, char *argv[]) {
	ifreechat_t *ifc;

	if (!g_thread_supported()) {
		g_thread_init(NULL);
	}
	gdk_threads_init();
	gtk_init(&argc, &argv);

	ifc = init_ifreechat();
	if (read_cfg("ifreechat.xml", ifc) < 0) {
		fprintf(stderr, "read cfg: ifreechat.xml error.\n");
		goto err;
	}

	init_udp_socket(ifc);
	udp_start_listen(ifc);

	init_window(ifc, "glade/ui.glade");
	show_window(ifc);

	online_broadcast(ifc);
	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();
err:
	udp_stop_listen(ifc);
	ifreechat_destroy(ifc);
	return 0;
}
