#include "../common/protocol.h"
#include "../common/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
	for (int i = 0; i < 1; i++) {
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);

		struct sockaddr_in servaddr;
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		servaddr.sin_port = htons(3030);

		printf("Attempting connection %d\n", i);
		connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

		send_register(sockfd, "grouchy garry guards greatly", 28);
		send_register(sockfd, "hangry henry", 12);
		send_register(sockfd, "Angry Angus", 11);
		send_register(sockfd, "Smokin Sam", 10);

		close(sockfd);
	}
}