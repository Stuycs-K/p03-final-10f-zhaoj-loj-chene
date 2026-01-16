.PHONY: compile client server clean default

default :
	echo "default make"

compile: client server

client: client.o networking.o mp3control.o
	@gcc -o client client.o networking.o

server: server.o networking.o account.o playlist.o
	@gcc -o server server.o networking.o account.o playlist.o

mp3control.o: mp3control.c networking.h
	@gcc -Wall -c mp3control.c

client.o: client.c networking.h
	@gcc -Wall -c client.c

server.o: server.c networking.h account.h playlist.h structs.h
	@gcc -Wall -c server.c

networking.o: networking.c networking.h
	@gcc -Wall -c networking.c

account.o: account.c account.h structs.h
	@gcc -Wall -c account.c

playlist.o: playlist.c playlist.h structs.h
	@gcc -Wall -c playlist.c

clean:
	@rm -f *.o client server *.mp3
	@rm -f *~
