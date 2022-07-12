#include "protocol.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

int protocol_command(int sockfd, char code, void* contents, size_t bufsize) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	time_t ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

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
	send(sockfd, buf, n, NULL);
}

int send_disconnect(int sockfd, char reason) {
	char* buf = " ";
	buf[0] = reason;

	return protocol_command(sockfd, 'Q', buf, 1);
}
