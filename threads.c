#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>

struct chat_message {
    size_t size;
    char* content;
};

void* recv_loop(int* socket_desc) {
    puts("Receive thread started");
    int sockfd = *socket_desc;

    struct chat_message msg;
    while (recv(sockfd, &msg.size, sizeof(msg.size), 0) > 0) {
        // printf("Receiving %d bytes\n", msg.size);

        // allocate enough for the incoming message + null terminator
        msg.content = malloc(msg.size + 1);
        recv(sockfd, msg.content, msg.size, 0);
        // add the null terminator
        msg.content[msg.size] = '\0';

        printf("%s\n", msg.content);

        free(msg.content);
        msg.content = NULL;
        msg.size = 0;
    }
}

void* send_loop(int* socket_desc) {
    puts("Send thread started");
    int sockfd = *socket_desc;

    struct chat_message msg;
    size_t n;
    while ((msg.size = getline(&msg.content, &n, stdin)) != -1) {
        // printf("Sending %d bytes\n", msg.size);

        // strip off newline the dodgy way:
        msg.size--;

        // send size of message
        send(sockfd, &msg.size, sizeof(msg.size), 0);
        // send message content
        send(sockfd, msg.content, msg.size, 0);

        free(msg.content);
        msg.content = NULL;
        msg.size = 0;
        n = 0;
    }
}