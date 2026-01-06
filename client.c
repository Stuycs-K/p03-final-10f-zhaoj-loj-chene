#include "networking.h"

void clientLogic(int server_socket){

}

int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }
  int server_socket = client_tcp_handshake(IP);
  printf("client connected.\n");

  printf("this is client, server_socket:%d \n", server_socket);
  char input[100];

  while(1) {
    printf("Enter message (or 'quit' to exit): ");
    if(fgets(input, sizeof(input), stdin) == NULL) {
        break;
    }
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if(strcmp(input, "quit") == 0) {
      break;
    }
    send(server_socket, input, strlen(input) + 1, 0);
    char response[100];
    int bytes_received = recv(server_socket, response, sizeof(response) - 1, 0);
    if(bytes_received <= 0) {
    printf("server disconnected\n");
    break;
    }
    if(bytes_received > 0) {
      response[bytes_received] = '\0';
      printf("server replied: %s\n", response);
    }
  }
  clientLogic(server_socket);
  close(server_socket);
}
