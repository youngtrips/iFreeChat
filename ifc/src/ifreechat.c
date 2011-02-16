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

#include "mem_pool.h"
#include "udp_socket.h"
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
	ifreechat->group 	= create_group(pool);
	ifreechat->user 	= create_user(pool);

	return 0;
}

static void destroy_freechat(ifreechat_t *ifc) {
	mem_pool_destroy(ifc->pool);
	free(ifc);
}

static int init_network(ifreechat_t *ifc) {
	udp_socket_t *usock;

	usock = create_udp_socket(ifc->pool,
			ifc->ip, ifc->port, 1024);
	if (usock == NULL)
		return -1;
	return udp_start(usock);
}

static void stop_network(ifreechat_t *ifc) {
	udp_stop(ifc->usock);
}

static void freechat_main(ifreechat_t *ifc) {
}

int main() {

	ifreechat_t *ifc;
	int i;

	if (init_freechat(&ifc) < 0)
		return 1;
	if (read_cfg(ifc) < 0) {
		printf("read config file error\n");
		return 1;
	}
	init_network(ifc);

	freechat_main(ifc);
	destroy_freechat(ifc);
	return 0;
}

