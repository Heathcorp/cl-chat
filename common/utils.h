#ifndef UTILS_H
#define UTILS_H

#include <sys/time.h>
#include <stdlib.h>

time_t millis();

int hexdump(void* buf, size_t bufsize, size_t rowsize);

int replace_char(char* buf, size_t bufsize, char target, char replace);

int remove_ctrl(char* buf, size_t bufsize, char replace);

#define COMMS_REGISTER		'R'
#define COMMS_MESSAGE		'M'
#define COMMS_DISCONNECT	'Q'
#define COMMS_DEBUG			'0'

#define EOT '\x04'

#endif