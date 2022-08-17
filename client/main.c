#include "../common/protocol.h"
#include "../common/utils.h"
#include "handlers.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

int main(int argc, char* argv[]) {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(3030);

	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	struct thread_config conf;

	pthread_t recv_thread, send_thread;
	pthread_create(&recv_thread, NULL, recv_routine, &conf);
	pthread_create(&send_thread, NULL, send_routine, &conf);

	pthread_join(recv_thread, NULL);
	pthread_join(send_thread, NULL);

	close(sockfd);
}