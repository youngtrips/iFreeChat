/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: udp_socket.h
 * Description: 
 *
 */

#ifndef __UDP_SOCKET_H
#define __UDP_SOCKET_H

#include "socket.h"
#include "queue.h"
#include "msg.h"

struct udp_socket_t {

	char ipv4[32];
	unsigned short port;
	
	int sock;

	/* for recv */
	struct queue_t *rque;

	/* for send */
	struct queue_t *wque;

	pthread_mutex_t mutex;
	pthread_mutex_t wq_lock;
	pthread_cond_t cond;

	pthread_t tid;
	volatile int shutdown;
};


int init_udp_socket(struct udp_socket_t *usock, 
		const char *ip, unsigned short port);


void *udp_listen_routine(void *arg);


int udp_start_listen(struct udp_socket_t *usock);

int udp_stop_listen(struct udp_socket_t *usock);

int udp_send(struct udp_socket_t *usock, const struct msg_t *msg); 

int udp_recv(struct udp_socket_t *usock, struct msg_t **msg); 

#endif
