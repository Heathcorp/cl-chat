#include "handlers.h"
#include "../common/protocol.h"
#include "../common/trans_buffer.h"
#include "../common/vector.h"
#include "../common/utils.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>

void* thread_routine(void* config) {
	int sockfd = ((struct thread_config*)config)->socket;

	int thread_connection_count = 0;

	while (1) {
		int client_socket = accept(sockfd, NULL, NULL);
		printf("New connection: fd:%d; count:%d\n", client_socket, ++thread_connection_count);
		handle_connection(client_socket);
	}
}

int handle_connection(int sockfd) {
	// TODO: put this in its own recv thread
	struct vector* vec = vector_init(1);
	struct trans_buffer* trans_buf = trans_buffer_init(sockfd);

	trans_buffer_recv(trans_buf, vec);

	// handle the message
	char msg_type = *(char*)vector_get(vec, 0);

	switch(msg_type) {
		case COMMS_MESSAGE:
			
			break;
		case COMMS_REGISTER:
			break;
		case COMMS_DISCONNECT:
			break;
		case COMMS_DEBUG:
		default:
			puts("CONNECTION ERROR");
			break;
	}

	trans_buffer_free(trans_buf);
	vector_free(vec);
	return 0;
}