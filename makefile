.PHONY: compile client server clean default

default :
	echo "default make"

compile: client server

client: client.o networking.o
	@gcc -o client client.o networking.o

server: server.o networking.o account.o
	@gcc -o server server.o networking.o account.o

client.o: client.c networking.h
	@gcc -Wall -c client.c

server.o: server.c networking.h account.h
	@gcc -Wall -c server.c

networking.o: networking.c networking.h
	@gcc -Wall -c networking.c

account.o: account.c account.h
	@gcc -Wall -c account.c

clean:
	@rm -f *.o client server users.dat
	@rm -f *~
