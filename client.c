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
    char buffer[BUFFER_SIZE]; // child: receives from server
    while(1){
      int bytes = recv(server_socket, buffer, BUFFER_SIZE - 1, 0);
      if(bytes <= 0){
        printf("server disconnected\n");
        break;
      }
      buffer[bytes] = '\0';

      if(strncmp(buffer, "FILE|", 5) == 0){   // check if it's a file transfer
        long file_size;
        char filename[100];
        sscanf(buffer, "FILE|%ld|%s", &file_size, filename);

        printf("receiving %s (%ld bytes)...\n", filename, file_size);

        FILE *temp = fopen("temp.mp3", "wb"); // receive file data
        long received = 0;

        while(received < file_size){
            bytes = recv(server_socket, buffer, BUFFER_SIZE, 0);
            printf("%ld byte copied \n", received);
            if(bytes <= 0) break;
            fwrite(buffer, 1, bytes, temp);
            received += bytes;
        }
        fclose(temp);

        printf("playing %s...\n", filename); // play the file
        fflush(stdout);

        int player = fork();
        if(player == 0){
            execlp("mpg123", "mpg123", "temp.mp3", NULL);
            exit(1);
        }
        waitpid(player, NULL, 0);

        printf("done playing.\n");
        fflush(stdout);

      } else {
        printf("%s", buffer); // regular text message
        fflush(stdout);
      }
    }
    kill(getppid(), SIGKILL);
    exit(0);

  } else {
    char buffer[BUFFER_SIZE]; // parent: sends to server
    while(1){
      if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) break;
      send(server_socket, buffer, strlen(buffer), 0);
    }
    close(server_socket);
  }
  return 0;
}
