#ifndef SOCKET_BUFFER_H
#define SOCKET_BUFFER_H

#include "vector.h"

#include <stddef.h>

// defines a buffer to take transmissions from a socket
struct trans_buffer {
	void* data;
};

struct trans_buffer* trans_buffer_init();

int trans_buffer_read(int sockfd);

#endif