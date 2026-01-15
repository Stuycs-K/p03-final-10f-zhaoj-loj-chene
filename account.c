#include "account.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void error(){
  printf("errno %d\n",errno);
  printf("%s\n",strerror(errno));
  exit(1);
}

//returns 0 if username taken, 1 otherwise
int createuser(char* username, char* password){
  int f = open("users.dat", O_RDWR | O_CREAT, 0666);
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

  lseek(f, 0, SEEK_END);
  if (write(f, &new_acc, sizeof(struct user)) < 0){
    error();
  }
  close(f);
  return 1;
}

//returns 0 if user not found, 1 otherwise
//takes in a pointer and copies user data onto it
int login(char* username, char* password, struct user* u_ptr){
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

//takes in a user pointer and copies the data onto users.dat
void save(struct user* u_ptr){
  int f = open("users.dat", O_RDWR | O_CREAT, 0666);
  if (f < 0){
    error();
  }
  struct user buff;
  while(read(f, &buff, sizeof(struct user)) == sizeof(struct user)){
    if (strcmp(buff.username, u_ptr->username) == 0 && strcmp(buff.password, u_ptr->password) == 0){
      lseek(f, -1 * sizeof(struct user), SEEK_CUR);
      write(f, u_ptr, sizeof(struct user));
      break;
    }
  }
  close(f);
}

//takes in a username and deletes that account in the users data file
//removes nothing if username isn't there
void delete_account(char* username){
  int dat = open("users.dat", O_RDONLY, 0);
  if (dat < 0){
    error();
  }
  int tmp = open("users.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (tmp < 0){
    error();
  }

  struct user u;
  while(read(dat, &u, sizeof(struct user)) == sizeof(struct user)){
    if (strcmp(u.username, username) == 0){ //skip
      continue;
    }
    if (write(tmp, &u, sizeof(struct user)) < 0){
      error();
    }
  }
  close(tmp);
  close(dat);
  rename("users.tmp", "users.dat");
}
