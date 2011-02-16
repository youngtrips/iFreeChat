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

	uint32_t data_size;
	uint32_t mtime;
	uint32_t command;
	uint32_t packet_id;
	uint16_t port;
	uint16_t avatar_id;
	uint16_t gpmsg_size;

	char *ip;
	char *macaddr;
	char *version;
	char *username;
	char *hostname;
	char *data;

	void *user_data;

	dlist_t list_node;
}msg_t;

#endif
