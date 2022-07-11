server: server/main.o common/vector.o server/handlers.o common/protocol.o
	gcc -pthread -o server.out server/main.o common/vector.o server/handlers.o common/protocol.o

client: client/main.o common/protocol.o
	gcc -pthread -o client.out client/main.o common/protocol.o

clean:
	rm -f *.o
	rm -f server/*.o
	rm -f client/*.o
	rm -f common/*.o
