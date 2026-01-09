.PHONY: compile client server clean

compile: client server

client: client.o networking.o
	@gcc -o client client.o networking.o

server: server.o networking.o
	@gcc -o server server.o networking.o

client.o: client.c networking.h
	@gcc -Wall -c client.c

server.o: server.c networking.h
	@gcc -Wall -c server.c

networking.o: networking.c networking.h
	@gcc -Wall -c networking.c

clean:
	@rm -f *.o client server
	@rm -f *~
