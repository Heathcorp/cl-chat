#include "utils.h"
#include "protocol.h"
#include "trans_buffer.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int protocol_command(int sockfd, char code, void* contents, size_t bufsize) {
	// size of final buffer to write to the socket
	size_t n = 19 + bufsize + 2;

	char* buf = malloc(n);

	buf[0] = code;
	buf[1] = '\n';
	// write the timestamp in hex
	write_timestamp(buf + 2, millis());
	buf[18] = '\n';

	// write the contents
	memcpy(buf + 19, contents, bufsize);

	buf[n - 2] = '\n';
	buf[n - 1] = EOT;

	// debug print:
	hexdump((void*)buf, n, 8);

	// send the message
	send(sockfd, buf, n, 0);
	
	free(buf);
}

int send_disconnect(int sockfd, char reason) {
	size_t n = 1;
	char* buf = " ";
	buf[0] = reason;

	return protocol_command(sockfd, 'Q', buf, n);
}

int send_register(int sockfd, char* username, size_t bufsize) {
	size_t n = bufsize;
	char* buf = malloc(n);
	// add the username
	memcpy(buf, username, bufsize);

	int ret = protocol_command(sockfd, 'R', buf, n);
	free(buf);
	return ret;
}

int send_message(int sockfd, char* targetusr, size_t usrsize, char* message, size_t msgsize) {
	size_t n = usrsize + 1 + msgsize;
	char* buf = malloc(n);

	// copy username
	memcpy(buf, targetusr, usrsize);
	buf[usrsize] = '\n';
	// copy the message
	memcpy(buf + usrsize + 1, message, msgsize);

	int ret = protocol_command(sockfd, 'M', buf, n);
	free(buf);
	return ret;
}

// TODO: switch this to a command parsing function, also create more command structs
int parse_command(char* buf, size_t n, struct command* cmd) {
	printf("Parsing a %ld byte transmission:\n", n);
	hexdump(buf, n, 8);

	char* token = buf;

	time_t timestamp;
	if(read_timestamp(buf + 2, &timestamp)) {
		return -1;
	}

	printf("%ld\n", timestamp);

	if(*token == COMMS_REGISTER) {

	} else if(*token == COMMS_DISCONNECT) {

	} else if(*token == COMMS_MESSAGE) {

	} else if(*token == COMMS_DEBUG) {
		hexdump(buf, n, 8);
		return -1;
	} else {
		return -1;
	}
}
