#include <stdio.h>
#include <time.h>

#include <sys/socket.h>
#include <arpa/inet.h>

struct chat_message {
    size_t size;
    char* content;
};

void* recv_loop(int* socket_desc) {
    int sockfd = *socket_desc;

    struct chat_message msg;
    while (recv(sockfd, &msg.size, sizeof(msg.size), 0) > 0) {
        printf("Receiving %d bytes\n", msg.size);

        // allocate enough for the incoming message + null terminator
        msg.content = (char*)malloc((size_t)msg.size + 1);
        recv(sockfd, &msg.content, msg.size, 0);
        // add the null terminator
        *(&msg.content + msg.size) = '\0';

        puts(&msg.content);

        free(msg.content);
    }
}

void* send_loop(int* socket_desc) {
    int sockfd = *socket_desc;
    while (1) {
        // receive command line messages from the user
        // send it through the socket
        printf("send loop process, test num: %d\n", *socket_desc);
        break;
    }
}