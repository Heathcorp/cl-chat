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
	size_t usrsize = getline(&msg->sender, &n, stdin) - 1;
	msg->sender[usrsize] = '\0';
	send_register(sockfd, msg->sender, usrsize);

	puts("Enter the user you want to talk to:");
	usrsize = getline(&msg->target, &n, stdin) - 1;
	msg->target[usrsize] = '\0';

	puts("---CONVERSATION BEGIN---");
	for(;;) {
		size_t msgsize = getline(&msg->contents, &n, stdin) - 1;
		msg->contents[msgsize] = '\0';

		send_message(sockfd, msg->sender, 5, msg->target, 5, msg->contents, msgsize);
	}

	msg_free(msg);
}
