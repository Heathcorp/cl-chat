server: server/main.o common/vector.o server/handlers.o common/protocol.o common/utils.o common/trans_buffer.o
	gcc -g -pthread -o server.out server/main.o common/vector.o server/handlers.o common/protocol.o common/utils.o common/trans_buffer.o

client: client/main.o common/protocol.o common/utils.o common/trans_buffer.o
	gcc -g -pthread -o client.out client/main.o common/protocol.o common/utils.o common/trans_buffer.o

clean:
	rm -f *.o
	rm -f server/*.o
	rm -f client/*.o
	rm -f common/*.o
