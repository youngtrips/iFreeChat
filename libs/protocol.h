/*
 * Author: youngtrips
 * Created Time:  2011-01-24
 * File Name: protocol.h
 * Description: 
 */

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

typedef struct msg_t {
	char ip[20];
	unsigned short port;
	unsigned short buf_size;
	unsigned short buf_cap;
	void *buf;
}msg_t;


#endif
