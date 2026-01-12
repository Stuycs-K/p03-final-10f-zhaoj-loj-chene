#include "networking.h"

<<<<<<< HEAD
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

=======
>>>>>>> main
int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }
  int server_socket = client_tcp_handshake(IP);
<<<<<<< HEAD
  printf("client connected.\n");
  while(1){
    clientLogic(server_socket);
  }
=======
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
>>>>>>> main
}
