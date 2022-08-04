#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

struct vector {
	size_t element_size;
	size_t length;
	// note: allocated = capacity * element_size
	size_t capacity;

	size_t allocated;
	size_t used;
	void* data;
};

struct vector* vector_init(size_t element_size);
int vector_free(struct vector* vec);

int vector_push_back(struct vector* vec, void* element);

void* vector_get(struct vector* vec, size_t index);
int vector_set(struct vector* vec, size_t index, void* element);

int vector_resize(struct vector* vec, size_t capacity);

#endif