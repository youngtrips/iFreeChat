/*
 * Author: youngtrips
 * Created Time:  2011-01-24
 * File Name: network.c
 * Description: 
 */

#include "network.h"

udp_socket_t *udpserver_socket_init(const char *ipv4, uint16_t port) {
	struct sockaddr_in addr;
	udpserver_socket_t *udp_socket;
	
	udp_socket = (udpserver_socket_t*)malloc(sizeof(udpserver_socket_t));
	if (udp_socket == NULL) {
		return NULL;
	}

	udp_socket->port = port;
	udp_socket->ipv4 = inet_addr(ipv4);
	udp_socket->s = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_socket->s < 0) {
		fprintf(stderr, "socket() error: %s\n", strerror(errno));
		free(udp_socket);
		return NULL;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ipv4);
	addr.sin_port = htons(port);

	if (bind(udp_socket->s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		fprintf(stderr, "bind() error: %s\n", strerror(errno));
		close(udp_socket->s);
		free(udp_socket);
		return NULL;
	}

	return udp_socket;
}

void udpserver_socket_destroy(udp_socket_t *udp_socket) {
	if (udp_socket) {
		close(udp_socket->s);
		free(udp_socket);
	}
}

void *udp_listen_func(void *arg) {
}

network_t *network_init() {
	network_t *net;
	net = (network_t*)malloc(sizeof(network_t));
	net->udp_socket = NULL;
	return net;
}

int network_init_udpserver(network_t *net, const char *bind_ip, uint16_t port) {
	
	net->udp_socket = udpserver_socket_init(bind_ip, port);
	if (net->udp_socket == NULL) 
		return -1;
	return 0;
}

int network_start(network_t *net) {
	pthread_create(&(net->udp_listenthread_id), NULL, 
			udp_listen_func, net);

	return 0;
}


ssize_t network_send(network_t *net, const void *msg, size_t size) {
}

ssize_t network_recv(network_t *net, void *msg, size_t size) {
}

void network_stop(network_t *net) {
}
