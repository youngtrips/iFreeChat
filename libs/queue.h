/*
 * Author: youngtrips
 * Created Time:  2011-01-25
 * File Name: queue.h
 * Description: 
 */

#ifndef __QUEUE_H
#define __QUEUE_H

#include <stddef.h>
#include <pthread.h>

class queue {
	public:
		queue();
		queue(size_t size);
		~queue() {
			delete []m_data;
			pthread_rwlock_destroy(&m_rwlock);
		}
		bool empty();
		bool push(void *data);
		void *pop();

	private:
		size_t m_size;
		void **m_data;
		size_t m_head;
		size_t m_tail;
		pthread_rwlock_t m_rwlock;
};

#endif
