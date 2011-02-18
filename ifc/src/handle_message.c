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
#include "user.h"
#include "group.h"
#include "category.h"

#define MAXN_TABLES 0xff

typedef int (*msg_func)(ifreechat_t *ifc, const void *msg);

static msg_func msg_func_table[MAXN_TABLES];

int on_entry_callback(ifreechat_t *ifc, const void *msg) {

	category_entry_t *old_cat_entry;
	category_entry_t *new_cat_entry;
	user_entry_t *user_entry;
	msg_t *pmsg;

	pmsg = (msg_t*)msg;
	printf("user(%s--%s) entry\n", 
			((msg_t*)msg)->nickname,
			((msg_t*)msg)->category
			);

	if (category_find_entry(ifc->category, pmsg->category, &new_cat_entry) < 0) {
		new_cat_entry = new_category_entry(ifc->pool, pmsg->category);
		category_insert_entry(ifc->clist, pmsg->category, new_cat_entry);
		/* update category ui */
	}

	user_entry = (user_entry_t*)user_find_entry(ifc->ulist, pmsg->ip);
	if (user_entry == NULL) {
		user_entry = (user_entry_t*)new_user_entry(ifc->pool,
				pmsg->nickname, pmsg->username,
				pmsg->hostname, pmsg->avatar,
				pmsg->ip,		pmsg->macaddr,
				" ",			pmsg->category,
				"GBK");
		user_entry->category_entry = new_cat_entry;
		user_entry->pos = NULL;
		new_cat_entry->count++;

		/* add user to user list */
		user_add_entry(ifc->ulist, user_entry);

		/* update user ui */

	} else {
		if (strcmp(pmsg->category, user_entry->category) != 0) {
			/* change category */
			old_cat_entry = user_entry->category_entry;
			old_cat_entry->count--;
			new_cat_entry->count++;
			user_entry->category = new_cat_entry;

			/* update old category ui */

			/* delete user from old category ui container */

		}
		
		/* update user entry ui */
	}

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


