/*
 * Author: youngtrips
 * Created Time:  2011-01-25
 * File Name: udp_socket.h
 * Description: 
 */

#ifndef __UDP_SOCKET_H
#define __UDP_SOCKET_H


void *udp_routine(void *arg);

class UdpSocket : Socket{
	public:
		UdpSocket(const char *ip, unsigned short m_port);
		~UdpSocket();
		virtual bool Init();
		virtual bool StartListen();
		virtual bool Shutdown();
		ssize_t SendTo(const char *ip, unsigned short port, const msg_t *msg);
		ssize_t RecvFrom(char *ip, unsigned short *port, msg_t *msg);
		unsigned short GetPort() { return m_port; }
		const char *GetIP() { return m_ipv4; }
		int GetSocket() { return m_socket; }
	private:
		unsigned short m_port;
		char m_ipv4[20];
		struct sockaddr_t m_bind_addr;
};

