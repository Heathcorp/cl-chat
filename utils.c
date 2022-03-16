#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

typedef int bool;
#define FALSE 0
#define TRUE 1

// types of messages sent between clients
#define M_NAME 'n'
#define M_MESSAGE 'm'
#define M_CONNECTION 'c'

struct user_data {
    char* username;
};

struct chat_message {
    size_t size;
    char* content;
};

void logmsg(struct user_data* user, char* message) {
    // TODO: add proper logging and timestamps
    printf("%s: %s\n", user->username, message);
}

// receives the message length then the content
ssize_t recvcontent(int fd, struct chat_message* msg) {
    // receive size of content
    if (recv(fd, &(msg->size), sizeof(msg->size), 0) == -1) return -1;
    // allocate enough and receive the content
    msg->content = malloc(msg->size + 1);
    if (recv(fd, msg->content, msg->size, 0) == -1) return -1;
    // add the null terminator
    msg->content[msg->size] = '\0';

    return msg->size;
}

ssize_t sendmessage(int fd, char type, struct chat_message* msg) {
    // send message type
    if (send(fd, &type, sizeof(type), 0) == -1) return -1;
    // send size of message
    if (send(fd, &(msg->size), sizeof(msg->size), 0) == -1) return -1;
    // send message content
    if (send(fd, msg->content, msg->size, 0) == -1) return -1;

    return 0;
}