#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
	int socket_desc;
	if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		puts("Failed to create socket");
		return 1;
	}

	struct sockaddr_in server;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) 
	{
		puts("Failed to bind socket");
		return 1;
	}

	if (listen(socket_desc, 1) == -1)
	{
		puts("Listen failed");
		return 1;
	}

	int client_socket = accept(socket_desc, NULL, NULL);
	if (client_socket == -1)
	{
		puts("Accept failed");
		return 1;
	}

	char message[32];
	int recvd;
	if ((recvd = recv(client_socket, message, 32, 0)) < 0)
	{
		puts("Recieve failed");
		return 1;
	}
	printf("Recieved %d bytes\n", recvd);

	puts(message);

	return 0;
}