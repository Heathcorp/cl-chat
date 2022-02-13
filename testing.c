#include <stdio.h>
extern char **environ;

int main(int argc, char **argv) {
	for(int i = 0; i < argc; i++) {
		puts(argv[i]);
	}
	for(;*environ;environ++) {
		puts(*environ);
	}
	return 0;
}
