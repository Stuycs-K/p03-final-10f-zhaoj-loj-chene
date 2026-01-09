#include "networking.h"

void clientLogic(int server_socket){
  char buffer[BUFFER_SIZE];

  printf("Enter a message: \n");
  if(fgets(buffer, BUFFER_SIZE, stdin) == NULL){
      exit(1);
  }
  sscanf(buffer, "%[^\n]", buffer);

  int s = send(server_socket, buffer, strlen(buffer), 0);
  err(s, "send to client error\n");

  int bytes = recv(server_socket, buffer, BUFFER_SIZE - 1, 0);
  buffer[bytes] = '\0';
  err(bytes, "error receiving\n");

  printf("server response: %s\n", buffer);
}

int main(int argc, char *argv[] ) {
  while(1){
    char* IP = "127.0.0.1";
    if(argc>1){
      IP=argv[1];
    }
    int server_socket = client_tcp_handshake(IP);
    printf("client connected.\n");
    clientLogic(server_socket);
    close(server_socket);
  }
}
