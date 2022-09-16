#include "hashtable.h"

#include <stdlib.h>
#include <string.h>

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

ht_key ht_hash(struct hashtable* ht, void* element) {
	int hash = 0;
	for(int i = 0; i < ht->element_size; i++) {
		hash += (char*)element[i];
	}
	return hash % ht->capacity;
}

void* hashtable_get(struct hashtable* ht, ht_key key) {
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

int hashtable_set(struct hashtable* ht, ht_key key, void* element) {
	if(ht->count >= ht->capacity) return -1;

	int hash = key;
	void* space;
	int element_key;

	do {
		hash = hash % ht->capacity;
		space = ht->data + (hash * ht->unit_size);
		element_key = *(int*)space;
		// do this until it finds an empty spot or this key already
	} while(element_key && element_key != key);

	*(int*)space = key;
	// advanced the pointer to the space where the element is stored
	space += sizeof(ht_key);
	// TODO: check for errors and stuff
	memcpy(space, element, ht->element_size);

	return 0;
}

int hashtable_delete(struct hashtable* ht, int key) {
	if(ht->count >= ht->capacity) return -1;

	int hash = key;
	void* space;
	int element_key;

	do {
		hash = hash % ht->capacity;
		space = ht->data + (hash * ht->unit_size);
		element_key = *(int*)space;
		// do this until it finds an empty spot or this key already

		if(!element_key) {
			// key not in the table
			return -1;
		}
	} while(element_key != key);

	// TODO: check for errors and stuff
	memset(space, 0, ht->unit_size);

	return 0;
}
