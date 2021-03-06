/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-10
 * File Name: ifreechat.h
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

#ifndef __IFREECHAT_H
#define __IFREECHAT_H

#include <stdint.h>

#include <pthread.h>

#include "mem_pool.h"
#include "udp_socket.h"
#include "dlist.h"
#include "group.h"
#include "user.h"
#include "protocol.h"
#include "category.h"

#include "window.h"

#define INIT_MEMPOOL_SIZE		(1024 * 1024 * 8)

typedef struct ifreechat_t {

	char nickname[64];
	char username[64];
	char hostname[64];
	char category[64];

	int avatar_type;
	int avatar_id;
	char custom_avatar[128];

	char ipaddr[20];
	char macaddr[20];
	char signature[128];
	char default_encoding[16];
	char current_encoding[16];
	char multicast_ip[20];
	uint16_t port;

	pthread_t main_loop_thread;
	window_t main_window;

	mem_pool_t 		*pool;
	udp_socket_t 	*usock;
	category_t		*clist;
	group_t 		*glist;
	user_t 			*ulist;
	protocol_t 		*proto;
	dlist_t			mlist;

	volatile int 	shutdown;
}ifreechat_t;

typedef struct ui_t {
}ui_t;

typedef struct app_t {
}app_t;


#endif
