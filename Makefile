.PHONY: clean

COMMON = $(subst .c,.o,$(wildcard common/*.c))
SERVER = $(subst .c,.o,$(wildcard server/*.c))
CLIENT = $(subst .c,.o,$(wildcard client/*.c))

%.o: %.c
	gcc -g -c -o $@ $^

server: $(COMMON) $(SERVER)
	gcc -g -pthread -o $@.out $^

client: $(COMMON) $(CLIENT)
	gcc -g -pthread -o $@.out $^

all: server client

clean:
	rm -f common/*.o
	rm -f server/*.o
	rm -f client/*.o
