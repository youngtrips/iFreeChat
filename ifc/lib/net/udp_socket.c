/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: udp_socket.c
 * Description: 
 *
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "udp_socket.h"
#include "packet.h"
#include "utils.h"


udp_socket_t *create_udp_socket(mem_pool_t *pool, 
		const char *ip, uint16_t port, size_t que_size) {

	udp_socket_t *usock;
	struct sockaddr_in addr;

	usock = (udp_socket_t*)mem_pool_alloc(pool, sizeof(udp_socket_t));
	if (usock == NULL) {
		return NULL; 
	}
	usock->pool = pool;
	usock->fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (usock->fd < 0) {
		fprintf(stderr, "socket() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		return NULL;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);

	if (bind(usock->fd, (struct sockaddr*)&addr, 
				sizeof(addr)) < 0) {
		fprintf(stderr, "bind() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		close(usock->fd);
		return NULL;
	}

	/* enable port reuse */
	if (enable_socket_reuse(usock->fd) < 0) {
		goto err;
	}
	/* enable broadcast */
	if (enable_socket_broadcast(usock->fd) < 0) {
		printf("enable broadcast option error...\n");
		goto err;
	}

	/* enable multicast */
	if (enable_socket_multicast(usock->fd) < 0) {
		goto err;
	}

	usock->recv_que = (msg_queue_t*)create_msg_queue(pool, que_size);
	usock->send_que = (msg_queue_t*)create_msg_queue(pool, que_size);
	return usock;
err:
	mem_pool_free(pool, usock);
	close(usock->fd);
	return NULL;
}

int udp_start(udp_socket_t *usock) {

	usock->shutdown = 0;

	if (pthread_create(&(usock->listen_tid), NULL, udp_listen_routine, 
				(void*)usock) != 0) {
		fprintf(stderr, "pthread_create() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		return -1;
	}

	

	if (pthread_create(&(usock->send_tid), NULL, udp_send_routine, 
				(void*)usock) != 0) {
		fprintf(stderr, "pthread_create() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		usock->shutdown = 1;
		pthread_join(usock->listen_tid, NULL);
		return -1;
	}


	return 0;
}

int udp_stop(udp_socket_t *usock) {

	if (usock->shutdown == 1) {
		return -1;
	}

	usock->shutdown = 1;
	destroy_msg_queue(usock->recv_que);
	destroy_msg_queue(usock->send_que);

	pthread_join(usock->listen_tid, NULL);
	pthread_join(usock->send_tid, NULL);

	close(usock->fd);
	mem_pool_free(usock->pool, usock);
	return 0;
}


void *udp_listen_routine(void *arg) {
	udp_socket_t *usock;
	mem_pool_t *pool;
	packet_t *pkt;
	struct epoll_event events[1024];
	struct epoll_event ev;
	struct sockaddr_in addr;
	socklen_t socklen;
	char buf[65535];
	int size;
	int nfds;
	int epfd;
	int fd;
	int i;

	usock = (udp_socket_t*)(arg);
	if (usock == NULL)
		return NULL;
	pool = usock->pool;

	epfd = epoll_create(128);
	if (epfd < 0) {
		fprintf(stderr, "epoll_create() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		return NULL;
	}
	ev.events = EPOLLIN;
	ev.data.fd = usock->fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, usock->fd, &ev) < 0) {
	   fprintf(stderr, "epoll_ctl() error: %s@%s:%d\n",
		   strerror(errno), __FILE__, __LINE__);
	   	close(epfd);
		return NULL;
	}		
	for(;;) {

		if (usock->shutdown == 1)
			break;
		nfds = epoll_wait(epfd, events, sizeof(events), 100);
		if (nfds < 0) {
			fprintf(stderr, "epoll_wait() error: %s@%s:%d\n",
					strerror(errno), __FILE__, __LINE__);
		}
		for(i = 0;i < nfds; i++) {
			fd = events[i].data.fd;
			if (events[i].events & EPOLLIN) {
				/* recv message, and put it to queue */
				memset(&addr, 0, sizeof(addr));
				memset(buf, 0, sizeof(buf));
				socklen = sizeof(addr);
				size = recvfrom(fd, buf, sizeof(buf), 0,
						(struct sockaddr*)&addr, &socklen);
				if (size > 0) {
					pkt = (packet_t*)new_packet(pool, ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port),
							buf, size);
					msg_queue_put(usock->recv_que, (void*)pkt);
				}
			}
		}	
	}
	close(epfd);
	pthread_exit(0);
	return NULL;
}

void *udp_send_routine(void *arg) {
	udp_socket_t *usock;
	mem_pool_t *pool;
	packet_t *pkt;
	struct sockaddr_in addr;

	usock = (udp_socket_t*)arg;
	pool = usock->pool;

	for(;;) {
		printf("send ...start wait...\n");
		msg_queue_get(usock->send_que, (void**)&pkt);
		printf("wait...\n");
		if (usock->shutdown == 1)
			break;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(pkt->ip);
		addr.sin_port = htons(pkt->port);
		if (sendto(usock->fd, pkt->data, pkt->size, 0,
					(struct sockaddr*)&addr, sizeof(addr)) < 0) {
			fprintf(stderr, "sendto() error: %s@%s:%d\n",
					strerror(errno), __FILE__, __LINE__);
		}
		free_packet(pool, pkt);
	}
	pthread_exit(0);
	return NULL;
}

void udp_send(udp_socket_t *usock, const void *data) {
	msg_queue_put(usock->send_que, data);
}

void udp_recv(udp_socket_t *usock, void **data) {
	msg_queue_get(usock->recv_que, data);
}


