struct thread_config {
    bool running;
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

    while (conf->running) {
        if (recv(sockfd, &msgType, sizeof(msgType), 0) < 1) {conf->running = FALSE; break;}
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
            case M_CONNECTION:
                if (strcmp(msg.content, "EXIT") == 0) {
                    conf->running = FALSE;
                    logmsg(sys, "Other user disconnected, press enter to close");
                }
        }


        free(msg.content);
        msg.content = NULL;
        msg.size = 0;
    }
    
    close(sockfd);
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
    msg.user = me;

    // send my username
    msg.content = me->username;
    msg.size = strlen(me->username);
    sendmessage(sockfd, M_NAME, &msg);

    msg.content = NULL;
    msg.size = 0;

    size_t n = 0;
    while (conf->running && ((msg.size = getline(&msg.content, &n, stdin)) != -1) ) {
        // printf("Sending %d bytes\n", msg.size);

        // replace newline with null terminator
        msg.content[--msg.size] = '\0';

        if (strcmp(":q", msg.content) == 0) {
            conf->running = FALSE;
            logmsg(sys, "Received exit command");

            struct chat_message exit_msg;
            exit_msg.content = "EXIT";
            exit_msg.size = strlen(exit_msg.content);
            sendmessage(sockfd, M_CONNECTION, &exit_msg);
        } else {
            sendmessage(sockfd, M_MESSAGE, &msg);
        }

        free(msg.content);
        msg.content = NULL;
        msg.size = 0;
        n = 0;
    }

    close(sockfd);
}