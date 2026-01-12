#include "networking.h"
<<<<<<< HEAD

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
=======
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
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin)){
      input[strcspn(input, "\n")] = '\0'; //trim \n at end
      if (strcmp("crt", input) == 0){
        while(1){
          char username[100];
          char password[100];
          printf("enter username: ");
          fflush(stdout);
          if (!fgets(username, sizeof(username), stdin)){
            error();
          }
          printf("enter password: ");
          fflush(stdout);
          if (!fgets(password, sizeof(password), stdin)){
            error();
          }
          username[strcspn(username, "\n")] = '\0';
          password[strcspn(password, "\n")] = '\0';
          if (createuser(username, password)){
            break;
          }
          printf("username taken\n");
          fflush(stdout);
        }
      }
      else if (strcmp("lgn", input) == 0){
        while(1){
          char username[100];
          char password[100];
          printf("enter username: ");
          fflush(stdout);
          if (!fgets(username, sizeof(username), stdin)){
            error();
          }
          printf("enter password: ");
          fflush(stdout);
          if (!fgets(password, sizeof(password), stdin)){
            error();
          }
          username[strcspn(username, "\n")] = '\0';
          password[strcspn(password, "\n")] = '\0';
          if (login(username, password, u_ptr)){
            break;
          }
          printf("wrong username or password\n");
          fflush(stdout);
        }
        break;
      }
      else{
        printf("invalid input: enter \"crt\" or \"lgn\"\n");
        fflush(stdout);
      }
    }
    else{
      error();
    }
  }
}

void subserver_logic(int client_socket){
  dup2(client_socket, STDIN_FILENO);
  dup2(client_socket, STDOUT_FILENO);

  //login
  struct user current_user;
  user_auth_prompt(&current_user);

  //music player
  printf("welcome %s! type 'play <song.mp3>' or 'exit'.\n", current_user.username);
  fflush(stdout);

  char buffer[BUFFER_SIZE];
  char command[BUFFER_SIZE];
  char *args[10];

  while(fgets(buffer, BUFFER_SIZE, stdin) != NULL){
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    }

    strcpy(command, buffer);
    parse(command, " ", args);

    if(strcmp(args[0], "play") == 0){
      if(args[1] == NULL){
        send(client_socket, "error: please include song to play", 50, 0);
        fflush(stdout);
      } else {
        char path[256] = "./music/";
        strcat(path, args[1]);

        printf("playing %s...\n", args[1]);
        fflush(stdout);

        int f = fork();
        if(f == 0){
          char *args[] = {"mpg123", path, NULL};
          execvp(args[0], args);
          exit(1);
        } else {
          int status;
          waitpid(f, &status, 0);
          printf("done playing.");
          fflush(stdout);
        }
      }
    } else if (strcmp(args[0], "exit") == 0){
      break;
    } else {
      printf("invalid command.");
      fflush(stdout);
    }
  }
>>>>>>> main
}

int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();
<<<<<<< HEAD
  while(1){
    int client_socket = server_tcp_handshake(listen_socket);
    int pid = fork();
    if (pid == 0){
      while(1){
        subserver_logic(client_socket);
      }
    }
  }

=======

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
>>>>>>> main
}
