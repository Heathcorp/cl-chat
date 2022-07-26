#include "trans_buffer.h"
#include "vector.h"
#include "utils.h"

#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

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

int trans_buffer_read(struct trans_buffer* obj, struct vector* vec) {
	if(!obj->containsUnread) {
		// need to read in data from the socket into the buffer
		// read in the max (not sure whether good practice)
		size_t recved = recv(obj->sockfd, obj->data, obj->allocated, 0);
		// TODO: handle errors (recved = -1)
		obj->length = recved;
		obj->containsUnread = TRUE;
		obj->readLength = 0;
	}

	// read the unread data from the buffer
	size_t unreadSize = obj->length - obj->readLength;
	void* unreadStart = obj->data + obj->readLength;
	// resize the vector to fit the unread data in the buffer
	vector_resize(vec, unreadSize);

	// copy the unread data from the buffer until it finds an EOT byte
	size_t copied = copyUntilEOT(vec->data, unreadStart, unreadSize);
	obj->readLength = copied;
	vec->used = copied;
	vec->length = copied;

	if(copied == unreadSize) {
		// buffer is empty, only one transmission was 'waiting' in the socket
		// TODO: handle the following case
		// currently this shouldn't happen because the messages I'm sending are really small
		// check if the transmission isn't completed and needs recving
		// bool fully_read = *(char*)(vec->data + copied - 1) == EOT;

		obj->containsUnread = FALSE;
	}
}
