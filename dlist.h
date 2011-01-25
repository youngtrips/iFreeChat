/*
 * Author: youngtrips
 * Created Time:  Mon 10 Jan 2011 05:06:30 PM CST
 * File Name: dlist.h
 * Description: 
 */

#ifndef __DLIST_H
#define __DLIST_H

#include <stddef.h>
#include <stdlib.h>

class DList {
	public:
		DList() : m_prev(this), m_next(this) {}
		virtual ~DList() {}

		void Link(DList &node) {
			node.SetPrev(m_next->GetPrev());
			node.SetNext(m_next);

			m_next->SetPrev(&node);
			m_next = &node;
		}
		void UnLink() {
			m_prev->SetNext(m_next);
			m_next->SetPrev(m_prev);
		}

		void ReLink() {
			m_prev->SetNext(this); m_next->SetPrev(this); }
		void SetPrev(DList *e) {
			m_prev = e;
		}
		void SetNext(DList *e) {
			m_next = e;
		}

		DList *GetPrev() {
			return m_prev;
		}
		DList *GetNext() {
			return m_next;
		}
	private:
		DList *m_prev;
		DList *m_next;
};

#endif

