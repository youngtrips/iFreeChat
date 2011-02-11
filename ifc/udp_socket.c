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
#include "utils.h"


int init_udp_socket(ifreechat_t *ifc) {
	udp_socket_t *usock;
	struct sockaddr_in addr;

	usock = (udp_socket_t*)malloc(sizeof(udp_socket_t));
	if (usock == NULL) {
		return -1;
	}

	usock->sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (usock->sock < 0) {
		fprintf(stderr, "socket() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ifc->ipaddr);
	addr.sin_port = htons(ifc->port);

	if (bind(usock->sock, (struct sockaddr*)&addr, 
				sizeof(addr)) < 0) {
		fprintf(stderr, "bind() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		close(usock->sock);
		return -1;
	}

	/* enable port reuse */
	if (enable_socket_reuse(usock->sock) < 0) {
		goto err;
	}
	/* enable broadcast */
	if (enable_socket_broadcast(usock->sock) < 0) {
		printf("enable broadcast option error...\n");
		goto err;
	}

	/* enable multicast */
	if (enable_socket_multicast(usock->sock) < 0) {
		goto err;
	}

	/* set nonblocking */
	if (setnonblocking(usock->sock) < 0) {
		goto err;
	}

	ifc->usock = usock;
	return 0;
err:
	close(usock->sock);
	return -1;
}

int udp_start_listen(ifreechat_t *ifc) {
	udp_socket_t *usock;
	pthread_t tid;

	usock = (udp_socket_t*)(ifc->usock);
	usock->shutdown = 0;

	if (pthread_create(&tid, NULL,
				udp_listen_routine, (void*)ifc) != 0) {
		fprintf(stderr, "pthread_create() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		return -1;
	}
	usock->tid = tid;
	return 0;
}

int udp_stop_listen(ifreechat_t *ifc) {
	udp_socket_t *usock;
	usock = (udp_socket_t*)(ifc->usock);
	if (usock->shutdown == 1) {
		return -1;
	}

	usock->shutdown = 1;
	if (pthread_join(usock->tid, NULL) != 0) {
		fprintf(stderr, "pthread_join() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		return -1;
	}
	close(usock->sock);
	free(usock);
	return 0;
}


void *udp_listen_routine(void *arg) {
	ifreechat_t *ifc;
	udp_socket_t *usock;
	struct epoll_event events[16];
	struct epoll_event ev;
	struct sockaddr_in addr;
	socklen_t socklen;
	struct msg_t *msg;
	char buf[8192];
	int size;
	int nfds;
	int epfd;
	int fd;
	int i;

	ifc = (ifreechat_t*)arg;
	usock = (udp_socket_t*)(ifc->usock);
	if (usock == NULL)
		return NULL;

	epfd = epoll_create(8);
	if (epfd < 0) {
		fprintf(stderr, "epoll_create() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		return NULL;
	}
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = usock->sock;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, usock->sock, &ev) < 0) {
	   fprintf(stderr, "epoll_ctl() error: %s@%s:%d\n",
		   strerror(errno), __FILE__, __LINE__);
	   	close(epfd);
		return NULL;
	}		
	for(;;) {

		if (usock->shutdown == 1)
			break;
		nfds = epoll_wait(epfd, events, sizeof(events), 10);
		if (nfds < 0) {
			fprintf(stderr, "epoll_wait() error: %s@%s:%d\n",
					strerror(errno), __FILE__, __LINE__);
		}
		for(i = 0;i < nfds; i++) {
			fd = events[i].data.fd;
			if (events[i].events & (EPOLLIN | EPOLLET)) {
				/* recv message, and put it to queue */
				memset(&addr, 0, sizeof(addr));
				memset(buf, 0, sizeof(buf));
				socklen = sizeof(addr);
				size = recvfrom(fd, buf, sizeof(buf), 0,
						(struct sockaddr*)&addr, &socklen);
				if (size > 0) {
					process_message(ifc, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port),
							buf, size);
				}
			}
		}	
	}
	close(epfd);
	pthread_exit(0);
	return NULL;
}

int udp_send(ifreechat_t *ifc, const msg_t *msg) {
	struct sockaddr_in addr;
	udp_socket_t *usock;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(msg->ip);
	addr.sin_port = htons(msg->port);

	usock = (udp_socket_t*)ifc->usock;

	return sendto(usock->sock, (void*)msg->buf, msg->buf_size, 0,
				(struct sockaddr*)&addr, sizeof(addr));
}


int udp_send_msg(ifreechat_t *ifc, const char *ip, unsigned port,
		const void *data, size_t size) {
	struct sockaddr_in addr;
	udp_socket_t *usock;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);

	usock = (udp_socket_t*)ifc->usock;

	return sendto(usock->sock, (void*)data, size, 0,
				(struct sockaddr*)&addr, sizeof(addr));
}







