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

	// parsing arguments to find the ip and/or port
	bool isServer = FALSE;
	char address[32];
	int port;
	strcpy(address, argv[2]);
    char* portStr = strchr(address, ':');

	if (isServer = (portStr == NULL)) {
		portStr = address;
	} else {
		// setting this to null seperates the address string into the ip and the port
    	portStr[0] = '\0';
		portStr++;
	}
	port = atoi(portStr);

	// if the user entered an ip and a port, then create a client and connect to the specified host
	// if the user only entered a port, then create a server listening on that port
	if (isServer) {
		conf.sockfd = create_server(port);
	} else {
		conf.sockfd = create_client(address, port);
	}

	if (conf.sockfd == -1) {
		logmsg(&sys, "Connection failed");
		return -1;
	}

	logmsg(&sys, "Connection created");

	// create the main receive and send threads
	pthread_t thread_recv, thread_send;
	if (pthread_create(&thread_recv, NULL, recv_routine, &conf)) {
		perror("Failed to create receive thread");
	}
	if (pthread_create(&thread_send, NULL, send_routine, &conf)) {
		perror("Failed to create send thread");
	}

	// return to main once the threads terminate
	pthread_join(thread_recv, NULL);
	pthread_join(thread_send, NULL);

	logmsg(&sys, "Closing chat");
	// close the socket
	close(conf.sockfd);
}