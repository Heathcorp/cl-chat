#include "handlers.h"
#include "../common/protocol.h"

#include <stdio.h>

void* recv_routine(void* conf) {

}

void* send_routine(void* conf) {
	int sockfd = ((struct thread_config*)conf)->sockfd;
	char* buffer = NULL;
	size_t n = 0;
	size_t usrsize = getline(&buffer, &n, stdin);
	send_register(sockfd, buffer, usrsize - 1);
	free(buffer);
}
