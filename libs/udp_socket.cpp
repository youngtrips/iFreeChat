/*
 * Author: youngtrips
 * Created Time:  2011-01-25
 * File Name: udp_socket.cpp
 * Description: 
 */

#include "udp_socket.h"
#include "utils.h"

udp_socket::udp_socket(const char *bind_ip, unsigned short bind_port,
		size_t rqsize, size_t wqsize, size_t rbsize, size_t wbsize,
		socket_mode mode) :
	Socket(rqsize, wqsize, rbsize, wbsize, mode),
	m_bind_port(bind_port) {
		strcpy(m_bind_ip, bind_ip);
}

udp_socket::~udp_socket() {
}

bool udp_socket::init() {
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_socket < 0) {
		fprintf(stderr, "socket() error: %s\n", strerror(errno));
		return false;
	}
	memset(&m_bind_addr, 0, sizeof(m_bind_addr));
	m_bind_addr.sin_family = AF_INET;
	m_bind_addr.sin_addr.s_addr = inet_addr(m_bind_ip);
	m_bind_addr.sin_port = htons(m_bind_port);
	if (bind(m_socket, (struct sockaddr*)&m_bind_addr,
			sizeof(m_bind_addr)) < 0) {
		fprintf(stderr, "bind() error: %s\n", strerror(errno));
		return false;
	}

	if (enable_socket_resuse() == false)
		return false;
	if (enable_broadcast() == false)
		return false;
	if (enable_multicast("226.81.9.8") == false)
		return false;
	if (setnonblocking() == false)
		return false;
	return true;
}

bool udp_socket::start_listen() {
	pthread_create(&m_tid, NULL,
			udp_listen_routine, (void*)this);
}

bool udp_socket::shutdown() {
	if (m_shutdown == true)
		return false;
	m_shutdown = true;
	pthread_join(m_tid, NULL);
	close(m_socket);
	return true;
}

ssize_t udp_socket::sendto(const char *ip, unsigned short port, 
		const void *data, size_t size) {
	msg_t *msg = NULL;
	msg = (msg_t*)malloc(sizeof(msg_t));
	strcpy(msg->ip, ip);
	msg->port = htons(port);
	msg->buf_size = size;
	msg->buf = (char*)malloc(size);
	memcpy(msg->buf, data, size);

	m_wque->push(msg);
	return size;
}

ssize_t udp_socket::recvfrom(char *ip, unsigned short &port,
		void *data, size_t size) {
	ssize_t len;
	msg_t *msg = NULL;
	msg = (msg_t*)m_rque->pop();
	if (msg == NULL)
		return -1;

	strcpy(ip, msg->ip);
	port = msg->port;
	memcpy(data, msg->buf, msg->buf_size);
	len = msg->buf_size;
	free(msg->buf);
	free(msg);
	return len;
}

bool udp_socket::enable_socket_resuse() {
	socklen_t len;
	int opt;
	opt = 1;
	len = sizeof(opt);
	if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, len) < 0) {
		fprintf(stderr, "setsockopt() error: %s\n", strerror(errno));
		return false;
	}
	return true;
}

bool udp_socket::enable_broadcast() {
	socklen_t len;
	int opt;
	opt = 1;
	len = sizeof(opt);
	if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, &opt, len) < 0) {
		fprintf(stderr, "setsockopt() error: %s\n", strerror(errno));
		return false;
	}
	return true;
}

bool udp_socket::enable_multicast(const char *multicast_addr) {
	struct ip_mreq mreq;

	mreq.imr_multiaddr.s_addr = inet_addr(multicast_addr);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
		fprintf(stderr, "setsockopt() error: %s\n", strerror(errno));
		return false;
	}
	return true;
}

bool udp_socket::setnonblocking() {
    int opts;

	opts = fcntl(m_socket, F_GETFL);
    if(opts < 0)
    {
        fprintf(stderr, "fcntl(): %s\n", strerror(errno));
		return false;
    }
    opts = opts | O_NONBLOCK;
    if(fcntl(m_socket, F_SETFL, opts)<0)
    {
        fprintf(stderr, "fcntl(): %s\n", strerror(errno));
		return false;
    } 
	return true;
}


void *udp_listen_routine(void *arg) {
#define MAXN_EVENTS 64
	struct epoll_event events[MAXN_EVENTS];
	struct epoll_event ev;
	udp_socket *usock;
	int sock;
	int nfds;
	int ep;
	int i;
	char buf[4096];
	int size;
	struct sockaddr_in addr;
	socklen_t socklen;
	msg_t *msg;
	queue *rque;
	queue *wque;
	buffer *rbuf;
	buffer *wbuf;

	usock = (udp_socket*)arg;
	if (usock == NULL) 
		return NULL;

	sock = usock->get_socket();
	rque = usock->get_rque();

	wque = usock->get_wque();

	if (rque == NULL || wque == NULL) {
		fprintf(stderr, "queue is null.\n");
		return NULL;
	}

	ep = epoll_create(32);

	ev.data.fd = sock;
	ev.events = EPOLLIN | EPOLLOUT | EPOLLET;

	epoll_ctl(ep, EPOLL_CTL_ADD, sock, &ev);

	for(;;) {
		nfds = epoll_wait(ep, events, MAXN_EVENTS, 100);
		if (nfds == -1) {
			fprintf(stderr, "epoll_wait:%s\n", strerror(errno));
			break;
		}
		for(i = 0;i < nfds; i++) {
			sock = events[i].data.fd;
			if (ev.events & (EPOLLIN | EPOLLET)) {
				memset(&addr, 0, sizeof(addr));
				memset(buf, 0, sizeof(buf));
				socklen = sizeof(addr);
				size = recvfrom(sock, (void*)buf, sizeof(buf), 0,
					   (struct sockaddr*)&addr, &socklen);
				if (size > 0) {
					msg = (msg_t*)malloc(sizeof(msg_t));
					if (msg == NULL) {
						fprintf(stderr, "malloc() error: %s\n",
								strerror(errno));
						break;
					}
					strcpy(msg->ip, inet_ntoa(addr.sin_addr));
					msg->port = ntohs(addr.sin_port); 
					msg->buf_size = size * sizeof(char);
					msg->buf = NULL;
					msg->buf = (char*)malloc(size * sizeof(char) * 2);
					if (msg->buf == NULL) {
						fprintf(stderr, "malloc() error: %s:%s:%d\n",
								strerror(errno), __FILE__, __LINE__);
						break;
					}
					bcopy(buf, msg->buf, size);

					rque->push(msg);
				} else if (size == 0) {
					fprintf(stderr, "recvfrom() error: %s\n",
							strerror(errno));
				}

			} else if (ev.events & (EPOLLOUT | EPOLLET)) {
				msg = (msg_t*)wque->pop();
				if (msg) {
					memset(&addr, 0, sizeof(addr));
					addr.sin_family = AF_INET;
					addr.sin_addr.s_addr = inet_addr(msg->ip);
					addr.sin_port = msg->port;
					size = sendto(sock, (void*)msg->buf, msg->buf_size, 0,
							(struct sockaddr*)&addr, sizeof(addr));
					if (size < 0) {
						fprintf(stderr, "sendto %s:%u error: %s\n",
								inet_ntoa(addr.sin_addr), ntohs(msg->port),
								strerror(errno));
					}
					free(msg->buf);
//					wbuf->pop(msg->buf_size);
					free(msg);
				}
			}
		}
		if (usock->is_shutdown()) {
			break;
		}
	}

	close(ep);
	return NULL;
}

