/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: dlist.h
 * Description: 
 *
 */

#ifndef __DLIST_H
#define __DLIST_H

/* modified from linux/list.h */
#include <stddef.h>

struct dlist_t {
	struct dlist_t *prev;
	struct dlist_t *next;
};


static inline void init_dlist_node(struct dlist_t *node) {
	node->next = node;
	node->prev = node;
}

static inline void __dlist_add(struct dlist_t *node,
		struct dlist_t *prev, struct dlist_t *next) {
	next->prev = node;
	node->next = next;
	node->prev = prev;
	prev->next = node;
}

static inline void dlist_add(struct dlist_t *node, struct dlist_t *head) {
	__dlist_add(node, head, head->next);
}

static inline void dlist_add_tail(struct dlist_t *node, struct dlist_t *head) {
	__dlist_add(node, head->prev, head);
}

static inline void __dlist_del(struct dlist_t *prev, struct dlist_t *next) {
	next->prev = prev;
	prev->next = next;
}


static inline void dlist_del(struct dlist_t *node) {
	__dlist_del(node->prev, node->next);
	node->next = node;
	node->prev = node;
}


#define dlist_entry(ptr, type, memb) \
	(((char*)ptr) - offsetof(type, memb))

#define dlist_foreach(pos, head) \
	for(pos = (head)->next;pos != (head); pos = pos->next)

#define dlist_foreach_safe(pos, n, head) \
	for(pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next)


#endif
