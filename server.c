#include "networking.h"

void rot13(char*s){
  int i = 0;
  while(s[i] != '\0'){
    char c = s[i];
    if(c >= 'a' && c <= 'z'){
      s[i] = 'a' + (c - 'a' + 13) % 26;
    } else if (c >= 'A' && c <= 'Z'){
      s[i] = 'A' + (c - 'A' + 13) % 26;
    }
    i++;
  }
}

void subserver_logic(int client_socket){
  char buffer[BUFFER_SIZE];

  int bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
  err(bytes, "server receive error\n");
  printf("received from client: %s\n", buffer);


  buffer[bytes] = '\0';
  rot13(buffer);
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
