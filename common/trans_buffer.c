#include "trans_buffer.h"
#include "vector.h"
#include "utils.h"

#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <stdbool.h>
#include <stdio.h>

struct trans_buffer* trans_buffer_init(int sockfd) {
	struct trans_buffer* obj = malloc(sizeof(struct trans_buffer));

	obj->sockfd = sockfd;

	obj->allocated = TRANS_BUFFER_SIZE;
	obj->data = malloc(obj->allocated);
	obj->length = 0;
	obj->readLength = 0;
	obj->containsUnread = FALSE;

	return obj;
}

int trans_buffer_free(struct trans_buffer* obj) {
	free(obj->data);
	free(obj);
}

// copies from src to dest until and including an EOT byte (0x04)
size_t copyUntilEOT(char* dest, char* src, size_t max) {
	// true if no EOT has been found yet
	bool t = TRUE;

	size_t i;
	for(i = 0; i < max && t; i++) {
		dest[i] = src[i];

		t = src[i] != EOT;
	}

	return i;
}

int trans_buffer_recv(struct trans_buffer* obj, struct vector* vec) {
	if(!obj->containsUnread) {
		// TODO: this whole thing is dodgy
		// need to wait until there's data in the socket
		// TODO: select is geriatric, redesign program and algorithm and switch to poll
		fd_set set;
		FD_ZERO(&set);
		FD_SET(obj->sockfd, &set);

		if(select(FD_SETSIZE, &set, NULL, NULL, NULL) == -1) {
			return -1;	
		}
		// really bad here: I haven't even checked the socket using FD_ISSET

		// read in data from the socket into the buffer
		// read in the max (not sure whether good practice)
		size_t recved = recv(obj->sockfd, obj->data, obj->allocated, 0);

		// TODO: handle errors (recved = -1)
		obj->length = recved;
		obj->containsUnread = TRUE;
		obj->readLength = 0;
		// printf("Read %ld bytes from fd %d\n", recved, obj->sockfd);
	}

	// read the unread data from the buffer
	size_t unreadSize = obj->length - obj->readLength;
	void* unreadStart = obj->data + obj->readLength;
	// resize the vector to fit the unread data in the buffer
	vector_resize(vec, unreadSize);

	// copy the unread data from the buffer until it finds an EOT byte
	size_t copied = copyUntilEOT(vec->data, unreadStart, unreadSize);
	vec->used = copied;
	vec->length = copied;

	obj->readLength += copied;

	// printf("Copied %ld into vector buffer of %ld in transmission buffer.\n", copied, unreadSize);

	if(obj->readLength >= obj->length) {
		// buffer is empty, only one transmission was 'waiting' in the socket
		// TODO: handle the following case
		// currently this shouldn't happen because the messages I'm sending are really small
		// check if the transmission isn't completed and needs recving
		// bool fully_read = *(char*)(vec->data + copied - 1) == EOT;

		obj->containsUnread = FALSE;
	}
}
