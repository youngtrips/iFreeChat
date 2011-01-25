/*
 * Author: youngtrips
 * Created Time:  2011-01-24
 * File Name: protocol.h
 * Description: 
 */

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include <sys/types.h>

typedef struct msg_t {
	char ip[32];
	unsigned short port;
	size_t buf_size;
	size_t buf_cap;
	char *buf;
}msg_t;


#endif
