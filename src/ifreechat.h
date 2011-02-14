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

#include "pchatbox.h"
#include "gchatbox.h"
#include "window.h"
#include "dlist.h"
#include "group.h"
#include "user.h"

typedef struct ifreechat_t {

	char cfgfile[1024];
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
	unsigned short port;

	void *usock; 	/* udp socket struct */
	window_t main_window; 	/* main window struct */

	dlist_t pchatbox; 		/* private chat box */
	dlist_t gchatbox; 		/* group chat box */
	dlist_t ulist; 			/* user list */
	dlist_t glist; 			/* group list */
	dlist_t clist; 			/* category list */
	dlist_t mlist; 			/* unread message list */

	pthread_mutex_t pchatbox_lock;
	pthread_mutex_t gchatbox_lock;
	pthread_mutex_t ulist_lock;
	pthread_mutex_t glist_lock;
	pthread_mutex_t mlist_lock;

}ifreechat_t;

#endif
