/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: udp_socket.h
 * Description: 
 *
 */

#ifndef __UDP_SOCKET_H
#define __UDP_SOCKET_H

#include <pthread.h>

#include "socket.h"
#include "ifreechat.h"
#include "msg.h"

typedef struct udp_socket_t {

	char ipv4[32];
	unsigned short port;
	
	int sock;

	pthread_t tid;
	volatile int shutdown;
}udp_socket_t;


int init_udp_socket(ifreechat_t *ifc, const char *ip, unsigned short port);

void *udp_listen_routine(void *arg);

int udp_start_listen(ifreechat_t *ifc);

int udp_stop_listen(ifreechat_t *ifc);

int udp_send(ifreechat_t *ifc, const msg_t *msg); 

#endif
