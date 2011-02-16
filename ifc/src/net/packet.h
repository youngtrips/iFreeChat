/*
 * Author: youngtrips
 * Created Time:  2011-02-15
 * File Name: packet.h
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

#ifndef __PACKET_H
#define __PACKET_H

#include <stdint.h>

#include "mem_pool.h"

typedef struct packet_t {
	uint32_t mtime;
	uint32_t ip;
	uint16_t port;
	uint32_t size;
	char *data;
}packet_t;

packet_t *new_packet(mem_pool_t *pool, uint32_t ip, uint16_t port, 
		const char *data, uint32_t size);

void free_packet(mem_pool_t *pool, packet_t *packet);

#endif
