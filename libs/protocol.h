/*
 * Author: youngtrips
 * Created Time:  2011-01-24
 * File Name: protocol.h
 * Description: 
 */

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

typedef struct msg_t {
	unsigned long ipv4;
	unsigned short port;
	unsigned short buf_size;
	void *buf;
}msg_t;


#endif
