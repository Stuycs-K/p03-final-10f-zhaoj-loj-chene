#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <mpg123.h>
#include <errno.h>

int err(){
    printf("errno %d\n",errno);
    printf("%s\n",strerror(errno));
}

void test(){
  execvp(--listentry, -1 )
}
