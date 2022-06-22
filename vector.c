#include "vector.h"

#include <string.h>
#include <stdio.h>

struct Vector* vector_init(size_t element_size) {
	struct Vector* vector = (struct Vector*)malloc(sizeof(struct Vector));

	vector->element_size = element_size;
	vector->length = 0;

	vector->used = 0;
	vector->allocated = element_size;
	vector->data = malloc(element_size);
}

int vector_push_back(struct Vector* vector, void* element) {
	if (vector->used >= vector->allocated) {
		vector->allocated *= 2;
		void* newdata = realloc(vector->data, vector->allocated);

		if (newdata) {
			vector->data = newdata;
		} else {
			return -1;
		}
	}

	memcpy(vector->data + vector->used, element, vector->element_size);

	vector->used += vector->element_size;
	vector->length++;
}

void* vector_get(struct Vector* vector, size_t index) {
	return vector->data + vector->element_size * index;
}
