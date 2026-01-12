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

//takes in user struct pointer and copies user data onto the user struct
void user_auth_prompt(struct user* u_ptr){
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

void subserver_logic(int client_socket){
  char buffer[BUFFER_SIZE];
  char command[BUFFER_SIZE];
  char *args[10];
  struct user current_user;
  user_auth_prompt(&current_user);
  int logged_in = 0;

  while(!logged_in){
    int bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    err(bytes, "can't connect to client\n");
    buffer[bytes] = '\0';

    //login
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
    } else {
      send(client_socket, "invalid authentication command", 50, 0);
    }
  }

  //music player
  while(logged_in){
    int bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    err(bytes, "can't connect to client\n");
    buffer[bytes] = '\0';

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    }

    printf("user %s requested: %s\n", current_user.username, buffer);

    strcpy(command, buffer);
    parse(command, " ", args);

    if(strcmp(args[0], "play") == 0){
      if(args[1] == NULL){
        send(client_socket, "error: please include song to play", 50, 0);
      } else {
        char path[256] = "./music/";
        strcat(path, args[1]);
        int f = fork();
        if(f == 0){
          execvp("mpg123", path, NULL);
          exit(1);
        } else {
          int status;
          waitpid(f, &status, 0);
          send(client_socket, "done playing.", 50, 0);
        }
      }
    } else if (strcmp(args[0], "exit") == 0){
      break;
    } else {
      send(client_socket, "invalid command.", 50, 0);
    }
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
      close(client_socket);
      exit(0);
    }
    close(client_socket);
  }
}
