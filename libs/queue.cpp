/*
 * Author: youngtrips
 * Created Time:  2011-01-25
 * File Name: queue.cpp
 * Description: 
 */

#include "queue.h"

#define QUEUE_DEFAULT_SIZE 65535

Queue::Queue() :
	m_data(new void*[QUEUE_DEFAULT_SIZE]),
	m_size(QUEUE_DEFAULT_SIZE),
	m_head(0),
	m_tail(0) {
}

Queue::Queue(size_t size) :
	m_data(new void*[size]),
	m_size(size),
	m_head(0),
	m_tail(0) {
}

bool Queue::Empty() {
	return m_head == m_tail;
}

bool Queue::Push(void *data) {
	if ((m_tail + 1) % m_size == m_head) 
		return false;
	m_data[m_tail] = data;
	m_tail = (m_tail + 1) % m_size;
	return true;
}

void *Queue::Pop() {
	void *data;
	if (m_head == m_tail) 
		return NULL;
	data = m_data[m_head];
	m_head = (m_head + 1) % m_size;
	return data;
}
