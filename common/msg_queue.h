#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include "vector.h"
#include "utils.h"

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

struct message {

};

struct message* msg_create();
int msg_free(struct message* msg);

// thread safe queue for message structs
struct msg_queue {
	
};

int msg_queue_create(struct msg_queue* queue);
int msg_queue_free(struct msg_queue* queue);

int msg_queue_push(struct message* msg);
int msg_queue_pop(struct message* msg);

#endif
