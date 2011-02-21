/*
 * Author: youngtrips
 * Created Time:  2011-02-15
 * File Name: protocol.h
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


#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include "dlist.h"
#include "packet.h"
#include "msg.h"

typedef int (*build_packet_func)(packet_t **pkt, const msg_t *msg);
typedef int (*parse_packet_func)(const packet_t *pkt, msg_t *msg);
typedef int (*handle_msg_func)(const msg_t *msg, void *user_data);
typedef int (*callback_func)(void *data, const msg_t *msg);

/*
int on_entry_callback(ifreechat_t *ifc, const void *msg);
int on_exit_callback(ifreechat_t *ifc, const void *msg);
int on_pchat_callback(ifreechat_t *ifc, const void *msg);
int on_gchat_callback(ifreechat_t *ifc, const void *msg);
int on_sendcheck_callback(ifreechat_t *ifc, const void *msg);
*/

typedef struct protocol_t {
	char protocol_name[32];
	char protocol_version[32];

	build_packet_func 	build_func;
	parse_packet_func 	parse_func;
	handle_msg_func		handle_func;

	callback_func on_entry;
	callback_func on_exit;
	callback_func on_pchat;
	callback_func on_gchat;
	callback_func on_sendcheck;

	dlist_t node;
}protocol_t;

int protocol_register(protocol_t *proto, const char *name, const char *version,
		build_packet_func build_func,
		parse_packet_func parse_func,
		handle_msg_func handle_func);

int protocol_build_packet(protocol_t *proto, packet_t **pkt, const msg_t *msg);

int protocol_parse_packet(protocol_t *proto, const packet_t *pkt, msg_t *msg);

int protocol_handle_msg(protocol_t *proto, const msg_t *msg, void *user_data);

static inline void protocol_reg_entry_callback(protocol_t *proto, callback_func func) {
	proto->on_entry = func;
}

static inline void protocol_reg_exit_callback(protocol_t *proto, callback_func func) {
	proto->on_exit = func;
}

static inline void protocol_reg_pchat_callback(protocol_t *proto, callback_func func) {
	proto->on_pchat = func;
}

static inline void protocol_reg_gchat_callback(protocol_t *proto, callback_func func) {
	proto->on_gchat = func;
}

static inline void protocol_reg_sendcheck_callback(protocol_t *proto, callback_func func) {
	proto->on_sendcheck = func;
}

#endif





