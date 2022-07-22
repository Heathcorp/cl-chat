#ifndef TRANS_BUFFER_H
#define TRANS_BUFFER_H

#define TRANS_BUFFER_SIZE 4096

#include <stddef.h>

// defines a buffer to take transmissions from a socket
struct trans_buffer {
	void* data;
	size_t length;

	// marks the length of the already read transmission(s) in the buffer
	// data[readLength] refers to the first byte of the next unread transmission
	size_t readLength;
	// true if there is an unread transmission in the buffer
	int containsUnread;
};

struct trans_buffer* trans_buffer_init();
int trans_buffer_free(struct trans_buffer* obj);

int trans_buffer_read(int sockfd);

#endif