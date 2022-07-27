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
	size_t n = bufsize + 3;
	char* buf = malloc(n);

	// write the username length
	write_hex_byte((char)bufsize, buf);
	buf[2] = '\n';
	// add the username
	memcpy(buf + 3, username, bufsize);

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

struct message_t* create_message() {
	return calloc(sizeof(struct message_t), 1);
}

void free_message(struct message_t* msg) {
	free(msg->sender_user);
	free(msg->target_user);
	free(msg->contents);
	free(msg);
}

int recv_message(struct trans_buffer* trans_buf, struct message_t* msg) {
	struct vector* vec = vector_init(1);
	char msg_type = COMMS_DEBUG;

	trans_buffer_read(trans_buf, vec);
	printf("READ %ld BYTES FROM SOCKET\n", vec->length);
	hexdump(vec->data, vec->used, 8);

	msg_type = *(char*)(vec->data + 0);
	msg->sent_time = get_timestamp(vec->data + 2);

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
}
