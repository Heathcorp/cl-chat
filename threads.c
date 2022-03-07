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
    char msgType;

    while (recv(sockfd, &msgType, sizeof(msgType), 0) > 0) {
        recvcontent(sockfd, &msg);

        switch (msgType) {
            case M_NAME:
                other->username = msg.content;
                // avoiding freeing the memory here as I want it to persist
                msg.content = NULL;
                break;
            case M_MESSAGE:
                logmsg(other, msg.content);
                break;
        }


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

    struct chat_message msg;

    // send my username
    msg.content = me->username;
    msg.size = strlen(me->username);
    sendmessage(sockfd, M_NAME, &msg);

    msg.content = NULL;
    msg.size = 0;

    size_t n = 0;
    while ((msg.size = getline(&msg.content, &n, stdin)) != -1) {
        // printf("Sending %d bytes\n", msg.size);

        // strip off newline the dodgy way:
        msg.size--;

        sendmessage(sockfd, M_MESSAGE, &msg);

        free(msg.content);
        msg.content = NULL;
        msg.size = 0;
        n = 0;
    }
}