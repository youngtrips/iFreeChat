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
#include "protocol.h"
#include "msg.h"

#define MAXN_TABLES 0xff

typedef int (*msg_func)(ifreechat_t *ifc, const void *msg);

static msg_func msg_func_table[MAXN_TABLES];

int on_entry_callback(ifreechat_t *ifc, const void *msg) {
	printf("user(%s) entry\n", ((msg_t*)msg)->username);
	return 0;
}

int on_exit_callback(ifreechat_t *ifc, const void *msg) {
	printf("user(%s) exit\n", ((msg_t*)msg)->username);
	return 0;
}

int on_pchat_callback(ifreechat_t *ifc, const void *msg) {
	msg_t *pmsg = (msg_t*)msg;
	printf("user(%s) send message:\n", pmsg->username);
	printf("[%s]\n", pmsg->data);

	return 0;
}

int on_gchat_callback(ifreechat_t *ifc, const void *msg) {
	return 0;
}

int on_sendcheck_callback(ifreechat_t *ifc, const void *msg) {
	return 0;
}

//int handle_message(ifreechat_t *ifc, const void *msg) {
//}

void process_message_loop(ifreechat_t *ifc) {
	udp_socket_t *usock;
	mem_pool_t *pool;
	packet_t *pkt;
	protocol_t *proto;
	msg_t msg;

	usock = (udp_socket_t*)ifc->usock;
	proto = (protocol_t*)ifc->proto;

	for(;;) {
		udp_recv(usock, (void**)&pkt);
		if (protocol_parse_packet(proto, pkt, &msg) < 0) {
			fprintf(stderr, "parse protocol error...\n");
		} else {
			/* this time msg takes protocol pointer */
			msg.user_data = (void*)proto;
			if (protocol_handle_msg(proto, &msg, (void*)ifc) < 0) {
				fprintf(stderr, "handle_message() occurs errors...\n");
			}
		}
		mem_pool_free(pool, pkt);
	}
}


