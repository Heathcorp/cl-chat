#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct user_config {
    char* username;
};

struct chat_message {
    size_t size;
    char* content;
};

void logmsg(struct user_config* user, char* message) {
    // TODO: add proper logging and timestamps
    printf("%s: %s\n", user->username, message);
}