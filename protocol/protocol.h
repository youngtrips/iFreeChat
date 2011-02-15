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

typedef int (*build_packet_func)(packet_t *pkt, const msg_t *msg);
typedef int (*parse_packet_func)(const packet_t *pkt, msg_t *msg);

typedef struct protocol_t {
	char protocol_name[16];
	char protocol_version[16];

	build_packet_func build_func;
	parse_packet_func parse_func;

	dlist_t node;
}protocol_t;

#endif
