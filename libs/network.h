/*
 * Author: youngtrips
 * Created Time:  2011-01-24
 * File Name: network.h
 * Description: 
 */

#ifndef __NETWORK_H
#define __NETWORK_H

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

#include "protocol.h"

class udp_socket;
class network {
	public:
		network(unsigned short tcp_port, unsigned short udp_port);
		~network();

		bool start();
		bool shutdown();

		void send_msg(const msg_t *msg);
		void recv_msg(msg_t *msg);
		ssize_t recv_msg(char *ip, unsigned short &port, void *data, size_t size);

	private:
		udp_socket *m_udpsock;
		unsigned m_tcp_port;
		unsigned m_udp_port;
};

#endif
