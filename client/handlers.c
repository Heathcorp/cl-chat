#include "handlers.h"
#include "../common/protocol.h"

#include <stdio.h>

void* recv_routine(void* conf) {

}

void* send_routine(void* conf) {
	int sockfd = ((struct thread_config*)conf)->sockfd;
	// printf("Started send thread: sockfd = %d\n", sockfd);

	printf("Please type your username:\n");
	char* buffer = NULL;
	size_t n = 0;
	size_t usrsize = getline(&buffer, &n, stdin);
	send_register(sockfd, buffer, usrsize - 1);
	free(buffer);

	for(;;) {
		buffer = NULL;
		size_t msg_len = getline(&buffer, &n, stdin);

		send_register(sockfd, buffer, msg_len - 1);

		free(buffer);
	}
}
