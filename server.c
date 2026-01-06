#include "networking.h"

void subserver_logic(int client_socket){
  char response[100];
  while(1){
    int bytes_recv = recv(client_socket, response, sizeof(response), 0);
    if(bytes_recv <= 0){
      printf("Client at port %d disconnected\n", client_socket);
      break;
    }
    response[bytes_recv] = '\0';
    printf("received: %s\n", response);
    char tosend[100];
    rotX(tosend, response, 13);
    printf("sending %s to client after rotating \n", tosend);
    send(client_socket, tosend, bytes_recv, 0);
  }

  close(client_socket);
}

int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();

  signal(SIGCHLD, SIG_IGN);
  while(1) {
    int client_socket = server_tcp_handshake(listen_socket);
    if(client_socket > 0) {
      printf("server successfully connected to new client\n");
    }
    else{
      err(client_socket, "client_socket problem");
    }
    pid_t pid = fork();
    if(pid < 0) {
      perror("fork failed");
      close(client_socket);
    }
    else if(pid == 0) {
      close(listen_socket);
      subserver_logic(client_socket);
      exit(0);
    }
    else {
      close(client_socket);
    }
  }

  return 0;
}
