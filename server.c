#include "networking.h"

int parse(char * line, char * delim, char ** arg_ary){
  char * token = calloc(1, sizeof(line) + 1);
  int size = 1;
  token = strsep(&line, delim);
  while (token != NULL){
    arg_ary[size - 1] = token;
    token = strsep(&line, delim);
    size++;
  }
  arg_ary[size - 1] = NULL;
  return size;
}

void subserver_logic(int client_socket){
  char buffer[BUFFER_SIZE];

  int bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
  err(bytes, "server receive error\n");
  printf("received from client: %s\n", buffer);


  buffer[bytes] = '\0';
  parse(buffer___);
  printf("sending to client: %s\n", buffer);
  send(client_socket, buffer, strlen(buffer), 0);
}

int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();

  while(1){
    int client_socket = server_tcp_handshake(listen_socket);

    int f = fork();
    if(f == 0){
      close(listen_socket);
      subserver_logic(client_socket);
      fflush(stdout);
      close(client_socket);
      exit(0);
    }
    close(client_socket);
  }
}
