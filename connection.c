#include <sys/socket.h>
#include <arpa/inet.h>

int create_client(char* address) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        puts("Socket creation failed");
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(address);
    servaddr.sin_port = htons(8888);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        puts("Socket connect failed");
    }

    return sockfd;
}

int create_server() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        puts("Socket creation failed");
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8888);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        puts("Socket bind failed");
    }

    if (listen(sockfd, 1) != 0) {
        puts("Socket listen failed");
    }

    int client_socket = accept(sockfd, NULL, NULL);
    if (client_socket == -1) {
        puts("Socket accept failed");
    }

    return client_socket;
}