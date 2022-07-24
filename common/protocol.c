#include "utils.h"
#include "protocol.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int protocol_command(int sockfd, char code, void* contents, size_t bufsize) {
	time_t ms = millis();

	// size of final buffer to write to the socket
	size_t n = 11 + bufsize + 2;

	char* buf = malloc(n);

	buf[0] = code;
	buf[1] = '\n';
	// write the timestamp, TODO: switch this to the string representation for portability?
	// TODO: definitely switch to hex or similar encoding
	memcpy(buf + 2, &ms, 8);
	buf[10] = '\n';

	// write the contents
	memcpy(buf + 11, contents, bufsize);

	buf[n - 2] = '\n';
	buf[n - 1] = EOT;

	// debug print:
	hexdump((void*)buf, n, 4);

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
	size_t n = bufsize + 2;
	char* buf = malloc(n);

	// write the username length
	buf[0] = (char)bufsize;
	buf[1] = '\n';
	// add the username
	memcpy(buf + 2, username, bufsize);

	int ret = protocol_command(sockfd, 'R', buf, n);
	free(buf);
	return ret;
}

int send_message(int sockfd, char* targetusr, size_t usrsize, char* message, size_t msgsize) {
	size_t n = usrsize + 3 + msgsize;
	char* buf = malloc(n);

	// copy username
	memcpy(buf, targetusr, usrsize);
	buf[usrsize] = '\n';
	// write message size
	buf[usrsize + 1] = (char)msgsize;
	buf[usrsize + 2] = '\n';
	// copy the message
	memcpy(buf + usrsize + 3, message, msgsize);

	int ret = protocol_command(sockfd, 'M', buf, n);
	free(buf);
	return ret;
}

int send_forward(int sockfd, time_t timestamp, char* sender, size_t sendersize, char* target, size_t targetsize, char* message, size_t msgsize) {
	size_t n = sendersize + targetsize + msgsize + 13;
	char* buf = malloc(n);

	int i = 0;
	// copy original sender timestamp
	// TODO: fix this for portability
	memcpy(buf, &timestamp, 8);
	i += 8;
	buf[i++] = '\n';
	// write target
	memcpy(buf + i, target, targetsize);
	i += targetsize;
	buf[i++] = '\n';
	// write sender
	memcpy(buf + i, sender, sendersize);
	i += sendersize;
	buf[i++] = '\n';
	// write message
	buf[i++] = (char)msgsize;
	buf[i++] = '\n';
	memcpy(buf + i, message, msgsize);

	int ret = protocol_command(sockfd, 'F', buf, n);
	free(buf);
	return ret;
}

struct message_t* create_message() {
	return calloc(sizeof(struct message_t), 1);
}

void free_message(struct message_t* msg) {
	free(msg->sender_user);
	free(msg->target_user);
	free(msg->contents);
	free(msg);
}

int read_message(int sockfd, struct message_t* msg) {
	char* buf = malloc(100);
	char msg_type = COMMS_DEBUG;

	size_t r = recv(sockfd, buf, 100, 0);
	printf("READ %ld BYTES FROM SOCKET %d\n", r, sockfd);
	hexdump(buf, r, 8);

	// extract the message type char and the timestamp (this segfaults)
	msg_type = buf[0];
	memcpy(&msg->sent_time, buf + 2, 8);

	printf("RECEIVING %c MESSAGE\n", msg_type);
	printf("SENT AT %ld\n", msg->sent_time);

	switch(msg_type) {
		case COMMS_MESSAGE:

			break;
		case COMMS_REGISTER:
			break;
		case COMMS_DISCONNECT:
			break;
		case COMMS_DEBUG:
		default:
			puts("CONNECTION ERROR");
			break;
	}

	free(buf);
}
