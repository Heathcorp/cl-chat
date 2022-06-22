#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
	for (int i = 0; i < 200; i++) {
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);

		struct sockaddr_in servaddr;
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		servaddr.sin_port = htons(3030);

		printf("Attempting connection %d\n", i);
		connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

		close(sockfd);
	}
}