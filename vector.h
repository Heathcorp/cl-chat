#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

struct Vector {
	size_t element_size;
	size_t length;

	size_t allocated;
	size_t used;
	void* data;
};

struct Vector* vector_init(size_t element_size);

int vector_push_back(struct Vector* vector, void* element);

#endif