#include "msg_queue.h"
#include "utils.h"

#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

struct message* msg_init() {
	struct message* msg = malloc(sizeof(struct message));
	msg->sender = NULL;
	msg->target = NULL;
	msg->contents = NULL;
	return msg;
}

int msg_free(struct message* msg) {
	free(msg->sender);
	free(msg->target);
	free(msg->contents);
	free(msg);
}

struct msg_queue* msg_queue_init() {
	struct msg_queue* queue = malloc(sizeof(struct msg_queue));
	queue->vec = vector_init(sizeof(struct message*));
	queue->front_index = 0;
	queue->length = 0;
	
	pthread_mutex_init(&queue->lock, NULL);

	return queue;
}

int msg_queue_free(struct msg_queue* queue) {
	// TODO: ref counting? semaphores?
	while(queue->length--) {
		size_t vec_index = queue->front_index++ % queue->vec->capacity;
		struct message* msg_ptr = *(struct message**)vector_get(queue->vec, vec_index);
		free(msg_ptr);
	}

	pthread_mutex_destroy(&queue->lock);

	vector_free(queue->vec);
	free(queue);
}

int msg_queue_enqueue(struct msg_queue* queue, struct message* msg) {

	pthread_mutex_lock(&queue->lock);

	if(queue->length == queue->vec->capacity) {
		// more in the queue than can fit, need to resize the vector
		// simply copy all items before the front item to the back of the vector
		// then delete the old ones, this will leave empty spaces at the front of the vector
		// TODO: maybe think of a better algorithm for this
		for(size_t i = 0; i < queue->front_index; i++) {
			// copy the pointers from the start of the vector to the end
			vector_push_back(queue->vec, vector_get(queue->vec, i));
		}
	}

	size_t back_index = (queue->front_index + queue->length) % queue->vec->capacity;
	vector_set(queue->vec, back_index, &msg);
	queue->length++;

	pthread_mutex_unlock(&queue->lock);
}

int msg_queue_dequeue(struct msg_queue* queue, struct message* msg) {

	pthread_mutex_lock(&queue->lock);

	if(queue->length < 1) {
		return -1;
	}

	struct message** src = vector_get(queue->vec, queue->front_index);
	memcpy(msg, *src, sizeof(struct message));

	if(queue->length > 1) {
		queue->front_index = (queue->front_index + 1) % queue->vec->capacity;
	}
	queue->length--;

	pthread_mutex_unlock(&queue->lock);
}
