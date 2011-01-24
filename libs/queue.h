/*
 * Author: youngtrips
 * Created Time:  2011-01-25
 * File Name: queue.h
 * Description: 
 */

#ifndef __QUEUE_H
#define __QUEUE_H

#include <stddef.h>

class Queue {
	public:
		Queue();
		Queue(size_t size);

		bool Empty();
		bool Push(void *data);
		void *Pop();

	private:
		size_t m_size;
		void **m_data;
		size_t m_head;
		size_t m_tail;
};

#endif
