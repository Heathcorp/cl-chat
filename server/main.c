#include "../vector.h"

#include <unistd.h>
#include <poll.h>
#include <stdio.h>

void* handle_connection(void* config) {

}

int main(int argc, char *argv[]) {
	struct Vector* vector;

	vector = vector_init(sizeof(int));

	int newElement = 7;
	vector_push_back(vector, &newElement);
	newElement = 25634;
	vector_push_back(vector, &newElement);
	newElement = 423;
	vector_push_back(vector, &newElement);
	newElement = 1234;
	vector_push_back(vector, &newElement);
	newElement = 2345;
	vector_push_back(vector, &newElement);
	newElement = 4576;
	vector_push_back(vector, &newElement);

	for (int i = 0; i < vector->length; i++) {
		int* element = (int*)(vector_get(vector, i));
		printf("element %d: %d\n", i, *element);
	}
}