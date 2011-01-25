/*
 * Author: youngtrips
 * Created Time:  Tue 25 Jan 2011 10:48:26 AM CST
 * File Name: network.cpp
 * Description: 
 */

#include "network.h"
#include "udp_socket.h"

network::network(unsigned short tcp_port, unsigned short udp_port) :
	m_tcp_port(tcp_port),
	m_udp_port(udp_port),
	m_udpsock(NULL) {
}

bool network::start() {
	m_udpsock = new udp_socket("0.0.0.0", m_udp_port,
			65535, 65535, 65535, 65535, UDP_SRV);
	if (m_udpsock == NULL) 
		return false;
	if (m_udpsock->init() == false)
		return false;
	return m_udpsock->start_listen();
}


void network::send_msg(const msg_t *msg) {
	m_udpsock->sendto(msg->ip, msg->port, msg->buf, msg->buf_size);
}

void network::recv_msg(msg_t *msg) {
	msg->buf_size = m_udpsock->recvfrom(msg->ip, msg->port, msg->buf, msg->buf_cap);
}

bool network::shutdown() {
	return m_udpsock->shutdown();
}

network::~network() {
	delete m_udpsock;
}


