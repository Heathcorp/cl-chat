#include <time.h>

struct chat_message {
    time_t time;
    char* content;
};

void* recv_loop(int* socket_desc) {
    while (1) {
        // receive and pump output into a pipe
        // recv(socket_desc);
        // write(pipe fd)
        printf("recv loop process, test num: %d\n", *socket_desc);
        break;
    }
}

void* send_loop(int* socket_desc) {
    while (1) {
        // receive command line messages from the user
        // send it through the socket
        printf("getline loop process, test num: %d\n", *socket_desc);
        break;
    }
}