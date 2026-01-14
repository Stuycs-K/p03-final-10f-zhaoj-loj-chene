.PHONY: compile client server clean default

default :
	echo "default make"

compile: client server

client: client.o networking.o
	@gcc -o client client.o networking.o

server: server.o networking.o account.o
	@gcc -o server server.o networking.o account.o

mp3control: mp3control.o
	@gcc -o mp3control mp3control.o

client.o: client.c networking.h
	@gcc -Wall -c client.c

server.o: server.c networking.h account.h
	@gcc -Wall -c server.c

networking.o: networking.c networking.h
	@gcc -Wall -c networking.c

account.o: account.c account.h
	@gcc -Wall -c account.c

mp3control.o: mp3control.c mp3control.h
	@gcc -Wall -c mp3control.c

clean:
	@rm -f *.o client server temp.mp3 mp3control
	@rm -f *~
