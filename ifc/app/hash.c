#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem_pool.h"
#include "hash.h"

/* Thomas Wang's 32 bit Mix Function */
unsigned int hash_integer(unsigned int key) {

	key += ~(key << 15);
    key ^=  (key >> 10);
    key +=  (key << 3);
    key ^=  (key >> 6);
    key += ~(key << 11);
    key ^=  (key >> 16);
    return key;
}

/* Generic hash function (a popular one from Bernstein) */
unsigned int hash_string(const unsigned char *buf, int len) {

    unsigned int hash = 5381;

    while(len--)
        hash = ((hash << 5) + hash) + (*buf++); /* hash * 33 + c */
    return hash;
}

hash_t *create_hash(mem_pool_t *pool, size_t hsize, int ktype) {
	char *base;
	size_t size;
	int i;
	hash_t *h;

	size = sizeof(hash_t) + sizeof(hash_entry_t*) * (1 + hsize);
	base = (char*)mem_pool_alloc(pool, size);
	memset(base, 0, size);

	h = (hash_t*)base;	base += sizeof(hash_t);
	h->hash_entry = (hash_entry_t**)base;
	h->hash_size = hsize;
	h->ktype	 = ktype;

#ifdef USE_LOCK
	pthread_mutex_init(&(h->lock), NULL);
#endif

	return h;
}

hash_entry_t *hash_entry_alloc() {
	hash_entry_t *entry;
	entry = (hash_entry_t*)malloc(sizeof(hash_entry_t));
	init_dlist_node(&(entry->node));
	return entry;
}


void hash_entry_free(mem_pool_t *pool, hash_entry_t *entry) {
	if (entry) {
		if (entry->key)
			mem_pool_free(pool, entry->key);
		mem_pool_free(free, entry);
	}
}

void hash_entry_set_key(mem_pool_t *pool, hash_entry_t *entry, int ktype, 
		const void *key) {
	size_t size;

	if (ktype == KEY_INT) {
		size = sizeof(unsigned int);
	} else {
		size = strlen((char*)key) + 1;
	}
	entry->key = mem_pool_alloc(pool, size);
	memcpy(entry->key, key, size);
}

void hash_entry_set_val(hash_entry_t *entry, const void *val) {
	entry->val = (void*)val;
}

unsigned int hash_index(hash_t *h, const void *key) {
	unsigned int idx;

	if (h->ktype == KEY_INT) {
		idx = hash_integer(*(unsigned int*)key);
	} else {
		idx = hash_string((char*)key, strlen((char*)key));
		idx = hash_integer(idx);
	}
	return (idx & h->hash_size);
}

int hash_insert(mem_pool_t *pool, hash_t *h, const void *key, const void *val) {
	hash_entry_t *entry;
	unsigned int idx;

	entry = hash_entry_alloc();
	if (entry == NULL)
		return -1;

	hash_entry_set_key(pool, entry, h->ktype, key);
	hash_entry_set_val(entry, val);
	idx = hash_index(h, key);
	h->hash_entry[idx] = entry;
	
#ifdef USE_LOCK
	pthread_mutex_lock(&(h->lock));
#endif
	if (h->hash_entry[idx] == NULL) {
		h->hash_entry[idx] = entry;
	} else {
		dlist_add_tail(&(entry->node), &(h->hash_entry[idx]->node));
	}
#ifdef USE_LOCK
	pthread_mutex_unlock(&(h->lock));
#endif
	return 0;
}

int hash_key_equal(int ktype, const void *k1, const void *k2) {
	if (ktype == KEY_INT) {
		return *((unsigned int*)k1) == *((unsigned int*)k2);
	}
	return !strcmp((char*)k1, (char*)k2);
}

int hash_del(hash_t *h, const void *key, void **val) {
	hash_entry_t *entry;
	hash_entry_t *next;
	unsigned int idx;
	dlist_t *p;

	idx = hash_index(h, key);
	entry = h->hash_entry[idx];
	if (entry == NULL)
		return -1;
	if (hash_key_equal(h->ktype, entry->key, key)) {

#ifdef USE_LOCK
		pthread_mutex_lock(&(h->lock));
#endif
		*val = entry->val;
		hash_entry_free(pool, entry);
		h->hash_entry[idx] = NULL;
#ifdef USE_LOCK
		pthread_mutex_unlock(&(h->lock));
#endif
		return 0;
	}
	dlist_foreach(p, &(entry->node)) {
		next = (hash_entry_t*)dlist_entry(p, hash_entry_t, node);
		if (hash_key_equal(h->ktype, next->key, key)) {
#ifdef USE_LOCK
		pthread_mutex_lock(&(h->lock));
#endif
			*val = next->val;
			dlist_del(&(next->node));
			hash_entry_free(pool, next);
#ifdef USE_LOCK
		pthread_mutex_unlock(&(h->lock));
#endif
			return 0;
		}
	}
	return -1;
}

int hash_find(hash_t *h, const void *key, void **val) {
	hash_entry_t *entry;
	hash_entry_t *next;
	unsigned int idx;
	dlist_t *p;

	idx = hash_index(h, key);
	entry = h->hash_entry[idx];
	if (entry == NULL)
		return -1;
	if (hash_key_equal(h->ktype, entry->key, key)) {
		*val = entry->val;
		return 0;
	}
#ifdef USE_LOCK
	pthread_mutex_lock(&(h->lock));
#endif
	dlist_foreach(p, &(entry->node)) {
		next = (hash_entry_t*)dlist_entry(p, hash_entry_t, node);
		if (hash_key_equal(h->ktype, next->key, key)) {
			*val = next->val;
#ifdef USE_LOCK
			pthread_mutex_unlock(&(h->lock));
#endif
			return 0;
		}
	}
#ifdef USE_LOCK
	pthread_mutex_unlock(&(h->lock));
#endif
	return -1;
}

void destroy_hash(hash_t *h) {
	hash_entry_t *entry;
	hash_entry_t *next;
	dlist_t *p;
	dlist_t *q;
	int i;
	if (h == NULL)
		return;
	for(i = 0;i < h->hash_size; i++) {
		entry = h->hash_entry[i];
		if (entry) {
			dlist_foreach_safe(p, q, &(entry->node)) {
				next = (hash_entry_t*)dlist_entry(p, hash_entry_t, node);
				dlist_del(&(next->node));
				hash_entry_free(next);
			}
		}
	}
	free(h);
}


#ifdef __TEST_

typedef struct pair_t {
	char a[64];
	char b[64];
}pair_t;

pair_t test[2] = {
	{"192.168.1.3", "hello java"},
	{"192.168.1.2", "hello perl"}
};


int main() {

	hash_t *hash;
	char *val;
	int ret;

	hash = create_hash(KEY_STR, 0xffff);

	hash_insert(hash, test[0].a, test[0].b);
	hash_insert(hash, test[1].a, test[1].b);

	ret = hash_find(hash, "192.168.1.2", (void**)&val);
	printf("ret: %d\n", ret);
	if (ret == 0)
		printf("val: [%s]\n", val);

	destroy_hash(hash);
	return 0;
}


#endif
