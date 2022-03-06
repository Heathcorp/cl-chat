#include <sys/socket.h>
#include <arpa/inet.h>

struct thread_config {
    int sockfd;
    struct user_config* user;
    struct user_config* sys;
};

void* recv_routine(void* config) {    
    struct thread_config* conf = (struct thread_config*)config;
    int sockfd = conf->sockfd;
    struct user_config* user = conf->user;
    struct user_config* sys = conf->sys;

    logmsg(sys, "Receive thread started");

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

void* send_routine(void* config) {    
    struct thread_config* conf = (struct thread_config*)config;
    int sockfd = conf->sockfd;
    struct user_config* user = conf->user;
    struct user_config* sys = conf->sys;

    logmsg(sys, "Send thread started");

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