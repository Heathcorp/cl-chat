#include "../vector.h"

#include <pthread.h>
#include <poll.h>
#include <stdio.h>

#include <sys/socket.h>
#include <arpa/inet.h>

struct thread_config {
	int socket;
	// etc
};

void* handle_connection(void* config) {
	int sockfd = ((struct thread_config*)config)->socket;
	printf("socket connected to client, client fd: %d\n", sockfd);
}

int main(int argc, char *argv[]) {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(3030);

	bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	listen(sockfd, 1000);

	for (int i = 0; i < 1000; i++) {
		int client_socket = accept(sockfd, NULL, NULL);

		struct thread_config conf;
		conf.socket = client_socket;

		pthread_t thread;
		pthread_create(&thread, NULL, handle_connection, &conf);
	}
}