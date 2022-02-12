#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	int socket_desc;
	if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		puts("Failed to create socket");
		return 1;
	}

	struct sockaddr_in other;
	other.sin_addr.s_addr = inet_addr("127.0.0.1");
	other.sin_family = AF_INET;
	other.sin_port = htons(8888);

	if (connect(socket_desc, (struct sockaddr *)&other, sizeof(other)) < 0)
	{
		puts("Failed to connect");
		return 1;
	}

	char *message = "client 1 wants to chat";
	if (send(socket_desc, message, 22, 0) < 0)
	{
		puts("Failed to send");
		return 1;
	}

	return 0;
}