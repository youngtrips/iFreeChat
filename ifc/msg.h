/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: msg.h
 * Description: 
 *
 */

#ifndef __MSG_H
#define __MSG_H

#include <stdint.h>

#include "dlist.h"

typedef struct msg_t {
	uint16_t port;
	size_t buf_size;
	char *ip;
	char *version;
	char *packet_no;
	char *username;
	char *hostname;
	char *cmd;
	char *data;
	char *buf;
	dlist_t node; 
}msg_t;

#endif
