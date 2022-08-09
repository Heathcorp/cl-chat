#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "trans_buffer.h"

#include <sys/time.h>
#include <stddef.h>

// TODO: change code to an enum, actually implement all these functions
int protocol_command(int sockfd, char code, void* contents, size_t bufsize);


int send_register(int sockfd, char* username, size_t bufsize);

int send_message(int sockfd, char* targetusr, size_t usrsize, char* message, size_t msgsize);
// TODO: figure out timestamping, possibly make a message object for server side stuff
int send_forward(int sockfd, time_t timestamp, char* sender, size_t sendersize, char* target, size_t targetsize, char* message, size_t msgsize);
// TODO: make disconnect reasons an enum
int send_disconnect(int sockfd, char reason);


int parse_command(char* buf, size_t n);

#endif