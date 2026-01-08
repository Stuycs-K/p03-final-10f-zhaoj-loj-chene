#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int err(){
    printf("errno %d\n",errno);
    printf("%s\n",strerror(errno));
    return 0;
}

int test(){
  char path[256] = "whoosh.mp3";
  char *args[] = {"mpg123", path, NULL};
  if (execvp(args[0], args) < 0){
    err();
  }
  return 0;
}

void main(){
  test();
}
