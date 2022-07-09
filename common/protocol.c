#include "protocol.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>

int protocol_command(int sockfd, char code, void* contents, size_t bufsize) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

    // TODO: actually send the message
}