#include "account_h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fctnl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int error(){
    printf("errno %d\n",errno);
    printf("%s\n",strerror(errno));
    exit(1);
}

int createuser(char* username, char* password){ //returns 0 if username taken, 1 otherwise
  int f = open("users.dat", O_RDWR, 0);
  if (f < 0){
    error();
  }
  struct user buff;
  while(read(f, &buff, sizeof(struct user)) == sizeof(struct user)){
    if (strcmp(buff.username, username) == 0){
      close(f);
      return 0;
    }
  }
  struct user new_acc;
  memset(&new_acc, 0, sizeof(struct user));
  strcpy(new_acc.username, username);
  strcpy(new_acc.password, password);
  if (write(f, &new_acc, sizeof(struct user)) < 0){
    error();
  }
  close(f);
  return 1;
}

int login(char* username, char* password, struct user* u_ptr){ //returns 0 if user not found, 1 otherwise
  int f = open("users.dat", O_RDONLY, 0);
  if (f < 0){
    error();
  }
  struct user buff;
  while(read(f, &buff, sizeof(struct user)) == sizeof(struct user)){
    if (strcmp(buff.username, username) == 0 && strcmp(buff.password, password) == 0){
      *u_ptr = buff;
      close(f);
      return 1;
    }
  }
  close(f);
  return 0;
}

void user_prompt(struct user* u_ptr){ //takes in user struct pointer and copies user data onto the user struct
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
        break;
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
