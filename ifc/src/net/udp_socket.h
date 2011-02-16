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
#include "mem_pool.h"
#include "msg_queue.h"
#include "msg.h"

typedef struct udp_socket_t {
	char ip[32];
	uint16_t port;

	volatile int shutdown;
	int fd;

	pthread_t listen_tid;
	pthread_t send_tid;

	msg_queue_t *recv_que;
	msg_queue_t *send_que;
	mem_pool_t *pool;
}udp_socket_t;


udp_socket_t *create_udp_socket(mem_pool_t *pool, 
		const char *ip, uint16_t port, uint32_t queue_size);

void *udp_listen_routine(void *arg);
void *udp_send_routine(void *arg);

int udp_start(udp_socket_t *usock);

int udp_stop(udp_socket_t *usock);


void udp_send(udp_socket_t *usock, const void *data);

void udp_recv(udp_socket_t *usock, void **data);

#endif
