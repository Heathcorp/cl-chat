#include "../common/vector.h"
#include "../common/protocol.h"
#include "../common/utils.h"
#include "../common/hashtable.h"
#include "../common/msg_queue.h"
#include "handlers.h"

#include <pthread.h>
#include <poll.h>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>

#include <pthread.h>

#define HASHTABLE_CAPACITY	10000
struct hashtable* user_table;
pthread_mutex_t uq_mutex;

int main(int argc, char *argv[]) {
	puts("STARTING...");

	user_table = hashtable_init(sizeof(struct msg_queue), HASHTABLE_CAPACITY);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(3030);

	bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	listen(sockfd, 300);

	struct vector* threads = vector_init(sizeof(pthread_t));

	for(int i = 0; i < 300; i++) {
		struct thread_config conf;
		conf.socket = sockfd;

		pthread_t thread;
		pthread_create(&thread, NULL, thread_routine, &conf);

		vector_push_back(threads, &thread);
	}

	for(int i = 0; i < threads->length; i++) {
		pthread_t* thread = vector_get(threads, i);

		pthread_join(*thread, NULL);
	}

	hashtable_free(user_table);
}