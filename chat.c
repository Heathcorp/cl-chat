#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "loops.c"

int main(int argc, char *argv[])
{
	int socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1) {
		puts("Failed to create socket");
	}

	pthread_t thread_recv, thread_send;
	if (pthread_create(&thread_recv, NULL, recv_loop, &socket_desc)) {
		puts("Failed to create receive thread");
	}
	if (pthread_create(&thread_send, NULL, send_loop, &socket_desc)) {
		puts("Failed to create send thread");
	}

	pthread_join(thread_recv, NULL);
	pthread_join(thread_send, NULL);
}