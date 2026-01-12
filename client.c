#include "networking.h"

int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }
  int server_socket = client_tcp_handshake(IP);
  printf("connected to music server.\n");

  int f = fork();
  if(f == 0){
    char buffer[BUFFER_SIZE];
    while(1){
      int bytes = recv(server_socket, buffer, BUFFER_SIZE - 1, 0);
      err(bytes, "server disconnected");
      buffer[bytes] = '\0';
      printf("%s", buffer);
      fflush(stdout);
    }
    kill(getppid(), SIGKILL);
    exit(0);
  } else {
    char buffer[BUFFER_SIZE];
    while(1){
      if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) break;
      send(server_socket, buffer, strlen(buffer), 0);
    }
    close(server_socket);
  }
  return 0;
}
