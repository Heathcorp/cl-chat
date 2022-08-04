#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include "vector.h"

struct message {
	char* sender;
	char* target;
	char* contents;

	// struct vector* hops;
};

struct message* msg_init();
int msg_free(struct message* msg);

// thread safe queue for message structs
struct msg_queue {
	// circular array queue
	struct vector* vec;
	size_t front_index;
	size_t length;
};

int msg_queue_init(struct msg_queue* queue);
int msg_queue_free(struct msg_queue* queue);

int msg_queue_enqueue(struct msg_queue* queue, struct message* msg);
struct message* msg_queue_dequeue(struct msg_queue* queue);

#endif
