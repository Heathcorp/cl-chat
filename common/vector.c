#include "vector.h"

#include <string.h>
#include <stdio.h>

struct vector* vector_init(size_t element_size) {
	struct vector* vec = (struct vector*)malloc(sizeof(struct vector));

	vec->element_size = element_size;
	vec->length = 0;

	vec->used = 0;
	vec->allocated = element_size;
	vec->data = malloc(element_size);
}

int vector_push_back(struct vector* vec, void* element) {
	if (vec->used >= vec->allocated) {
		vec->allocated *= 2;
		void* newdata = realloc(vec->data, vec->allocated);

		if (newdata) {
			vec->data = newdata;
		} else {
			return -1;
		}
	}

	memcpy(vec->data + vec->used, element, vec->element_size);

	vec->used += vec->element_size;
	vec->length++;
}

void* vector_get(struct vector* vec, size_t index) {
	return vec->data + vec->element_size * index;
}
