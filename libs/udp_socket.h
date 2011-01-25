/*
 * Author: youngtrips
 * Created Time:  2011-01-25
 * File Name: udp_socket.h
 * Description: 
 */

#ifndef __UDP_SOCKET_H
#define __UDP_SOCKET_H

#include "protocol.h"
#include "socket.h"

void *udp_listen_routine(void *arg);

class udp_socket : public Socket {
	public:
		udp_socket(const char *bind_ip, unsigned short m_port,
				size_t rqsize, size_t wqsize,
				size_t rbsize, size_t wbsize,
				socket_mode mode);
		~udp_socket();

		virtual bool init();
		virtual bool start_listen();
		virtual bool shutdown();

		ssize_t sendto(const char *ip, unsigned short port, 
				const void *data, size_t size);
		ssize_t recvfrom(char *ip, unsigned short &port,
				void *data, size_t size);

		unsigned short get_bind_port() { return m_bind_port; }
		const char *get_bind_ip() { return m_bind_ip; }
		int get_socket() { return m_socket; }
		bool is_shutdown() {
			return m_shutdown == true;
		}

	private:
		bool enable_socket_resuse();
		bool enable_broadcast();
		bool enable_multicast(const char *mulitcast_addr);
		bool setnonblocking();

	private:
		unsigned short m_bind_port;
		char m_bind_ip[20];
		struct sockaddr_in m_bind_addr;
};

#endif
