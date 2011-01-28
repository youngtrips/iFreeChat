/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-28
 * File Name: ifreechat.h
 * Description: 
 *
 */

#ifndef __IFREECHAT_H
#define __IFREECHAT_H

#include "main_window.h"
#include "dlist.h"

struct ifreechat_t {
	struct window_t *win;
	struct dlist_t *glist;
	struct dlist_t *ulist;
	struct udp_socket_t *usock;
};

#endif
