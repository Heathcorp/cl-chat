#include "protocol.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

time_t millis() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

int protocol_command(int sockfd, char code, void* contents, size_t bufsize) {
	time_t ms = millis();

	// size of final buffer to write to the socket
	size_t n = 11 + bufsize + 1;

	char* buf = malloc(n);

	buf[0] = code;
	buf[1] = '\n';
	// write the timestamp, TODO: switch this to the string representation for portability?
	memcpy(buf + 2, &ms, 8);
	buf[10] = '\n';

	// write the contents
	memcpy(buf + 11, contents, bufsize);

	buf[n - 1] = '\n';

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
