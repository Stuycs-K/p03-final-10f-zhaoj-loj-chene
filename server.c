#include "networking.h"
#include "account.h"

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
  char *args[10];
  struct user current_user;
  int logged_in = 0;

  while(!signed_in){
    int bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    err(bytes, "can't connect to client\n");
    buffer[bytes] = '\0';

    //login
    char command[BUFFER_SIZE];
    strcpy(command, buffer);
    parse(command, " ", args);

    if(strcmp(args[0], "crt") == 0){
      if(createuser(args[1], args[2])){
        send(client_socket, "account created. please log in", 50, 0); \\uses magic num maybe change later
      } else {
        send(client_socket, "account cannot be created. username taken", 50, 0); \\uses magic num maybe change later
      }
    } else if (strcmp(args[0], "lgn") == 0){
      if(login(args[1], args[2], &current_user)){
        send(client_socket, "logged in", 50, 0);
        logged_in = 1;
        printf("user %s logged in.\n", current_user.username);
      } else{
        send(client_socket, "invalid login.", 50, 0);
      }
    }

    printf("sending to client: %s\n", buffer);

  }
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
