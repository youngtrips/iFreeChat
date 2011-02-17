/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-16
 * File Name: handle_message.c
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

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <error.h>

#include "ifreechat.h"
#include "handle_message.h"
#include "udp_socket.h"
#include "packet.h"

int on_entry_func(ifreechat_t *ifc, const void *msg) {
}

int on_exit_func(ifreechat_t *ifc, const void *msg) {
}

int on_pchat_func(ifreechat_t *ifc, const void *msg) {
}

int on_gchat_func(ifreechat_t *ifc, const void *msg) {
}

int on_sendcheck_func(ifreechat_t *ifc, const void *msg) {
}

int handle_message(ifreechat_t *ifc, const void *msg) {
}

void process_message_loop(ifreechat_t *ifc) {
	udp_socket_t *usock;
	mem_pool_t *pool;
	packet_t *pkt;

	usock = (udp_socket_t*)ifc->usock;
	for(;;) {
		udp_recv(usock, &pkt);
		printf("mtime: %u\n", pkt->mtime);
		printf("ip	 : %u\n", pkt->ip);
		printf("port : %u\n", pkt->port);
		printf("size : %u\n", pkt->size);
		printf("data : %s\n", pkt->data);
		if (handle_message(ifc, pkt) < 0) {
			fprintf(stderr, "handle_message() occurs errors...\n");
		}
		mem_pool_free(pool, pkt);
	}
}
