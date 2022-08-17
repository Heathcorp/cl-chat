#ifndef HANDLERS_H
#define HANDLERS_H

struct thread_config {
	int sockfd;
};

void* recv_routine(void* conf);
void* send_routine(void* conf);

#endif