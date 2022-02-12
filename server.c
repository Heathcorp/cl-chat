#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
	int socket_desc;
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in other;
	other.sin_addr.s_addr = inet_addr("127.0.0.1");
	other.sin_family = AF_INET;
	other.sin_port = htons(8888);

	connect(socket_desc, (struct sockaddr *)&other, sizeof(other));

	char *message = "                      ";
	recv(socket_desc, message, 22, 0);

	puts(message);

	return 0;
}