#include <pthread.h>

#include "threads.c"
#include "connection.c"

int main(int argc, char *argv[])
{
	int socket_desc;
	if (argc > 1) {
		socket_desc = create_client(argv[1]);
	} else {
		socket_desc = create_server();
	}

	puts("Connection created");

	pthread_t thread_recv, thread_send;
	if (pthread_create(&thread_recv, NULL, recv_loop, &socket_desc)) {
		perror("Failed to create receive thread");
	}
	if (pthread_create(&thread_send, NULL, send_loop, &socket_desc)) {
		perror("Failed to create send thread");
	}

	pthread_join(thread_recv, NULL);
	pthread_join(thread_send, NULL);

	puts("Closing chat");
	close(socket_desc);
}