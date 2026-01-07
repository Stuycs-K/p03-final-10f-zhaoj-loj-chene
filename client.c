#include "networking.h"

void clientLogic(int server_socket){
  char buff[200];
  printf("input a string:\n");
  if (!fgets(buff, 200, stdin)){
    printf("client closed\n");
    exit(1);
  }

  if (send(server_socket, buff, 200, 0) < 0){
    perror("send");
    exit(1);
  }

  if (recv(server_socket, buff, 200, 0) < 0){
    perror("recv");
    exit(1);
  }
  printf("string rot13: %s", buff);
}

int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }
  int server_socket = client_tcp_handshake(IP);
  printf("client connected.\n");
  while(1){
    clientLogic(server_socket);
  }
}
