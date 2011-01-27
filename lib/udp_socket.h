/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: udp_socket.h
 * Description: 
 *
 * You should have received a copy of the GNU General Public License
 * along with emesene; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __UDP_SOCKET_H
#define __UDP_SOCKET_H

#include "socket.h"
#include "queue.h"

struct udp_socket_t {

	char ipv4[32];
	unsigned short port;
	
	int sock;

	/* for recv */
	struct queue_t *rque;

	/* for send */
	struct queue_t *wque;

	pthread_t tid;
	volatile int shutdown;
};


int init_udp_socket(struct udp_socket_t *usock, 
		const char *ip, unsigned short port);


void *udp_listen_routine(void *arg);


int udp_start_listen(struct udp_socket_t *usock);

int udp_stop_listen(struct udp_socket_t *usock);

int udp_send(struct udp_socket_t *usock, const char *ip, 
		unsigned short port, const msg_t *msg);

int udp_recv(struct udp_socket_t *usock, const char *ip, 
		unsigned short &port, msg_t *msg); 

#endif
