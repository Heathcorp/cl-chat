#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

struct vector {
	size_t element_size;
	size_t length;

	size_t allocated;
	size_t used;
	void* data;
};

struct vector* vector_init(size_t element_size);

int vector_push_back(struct vector* vector, void* element);

void* vector_get(struct vector* vector, size_t index);

#endif