#ifndef HANDLERS_H
#define HANDLERS_H

int handle_connection(int sockfd);


struct thread_config {
	int socket;
	// etc
};

void* thread_routine(void* config);

#endif