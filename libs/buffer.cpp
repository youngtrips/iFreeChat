/*
 * Author: youngtrips
 * Created Time:  Wed 12 Jan 2011 05:12:41 PM CST
 * File Name: buffer.cpp
 * Description: 
 */
#include "buffer.h"

buffer::buffer(size_t maxn) : m_data(new char[maxn]),
m_rpos(0),
m_wpos(0),
m_size(0),
m_maxn(maxn) {
	memset(m_data, 0, maxn);
}

buffer::~buffer() {
	delete[] m_data;
}

ssize_t buffer::read(void *data, size_t size) {
	if (m_size < size) {
		return -1;
	}
	memcpy(data, m_data + m_rpos, size);
	if (m_rpos + size > m_maxn) {
		int l = m_maxn - m_rpos;
		m_rpos = size - l;
	} else {
		m_rpos += size;
	}
	m_size -= size;
	return size;
}

ssize_t buffer::soft_read(void *data, size_t size) {
	if (m_size < size) {
		return -1;
	}
	memcpy(data, m_data + m_rpos, size);
	return size;
}

ssize_t buffer::write(const void *data, size_t size) {
	if ( m_size + size > m_maxn) {
		return -1;
	}
	if (m_wpos + size > m_maxn) {
		size_t l = m_maxn - m_wpos;
		memcpy(m_data + m_wpos, data, size);
		m_wpos = size - l;
	} else {
		memcpy(m_data + m_wpos, data, size);
		memcpy(m_data + m_maxn + m_wpos, data, size);
		m_wpos += size;
	}
	m_size += size;
	return size;
}

void buffer::clean() {
	memset(m_data, 0, m_maxn);
	m_rpos = 0;
	m_wpos = 0;
	m_size = 0;
}
