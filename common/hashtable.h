#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>

// defines a linear-probing hashtable structure with integer keys
struct hashtable {
	void* data;
	size_t allocated;

	// size in bytes of individual element values
	size_t element_size;
	// allocated / (element_size + sizeof(int)): the number of elements it can fit at maximum
	size_t capacity;
	// number of elements currently set
	size_t count;
};

struct hashtable* hashtable_init(size_t element_size, size_t capacity);
int hashtable_free(struct hashtable* ht);

void* hashtable_get(struct hashtable* ht, int key);
int hashtable_set(struct hashtable* ht, int key, void* element);
int hashtable_delete(struct hashtable* ht, int key);

#endif