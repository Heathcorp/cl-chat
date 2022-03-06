#include <sys/socket.h>
#include <arpa/inet.h>

struct thread_config {
    int sockfd;
    struct user_data* user_other;
    struct user_data* user_me;
    struct user_data* user_sys;
};

void* recv_routine(void* config) {    
    struct thread_config* conf = (struct thread_config*)config;
    int sockfd = conf->sockfd;
    struct user_data* other = conf->user_other;
    struct user_data* me = conf->user_me;
    struct user_data* sys = conf->user_sys;

    logmsg(sys, "Receive thread started");

    struct chat_message msg;

    // // first get the other user's name
    // size_t n;
    // recv(sockfd, &n, sizeof(n), 0);
    // printf("%d\n", n);
    // other->username = malloc(n + 1);
    // printf("%d\n", n);
    // recv(sockfd, other->username, n, 0);
    // other->username[n] = '\0';
    // printf("%d\n", n);
    // printf("%s\n", other->username);

    while (recv(sockfd, &msg.size, sizeof(msg.size), 0) > 0) {
        // printf("Receiving %d bytes\n", msg.size);

        // allocate enough for the incoming message + null terminator
        msg.content = malloc(msg.size + 1);
        recv(sockfd, msg.content, msg.size, 0);
        // add the null terminator
        msg.content[msg.size] = '\0';

        logmsg(other, msg.content);

        free(msg.content);
        msg.content = NULL;
        msg.size = 0;
    }

    free(other->username);
}

void* send_routine(void* config) {    
    struct thread_config* conf = (struct thread_config*)config;
    int sockfd = conf->sockfd;
    struct user_data* other = conf->user_other;
    struct user_data* me = conf->user_me;
    struct user_data* sys = conf->user_sys;

    logmsg(sys, "Send thread started");

    // // first send my username
    // size_t n = strlen(me->username);
    // send(sockfd, &n, sizeof(n), 0);
    // send(sockfd, me->username, n, 0);

    struct chat_message msg;
    size_t n = 0;
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