.PHONY : default clean compile
default :
	echo "arigyato"
compile server client : networking.o server.o client.o
	@gcc -o server server.o networking.o
	@gcc -o client client.o networking.o
networking.o : networking.c networking.h
	@gcc -c networking.c
client.o : client.c networking.h
	@gcc -c client.c
server.o : server.c networking.h
	@gcc -c server.c
clean :
	@rm server client
	@rm *.o
