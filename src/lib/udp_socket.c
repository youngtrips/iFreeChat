/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: udp_socket.c
 * Description: 
 *
 * You should have received a copy of the GNU General Public License
 * along with emesene; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "udp_socket.h"
#include "utils.h"


int init_udp_socket(struct udp_socket_t *usock,
		const char *ip, unsigned short port) {
	struct sockaddr_in addr;

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
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);

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

	return 0;
err:
	close(usock->sock);
	return -1;
}

int udp_start_listen(struct udp_socket_t *usock) {
	pthread_t tid;
	usock->shutdown = 0;
	if (pthread_create(&tid, NULL,
				udp_listen_routine, (void*)usock) != 0) {
		fprintf(stderr, "pthread_create() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		return -1;
	}
	usock->tid = tid;
	return 0;
}

int udp_stop_listen(struct udp_socket_t *usock) {
	if (usock->shutdown == 1) {
		return -1;
	}

	usock->shutdown = 1;
	if (pthread_join(usock->tid, NULL) != 0) {
		fprintf(stderr, "pthread_join() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		return -1;
	}
	return 0;
}


void *udp_listen_routine(void *arg) {
	struct udp_socket_t *usock;
	struct epoll_event events[16];
	struct epoll_event ev;
	struct msg_t *msg;
	int nfds;
	int epfd;
	int fd;
	int i;

	usock = (struct udp_socket_t*)arg;
	if (usock == NULL)
		return NULL;

	epfd = epoll_create(8);
	if (epfd < 0) {
		fprintf(stderr, "epoll_create() error: %s@%s:%d\n",
				strerror(errno), __FILE__, __LINE__);
		return NULL;
	}
	ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
	ev.data.fd = usock->sock;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, usock->sock, &ev) < 0) {
	   fprintf(stderr, "epoll_ctl() error: %s@%s:%d\n",
		   strerror(errno), __FILE__, __LINE__);
	   	close(epfd);
		return NULL;
	}		
	for(;;) {
		nfds = epoll_wait(epfd, events, 10, 10);
		if (nfds < 0) {
			fprintf(stderr, "epoll_wait() error: %s@%s:%d\n",
					strerror(errno), __FILE__, __LINE__);
			break;
		}
		for(i = 0;i < nfds; i++) {
			fd = events[i].data.fd;
			if (events[i].events & (EPOLLIN | EPOLLET)) {
				/* recv message, and put it to queue */
			} else if (events[i].events & (EPOLLOUT | EPOLLET)) {
				/* get message from queue, and send it out. */
			}
		}
	}
	close(epfd);
	return NULL;
}

int udp_send(struct udp_socket_t *usock, const char *ip,
		unsigned short port, const struct msg_t *msg) {
}

int udp_recv(struct udp_socket_t *usock, const char *ip,
		unsigned short *port, struct msg_t *msg) {
}