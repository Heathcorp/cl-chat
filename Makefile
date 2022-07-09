server: server/main.o common/vector.o server/handlers.o
	gcc -pthread -o server.out server/main.o common/vector.o server/handlers.o

client: client/main.o
	gcc -pthread -o client.out client/main.o

clean:
	rm -f *.o
	rm -f server/*.o
	rm -f client/*.o
	rm -f common/*.o
