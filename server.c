#include "networking.h"

void subserver_logic(int client_socket){
  char buff[200];
  int bytes = recv(client_socket, buff, 200, 0);
  if (bytes <= 0){
    printf("socket closed\n");
    exit(1);
  }
  buff[bytes] = '\0';
  printf("received %s", buff);

  for (int i = 0; buff[i] != '\0'; i++){
    if (isupper(buff[i])){
      buff[i] = (buff[i] - 'A' + 13) % 26 + 'A';
    }
    else if (islower(buff[i])){
      buff[i] = (buff[i] - 'a' + 13) % 26 + 'a';
    }
  }

  if (send(client_socket, buff, 200, 0) < 0){
    perror("send");
    exit(1);
  }
  printf("sent %s", buff);
}

int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();
  while(1){
    int client_socket = server_tcp_handshake(listen_socket);
    int pid = fork();
    if (pid == 0){
      while(1){
        subserver_logic(client_socket);
      }
    }
  }

}
