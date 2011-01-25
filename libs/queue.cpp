/*
 * Author: youngtrips
 * Created Time:  2011-01-25
 * File Name: queue.cpp
 * Description: 
 */

#include "queue.h"

#define QUEUE_DEFAULT_SIZE 65535

queue::queue() :
	m_data(new void*[QUEUE_DEFAULT_SIZE]),
	m_size(QUEUE_DEFAULT_SIZE),
	m_head(0),
	m_tail(0) {
		pthread_rwlock_init(&m_rwlock, NULL);
}

queue::queue(size_t size) :
	m_data(new void*[size]),
	m_size(size),
	m_head(0),
	m_tail(0) {
}

bool queue::empty() {
	return m_head == m_tail;
}

bool queue::push(void *data) {
//	pthread_rwlock_rdlock(&m_rwlock);
	if ((m_tail + 1) % m_size == m_head) {
//		pthread_rwlock_unlock(&m_rwlock);
		return false;
	}

	m_data[m_tail] = data;
	
	pthread_rwlock_wrlock(&m_rwlock);
	m_tail = (m_tail + 1) % m_size;
	pthread_rwlock_unlock(&m_rwlock);
	return true;
}

void *queue::pop() {
	void *data;
//	pthread_rwlock_rdlock(&m_rwlock);
	if (m_head == m_tail) {
//		pthread_rwlock_unlock(&m_rwlock);
		return NULL;
	}

	data = m_data[m_head];
	pthread_rwlock_wrlock(&m_rwlock);
	m_head = (m_head + 1) % m_size;
	pthread_rwlock_unlock(&m_rwlock);
	return data;
}
