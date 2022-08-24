#include "hashtable.h"

#include <stdlib.h>

struct hashtable* hashtable_init(size_t element_size, size_t capacity) {
	struct hashtable* ht = malloc(sizeof(struct hashtable));

	size_t unit_size = sizeof(ht_key) + element_size;
	size_t ht_alloc = capacity * unit_size;

	ht->data = malloc(ht_alloc);
	// TODO: check for bad malloc returns
	ht->allocated = ht_alloc;
	ht->capacity = capacity;
	ht->element_size = element_size;
	ht->unit_size = unit_size;
	ht->count = 0;
}

int hashtable_free(struct hashtable* ht) {
	free(ht->data);
	free(ht);
}


void* hashtable_get(struct hashtable* ht, int key) {
	int hash = key;
	void* element;
	int element_key;
	do {
		// hash function is simply modulo
		hash = hash % ht->capacity;
		element = ht->data + (hash * ht->unit_size);
		element_key = *(int*)element;

		if(!element_key) {
			// found an empty slot before finding the key
			return NULL;
		}

		hash++;
	} while(element_key != key);

	// get the element assigned to that key
	return element + sizeof(ht_key);
}

int hashtable_set(struct hashtable* ht, int key, void* element) {

}

int hashtable_delete(struct hashtable* ht, int key) {

}
