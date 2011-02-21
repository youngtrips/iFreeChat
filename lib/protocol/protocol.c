/*
 * Author: youngtrips
 * Created Time:  2011-02-15
 * File Name: protocol.c
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
#include <string.h>

#include "protocol.h"

int protocol_register(protocol_t *proto, const char *name, const char *version,
		build_packet_func build_func,
		parse_packet_func parse_func,
		handle_msg_func handle_func) {
	if (proto == NULL || name == NULL || version == NULL ||
			build_func == NULL || parse_func == NULL)
		return -1;
	strcpy(proto->protocol_name, 	name);
	strcpy(proto->protocol_version, version);

	proto->build_func 	= build_func;
	proto->parse_func 	= parse_func;
	proto->handle_func	= handle_func;

	return 0;
}

int protocol_build_packet(protocol_t *proto, packet_t **pkt, const msg_t *msg) {
	return proto->build_func(pkt, msg);
}

int protocol_parse_packet(protocol_t *proto, const packet_t *pkt, msg_t *msg) {
	return proto->parse_func(pkt, msg);
}

int protocol_handle_msg(protocol_t *proto, const msg_t *msg, void *user_data) {
	return proto->handle_func(msg, user_data);
}
//int protocol_send_msg(protocol_t *proto, const user_t *user, const char *data) {
//	return proto->send_msg(user, data);
//}

