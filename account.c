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

int createuser(char* username, char* password){
  return 0;
}

int login(char* username, char* password, struct user* u_ptr){ //returns 0 if user not found, 1 otherwise
  int f = open("users.dat", O_RDONLY, 0);
  if (f < 0){
    error();
  }
  struct user buff;
  while(read(f, &buff, sizeof(struct user)) == sizeof(struct user)){
    if (!strcmp(buff.username, username) && !strcmp(buff.password, password)){
      *u_ptr = buff;
      close(f);
      return 1;
    }
  }
  close(f);
  return 0;
}

int user_prompt(){
  return 0;
}
