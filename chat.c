#include <unistd.h>
#include <pthread.h>

#include "utils.c"
#include "threads.c"
#include "connection.c"

int main(int argc, char *argv[])
{
	struct user_data me, sys, other;
	me.username = argv[1];
	sys.username = "SYSTEM";
	other.username = "TEST!";

	struct thread_config conf;
	conf.user_me = &me;
	conf.user_sys = &sys;
	conf.user_other = &other;
	conf.running = TRUE;

	if (argc > 2) {
		conf.sockfd = create_client(argv[2]);
	} else {
		conf.sockfd = create_server();
	}

	logmsg(&sys, "Connection created");

	pthread_t thread_recv, thread_send;
	if (pthread_create(&thread_recv, NULL, recv_routine, &conf)) {
		perror("Failed to create receive thread");
	}
	if (pthread_create(&thread_send, NULL, send_routine, &conf)) {
		perror("Failed to create send thread");
	}

	pthread_join(thread_recv, NULL);
	pthread_join(thread_send, NULL);

	logmsg(&sys, "Closing chat");
	close(conf.sockfd);
}