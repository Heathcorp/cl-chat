#include "handlers.h"

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
    return 0;
}