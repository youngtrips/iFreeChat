/*
 * Author: youngtrips
 * Created Time:  2011-02-15
 * File Name: packet.c
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

#include <string.h>

#include "mem_pool.h"
#include "packet.h"

packet_t *new_packet(mem_pool_t *pool, uint32_t ip, uint16_t port, 
		const char *data, uint32_t size) {
	size_t tot_size;
	char *base;

	packet_t *pkt;

	tot_size = sizeof(packet_t) + size + 1;
	base = (char*)mem_pool_alloc(pool, tot_size);
	if (base == NULL)
		return NULL;
	pkt = (packet_t*)base; base += sizeof(packet_t);
	pkt->ip = ip;
	pkt->port = port;
	pkt->size = size;
	pkt->data = base;
	memcpy(pkt->data, data, size);
	return pkt;
}

void free_packet(mem_pool_t *pool, packet_t *packet) {
	mem_pool_free(pool, packet);
}
