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
	buf[1] = CR;
	// write the timestamp in hex
	write_timestamp(buf + 2, millis());
	buf[18] = CR;

	// write the contents
	memcpy(buf + 19, contents, bufsize);

	buf[n - 2] = CR;
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
	buf[usrsize] = CR;
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

	// reset the command object
	memset(cmd, 0, sizeof(struct command));

	char* token = buf;
	
	// check the command code and timestamp and newlines
	cmd->type = *token;
	++token;
	if(*token != CR) return -1;
	++token;
	if(read_timestamp(buf, &cmd->timestamp)) return -1;
	token += 16;
	if(*token != CR) return -1;
	++token;

	printf("%c\n", cmd->type);
	printf("%ld\n", cmd->timestamp);

	// find the newline character
	char* eol = strchr(token, CR);
	size_t linelength = eol - token;

	if(cmd->type == COMMS_REGISTER) {
		if(linelength > MAX_USERNAME_LENGTH) return -1;

		memcpy(cmd->sender, token, linelength);

		token += linelength + 1;
	} else if(cmd->type == COMMS_DISCONNECT) {
		if(linelength > MAX_MESSAGE_LENGTH) return -1;

		memcpy(cmd->contents, token, linelength);

		token += linelength + 1;
	} else if(cmd->type == COMMS_MESSAGE) {
		if(linelength > MAX_USERNAME_LENGTH) return -1;
		memcpy(cmd->sender, token, linelength);
		token += linelength + 1;

		// get target username line
		eol = strchr(token, CR);
		linelength = eol - token;
		if(linelength > MAX_USERNAME_LENGTH) return -1;
		memcpy(cmd->target, token, linelength);
		token += linelength + 1;

		// get message contents line
		eol = strchr(token, CR);
		linelength = eol - token;
		if(linelength > MAX_MESSAGE_LENGTH) return -1;
		memcpy(cmd->contents, token, linelength);
		token += linelength + 1;

		// TODO: check for EOT and get hop timestamps
	} else if(cmd->type == COMMS_DEBUG) {
		hexdump(buf, n, 8);
		return -1;
	} else {
		return -1;
	}

	if(*token != EOT) return -1;
}
