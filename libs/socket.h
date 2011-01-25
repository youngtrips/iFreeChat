/*
 * Author: youngtrips
 * Created Time:  2011-01-25
 * File Name: Socket.h
 * Description: 
 */

#ifndef __SOCKET_H
#define __SOCKET_H

#include <stddef.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "buffer.h"
#include "queue.h"

#define MAXN_RQUE_SIZE 65535
#define MAXN_WQUE_SIZE 65535
#define MAXN_RBUF_SIZE 65535
#define MAXN_WBUF_SIZE 65535

enum socket_mode{
	TCP_CLT,
	TCP_SRV,
	UDP_CLT,
	UDP_SRV
};

class Socket {
	public:
		Socket(size_t rqsize, size_t wqsize,
				size_t rbsize, size_t wbsize,
				socket_mode mode) : m_rque(new queue(rqsize)),
		m_wque(new queue(wqsize)),
		m_rbuf(new buffer(rbsize)),
		m_wbuf(new buffer(wbsize)),
		m_rque_size(rqsize),
		m_wque_size(wqsize),
		m_rbuf_size(rbsize),
		m_wbuf_size(wbsize),
		m_shutdown(false),
		m_mode(mode),
   		m_socket(0)	{}

		virtual ~Socket() { 
			delete m_rque; 
			delete m_wque;
			delete m_rbuf;
			delete m_wbuf;
	   	}
		virtual bool init() {} 
		virtual bool start_listen() {} 
		virtual bool shutdown() {} 
	protected:
		queue *m_rque;
		queue *m_wque;
		buffer *m_rbuf;
		buffer *m_wbuf;
		int m_socket;
		socket_mode m_mode;
		size_t m_rque_size;
		size_t m_wque_size;
		size_t m_rbuf_size;
		size_t m_wbuf_size;
		volatile bool m_shutdown;
		pthread_t m_tid;
};

#endif
