#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

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