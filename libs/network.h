/*
 * Author: youngtrips
 * Created Time:  2011-01-24
 * File Name: network.h
 * Description: 
 */

#ifndef __NETWORK_H
#define __NETWORK_H

#include <stdint.h>

typedef struct udpserver_socket_t {
	int s;
	uint16_t port;
	uint16_t ipv4;
	struct sockaddr_t bind;
}udp_socket_t;

udp_socket_t *udpserver_socket_init(const char *ipv4, uint16_t port);

void udpserver_socket_destroy(udp_socket_t *udp_socket);


typedef struct network_t {
	udp_socket_t *udp_socket;
	pthread_t udp_listenthread_id;
	pthread_rwlock_t *sendq_rwlock;
	pthread_rwlock_t *recvq_rwlock;
	volatile uint8_t shutdown;
};

#endif
