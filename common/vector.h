#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

struct vector {
	// size of each element in bytes
	size_t element_size;
	// number of elements currently in the vector
	size_t length;
	// capacity = allocated / element_size
	size_t capacity;

	// number of bytes allocated to the vector
	size_t allocated;
	// used = length * element_size
	size_t used;

	void* data;
};

struct vector* vector_init(size_t element_size);
int vector_free(struct vector* vec);

int vector_push_back(struct vector* vec, void* element);
int vector_delete_back(struct vector* vec);

void* vector_get(struct vector* vec, size_t index);
int vector_set(struct vector* vec, size_t index, void* element);

int vector_resize(struct vector* vec, size_t capacity);

#endif