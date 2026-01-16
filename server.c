#include "structs.h"
#include "networking.h"
#include "account.h"
#include "playlist.h"
#include "mp3control.h"

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
            printf("user created successfully. please log in.\n");
            fflush(stdout);
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
void send_song_file(int client_socket, const char* song_filename){
  char path[256] = "./music/";
  strcat(path, song_filename);

  FILE *file = fopen(path, "rb");
  if(!file){
      printf("error: song %s not found, skipping\n", song_filename);
      fflush(stdout);
      return;
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  char header[300];
  sprintf(header, "FILE|%ld|%s\n", file_size, song_filename);
  send(client_socket, header, strlen(header), 0);

  char file_buffer[BUFFER_SIZE];
  size_t bytes_read;
  while((bytes_read = fread(file_buffer, 1, BUFFER_SIZE, file)) > 0){
      send(client_socket, file_buffer, bytes_read, 0);
  }
  fclose(file);

  printf("sent %s to client\n", song_filename);
  fflush(stdout);
}

void subserver_logic(int client_socket){
  dup2(client_socket, STDIN_FILENO);
  dup2(client_socket, STDOUT_FILENO);

  struct user current_user; //login
  user_auth_prompt(&current_user);

  printf("welcome %s! type 'play <song.mp3>' or 'exit'.\n", current_user.username); //music player
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
          printf("error: please include song to play\n");
          fflush(stdout);
      } else if (strcmp(args[1], "playlist") == 0){
        if (args[2] == NULL){
          printf("error: please include playlist to play\n");
          fflush(stdout);
        } else {
          int found = 0;
          for(int i = 0; i < 5; i++){
            if (strcmp(current_user.user_playlists[i].name, args[2]) == 0){
              found = 1;
              usleep(100000); //for timing issues
              write_playlist(args[2], &(current_user.user_playlists[i]));
              break;
            }
          }
          if (!found){
            printf("error: playlist not found\n");
            fflush(stdout);
          }
          char header[300]; // send header: "play|playlist_name"
          sprintf(header, "playlist|%s\n", args[2]);
          send(client_socket, header, strlen(header), 0);
        }
      } else {
          char musicname[256];
          strcpy(musicname, args[1]);
          send_song_file(client_socket,musicname);
        }
    } else if(strcmp(args[0], "vol") == 0){
        if(args[1] == NULL){
            printf("error: please specify volume (0-200)\n");
            fflush(stdout);
        } else {
            int volume = atoi(args[1]);
            char mpg123_command[50];
            sprintf(mpg123_command, "VOL|%d\n", volume);
            send(client_socket, mpg123_command, strlen(mpg123_command), 0);
            printf("sent volume command: %d%%\n", volume);
            fflush(stdout);
        }
    } else if(strcmp(args[0], "pause") == 0){
      send(client_socket, "PAUSE\n", 6, 0);
      printf("sent pause command\n");
      fflush(stdout);
    } else if (strcmp(args[0], "exit") == 0){  // to sign out of account and kill client
      save(&current_user);
      send(client_socket, "logged out", 50, 0);
      break;
    } else if (strcmp(args[0], "remove") == 0){
      if (args[1] == NULL){
        printf("error: please include playlist to remove from\n");
        fflush(stdout);
        continue;
      } else if (args[2] == NULL){
        printf("error: please include song to remove\n");
        fflush(stdout);
      } else{
        int found = 0;
        for(int i = 0; i < 5; i++){
          if (strcmp(current_user.user_playlists[i].name, args[1]) == 0){
            found = 1;
            if (!remove_song(&(current_user.user_playlists[i]), args[2])){
              printf("error: song not found\n");
              fflush(stdout);
            }
            break;
          }
        }
        if (!found){
          printf("error: playlist not found\n");
          fflush(stdout);
        }
      }
    } else if (strcmp(args[0], "add") == 0){
      if (args[1] == NULL){
        printf("error: please include playlist to add to\n");
        fflush(stdout);
        continue;
      } else if (args[2] == NULL){
        printf("error: please include song to add\n");
        fflush(stdout);
      } else{
        char path[256];
        sprintf(path, "./music/%s", args[2]);
        struct stat st;
        if (stat(path, &st) != 0){
          printf("error: song not found\n");
          fflush(stdout);
        } else {
          int found = 0;
          for(int i = 0; i < 5; i++){
            if (strcmp(current_user.user_playlists[i].name, args[1]) == 0){
              found = 1;
              if (!add_song(&(current_user.user_playlists[i]), args[2])){
                printf("error: playlist full\n");
                fflush(stdout);
              }
              break;
            }
          }
          if (!found){
            printf("error: playlist not found\n");
            fflush(stdout);
          }
        }
      }
    } else if (strcmp(args[0], "make") == 0){
      if (args[1] == NULL){
        printf("error: please include a name\n");
        fflush(stdout);
      } else {
        make_playlist(&current_user, args[1]);
        fflush(stdout);
      }
    } else if (strcmp(args[0], "playlists") == 0){
      list_playlists(&current_user);
      fflush(stdout);
    } else if (strcmp(args[0], "view") == 0){
      if (args[1] == NULL){
        printf("error: please include playlist to view\n");
        fflush(stdout);
      } else {
        int found = 0;
        for(int i = 0; i < 5; i++){
          if (strcmp(current_user.user_playlists[i].name, args[1]) == 0){
            found = 1;
            view_playlist(&(current_user.user_playlists[i]));
            fflush(stdout);
            break;
          }
        }
        if (!found){
          printf("error: playlist not found\n");
          fflush(stdout);
        }
      }
    } else if(strcmp(args[0], "delete") == 0){
      if (args[1] == NULL){
        printf("error: please include playlist to delete or \"account\" to delete account\n");
        fflush(stdout);
      } else if (strcmp(args[1], "account") == 0){
        delete_account(current_user.username);
        break;
      } else {
        int found = 0;
        for(int i = 0; i < 5; i++){
          if (strcmp(current_user.user_playlists[i].name, args[1]) == 0){
            found = 1;
            delete_playlist(&current_user, current_user.user_playlists[i].name);
            fflush(stdout);
            break;
          }
        }
        if (!found){
          printf("error: playlist not found\n");
          fflush(stdout);
        }
      }
    } else {
      printf("invalid command.\n");
      fflush(stdout);
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
