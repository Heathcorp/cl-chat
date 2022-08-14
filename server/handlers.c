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

	while(1) {
		int client_socket = accept(sockfd, NULL, NULL);
		printf("New connection: fd:%d; count:%d\n", client_socket, ++thread_connection_count);
		handle_connection(client_socket);
	}
}

int handle_connection(int sockfd) {
	// TODO: put this in its own recv thread
	struct vector* vec = vector_init(1);
	struct trans_buffer* trans_buf = trans_buffer_init(sockfd);

	// receive a message into the buffer
	trans_buffer_recv(trans_buf, vec);

	// handle the message
	struct command cmd;
	if(parse_command(vec->data, vec->used, &cmd)) {
		// parse failed
		puts("Bad message received:");
		hexdump(vec->data, vec->used, 8);
		
	} else {
		// parse succeeded
		switch(cmd.type) {
			case COMMS_REGISTER:
				printf("New user \"%s\" registered.\n", cmd.sender);
				break;
			case COMMS_DISCONNECT:
				printf("User disconnected, reason: \"%s\".\n", cmd.contents);
				break;
			case COMMS_MESSAGE:
				// printf("New user registered: %s\n", cmd.contents);
				break;
			case COMMS_DEBUG:
			default:
				break;
		}
	}

	

	trans_buffer_free(trans_buf);
	vector_free(vec);
	return 0;
}