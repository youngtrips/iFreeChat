/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: msg.h
 * Description: 
 *
 */

#ifndef __MSG_H
#define __MSG_H

typedef struct msg_t {
	char *ip;
	unsigned short port;
	size_t size;
	char *data;
}msg_t;

#endif
