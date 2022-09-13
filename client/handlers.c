#include "handlers.h"
#include "../common/protocol.h"
#include "../common/utils.h"
#include "../common/msg_queue.h"

#include <stdio.h>
#include <string.h>

void* recv_routine(void* conf) {

}

void* send_routine(void* conf) {
	// TODO: fix message struct so that it contains length info
	int sockfd = ((struct thread_config*)conf)->sockfd;
	// printf("Started send thread: sockfd = %d\n", sockfd);

	puts("Please type your username:");

	struct msg_t* msg = msg_init();

	size_t n = 0;
	size_t sendersize = getline(&msg->sender, &n, stdin) - 1;
	msg->sender[sendersize] = '\0';
	send_register(sockfd, msg->sender, sendersize);

	puts("Enter the user you want to talk to:");
	size_t targetsize = getline(&msg->target, &n, stdin) - 1;
	msg->target[targetsize] = '\0';

	printf("Starting conversation with \"%s\", type \"/end\" to finish.\n", msg->target);
	for(;;) {
		size_t msgsize = getline(&msg->contents, &n, stdin) - 1;
		msg->contents[msgsize] = '\0';

		if(strcmp(msg->contents, "/end") == 0) {
			break;
		}

		send_message(sockfd, msg->sender, sendersize, msg->target, targetsize, msg->contents, msgsize);
	}

	msg_free(msg);
}
