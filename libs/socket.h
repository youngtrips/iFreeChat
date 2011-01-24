/*
 * Author: youngtrips
 * Created Time:  2011-01-25
 * File Name: socket.h
 * Description: 
 */

#ifndef __SOCKET_H
#define __SOCKET_H

#include "queue.h"

class Socket {
	public:
		Socket() : m_sendq(new Queue()), m_recvq(new Queue()), m_socket(0){}
		virtual ~Socket() { delete m_sendq; delete m_recvq; }
		virtual bool Init() = 0;
		virtual bool StartListen() = 0;
		virtual bool Shutdown() = 0;
	protected:
		Queue *m_sendq;
		Queue *m_recvq;
		int m_socket;
};

#endif
