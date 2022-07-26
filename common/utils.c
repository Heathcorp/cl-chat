#include "utils.h"

#include <stdio.h>
#include <string.h>

time_t millis() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

int write_timestamp(char* dest) {
	char* charset = "0123456789abcdef";
	time_t t = millis();

	for(int i = 15; i > -1; i--) {
		dest[i] = charset[t & 0b1111];
		t >>= 4;
	}
}

time_t get_timestamp(char* src) {
	time_t t = 0;

	for(int i = 60; i >= 0; i -= 4) {
		char c = *(src++);
		char hexdigit = (c >= 'a') ? (c - 87) : (c - '0');
		t += (long)hexdigit << i;
	}
	
	return t;
}

int hexdump(void* buf, size_t bufsize, size_t columns) {
	char* text = malloc(bufsize);
	memcpy(text, buf, bufsize);
	remove_ctrl(text, bufsize, ' ');

	for(size_t i = 0; i < bufsize; i += columns) {
		for(int j = 0; j < columns; j++) {
			printf(j + i < bufsize ? "%02hhX " : "   ", *(char*)(buf + i + j));
		}

		printf("| %.*s\n", (int)columns, text + i);
	}

	return 0;
}

int replace_char(char* buf, size_t bufsize, char target, char replace) {
	// heavily inspired by stackoverflow
	for(char* substr = strchr(buf, target); substr; substr = strchr(buf, target)) {
		*substr = replace;
	}

	return 0;
}

int remove_ctrl(char* buf, size_t bufsize, char replace) {
	for(size_t i = 0; i < bufsize; i++) {
		if(buf[i] < ' ') {
			buf[i] = replace;
		}
	}

	return 0;
}
