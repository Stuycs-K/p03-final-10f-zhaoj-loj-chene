#include "networking.h"

//reading user input
void read_input(char* buffer, int size) {
  if (fgets(buffer, size, stdin) == NULL) {
    exit(0);
  }

  int len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') {
    buffer[len - 1] = '\0';
  }
}

//takes in user struct pointer and copies user data onto the user struct
void user_prompt(struct user* u_ptr){
  char input[100];
  while(1){
    printf("create account or login? (type crt/lgn): ");
    if (fgets(input, sizeof(input), stdin)){
      input[strcspn(input, "\n")] = '\0'; //trim \n at end
      if (strcmp("crt", input) == 0){
        while(1){
          char username[100];
          char password[100];
          printf("enter username: ");
          if (!fgets(username, sizeof(username), stdin)){
            error();
          }
          printf("enter password: ");
          if (!fgets(password, sizeof(password), stdin)){
            error();
          }
          username[strcspn(username, "\n")] = '\0';
          password[strcspn(password, "\n")] = '\0';
          if (createuser(username, password)){
            break;
          }
          printf("username taken\n");
        }
      }
      else if (strcmp("lgn", input) == 0){
        while(1){
          char username[100];
          char password[100];
          printf("enter username: ");
          if (!fgets(username, sizeof(username), stdin)){
            error();
          }
          printf("enter password: ");
          if (!fgets(password, sizeof(password), stdin)){
            error();
          }
          username[strcspn(username, "\n")] = '\0';
          password[strcspn(password, "\n")] = '\0';
          if (login(username, password, u_ptr)){
            break;
          }
          printf("wrong username or password\n");
        }
        break;
      }
      else{
        printf("invalid input: enter \"crt\" or \"lgn\"\n");
      }
    }
    else{
      error();
    }
  }
}

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
