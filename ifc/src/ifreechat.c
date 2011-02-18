/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-16
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

#include "handle_message.h"
#include "mem_pool.h"
#include "udp_socket.h"
#include "config.h"
#include "protocol.h"
#include "feiq.h"
#include "category.h"
#include "ifreechat.h"

static int init_freechat(ifreechat_t **ifc) {
	ifreechat_t *ifreechat;
	mem_pool_t *pool;

	ifreechat = (ifreechat_t*)malloc(sizeof(ifreechat_t));
	if (ifreechat == NULL)
		return -1;

	*ifc = ifreechat;
	pool = mem_pool_init(INIT_MEMPOOL_SIZE, sizeof(char));
	ifreechat->pool 	= pool;
	ifreechat->glist 	= create_group(pool);
	ifreechat->ulist 	= create_user(pool);
	ifreechat->clist	= create_category(pool);

	init_dlist_node(&(ifreechat->mlist));
	ifreechat->shutdown = 0;
	/* register protcol */
	ifreechat->proto = (protocol_t*)malloc(sizeof(protocol_t));

	protocol_register(ifreechat->proto, 
			"feiq", 
			"1_lbt",
			feiq_build_packet,
			feiq_parse_packet,
			feiq_handle_msg
			);

	printf("proto name: %s\n", ifreechat->proto->protocol_name);

	/* register callback functions to protocol */
	protocol_reg_entry_callback(ifreechat->proto, 		on_entry_callback);
	protocol_reg_exit_callback(ifreechat->proto, 		on_exit_callback);
	protocol_reg_pchat_callback(ifreechat->proto, 		on_pchat_callback);
	protocol_reg_gchat_callback(ifreechat->proto, 		on_gchat_callback);
	protocol_reg_sendcheck_callback(ifreechat->proto, 	on_sendcheck_callback);

	return 0;
}

static void destroy_freechat(ifreechat_t *ifc) {
	mem_pool_destroy(ifc->pool);
	free(ifc);
}

static int init_network(ifreechat_t *ifc) {
	udp_socket_t *usock;

	usock = create_udp_socket(ifc->pool,
			ifc->ipaddr, ifc->port, 1024);
	if (usock == NULL)
		return -1;
	ifc->usock = usock;
	return udp_start(usock);
}

static void stop_network(ifreechat_t *ifc) {
	udp_stop(ifc->usock);
}

static void freechat_main(ifreechat_t *ifc) {
	
	pthread_create(&(ifc->main_loop_thread), NULL,
			process_message_loop, (void*)ifc);
}

int main(int argc, char *argv[]) {

	ifreechat_t *ifc;

	if (!g_thread_supported()) {
		g_thread_init(NULL);
	}
	gtk_init(&argc, &argv);
	if (init_freechat(&ifc) < 0)
		return 1;
	if (read_cfg(ifc) < 0) {
		printf("read config file error\n");
		return 1;
	}
	/* init gui */
	init_window(ifc, "../glade/ui.glade");
	show_window(ifc);

	/* init network */
	init_network(ifc);
	freechat_main(ifc);

	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();

	ifc->shutdown = 1;

	stop_network(ifc);
	pthread_join(ifc->main_loop_thread, NULL);
	destroy_freechat(ifc);
	return 0;
}

