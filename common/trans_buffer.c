#include "trans_buffer.h"
#include "vector.h"
#include "utils.h"

#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

struct trans_buffer* trans_buffer_init() {
	struct trans_buffer* obj = malloc(sizeof(struct trans_buffer));

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
	int t = TRUE;

	size_t i;
	for(i = 0; i < max && t; i++) {
		dest[i] = src[i];

		t = src[i] != 0x04;
	}

	return i;
}

int trans_buffer_read(struct trans_buffer* obj, int sockfd, struct vector* vec) {
	if(obj->containsUnread) {
		size_t unreadSize = obj->length - obj->readLength;
		void* unreadStart = obj->data + obj->readLength;
		// resize the vector to fit the unread data in the buffer
		vector_resize(vec, unreadSize);
		// copy the unread data from the buffer until it finds an EOT byte
		size_t copied = copyUntilEOT(vec->data, unreadStart, unreadSize);

		// need to check if the transmission isn't completed and needs recving
	}
}
