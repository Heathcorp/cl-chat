#ifndef UTILS_H
#define UTILS_H

#include <sys/time.h>
#include <stdlib.h>

time_t millis();
// writes a 16 character hexadecimal representation of the current time to dest
int write_timestamp(char* dest, time_t t);
int read_timestamp(char* src, time_t* t);

// read and write a single byte as a 2 digit hex number
int write_hex_byte(char byte, char* dest);
char read_hex_byte(char* src);

// convert a single hexadecimal character digit to an int
int hexchar2num(char hex);
char num2hexchar(int digit);

int verifyhex(char* buf, size_t n);

int hexdump(void* buf, size_t bufsize, size_t rowsize);

int replace_char(char* buf, size_t bufsize, char target, char replace);

int remove_ctrl(char* buf, size_t bufsize, char replace);
// copy up to n characters until it reaches char c
size_t cpync(char* dest, char* src, char c, size_t n);

#define COMMS_REGISTER		'R'
#define COMMS_MESSAGE		'M'
#define COMMS_DISCONNECT	'Q'
#define COMMS_DEBUG			'0'

#define EOT '\x04'
#define CR	'\n'

#define TRUE	1
#define FALSE	0

#define MAX_USERNAME_LENGTH	64
#define MAX_MESSAGE_LENGTH	256

#endif