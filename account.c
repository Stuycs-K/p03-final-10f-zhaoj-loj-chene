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

int createuser(struct user u){
  return 0;
}

int login(struct user u){ //return file descriptor, -1 if no such login
  char path[100] = "users/";
  strcat(path, u.username);
  int f = open(path, O_RDONLY, 0);
  if (f < 0){
    return -1;
  }
  int* s;
  if (read(f, s, sizeof(int)) < 0){
    error();
  }
  return f;
}

int user_prompt(){
  return 0;
}
