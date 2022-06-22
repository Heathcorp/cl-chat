#include "../common/vector.h"
#include "handlers.c"

#include <pthread.h>
#include <poll.h>
#include <stdio.h>

#include <sys/socket.h>
#include <arpa/inet.h>

struct thread_config {
	int socket;
	// etc
};

void* thread_routine(void* config) {
	int sockfd = ((struct thread_config*)config)->socket;

	int thread_connection_count = 0;

	while (1) {
		int client_socket = accept(sockfd, NULL, NULL);
		printf("New connection: fd:%d; count:%d\n", client_socket, ++thread_connection_count);
		handle_connection(client_socket);
	}
}

int main(int argc, char *argv[]) {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(3030);

	bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	listen(sockfd, 300);

	struct Vector* threads = vector_init(sizeof(pthread_t));

	for (int i = 0; i < 300; i++) {
		struct thread_config conf;
		conf.socket = sockfd;

		pthread_t thread;
		pthread_create(&thread, NULL, thread_routine, &conf);

		vector_push_back(threads, &thread);
	}

	for (int i = 0; i < threads->length; i++) {
		pthread_t thread = *(pthread_t*)vector_get(threads, i);

		pthread_join(thread, NULL);
	}
}