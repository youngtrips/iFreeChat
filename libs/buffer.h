/*
 * Author: youngtrips
 * Created Time:  Wed 12 Jan 2011 01:26:37 PM CST
 * File Name: buffer.h
 * Description: 
 */

#ifndef __BUFFER_H
#define __BUFFER_H

#include <sys/types.h>
#include <string.h>

class buffer {
	public:
		buffer(size_t maxn);
		~buffer();

		ssize_t read(void *data, size_t size);
		ssize_t soft_read(void *data, size_t size);
		ssize_t write(const void *data, size_t size);
		void *push(const void *data, size_t size);
		void pop(size_t size);
		const void *get_data() { return (void*)(m_data + m_rpos); }
		size_t get_size() { return m_size; }
		void clean();

	private:
		char *m_data;

		size_t m_rpos;
		size_t m_wpos; 
		size_t m_size;
		size_t m_maxn;
};

#endif
