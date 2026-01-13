#include "account.h"
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

//takes in a filename and a playlist pointer
//writes the playlist to the file in a format suitable for mpg123
//creates file if it doesn't exist and replaces it if it does
void write_playlist(char* filename, struct playlist* p){
  int f = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (f < 0){
    error();
  }
  if (write(f, p->name, strlen(p->name)) < 0){
    error();
  }
  write(f, "\n", 1); //newline for mpg123 playlist format

  for(int i = 0; i < 50; i++){
    if (p->songs[i][0] == '\0'){ //break loop if song is null
      break;
    }
    if (write(f, p->songs[i], strlen(p->songs[i])) < 0){
      error();
    }
    write(f, "\n", 1);
  }
  close(f);
}

void read_playlist(char* filename, struct playlist* p){
  FILE* fp = fopen(filename, "r");
  if (!fp){
    error();
  }
  char line[100];
  int index = 0;
  memset(p, 0, sizeof(struct playlist)); //assuming memory of playlist was not necessarily allocated
  if (!fgets(line, sizeof(line), fp)) { //read playlist name
    error();
  }
  line[strcspn(line, "\n")] = '\0'; //trim newline at end
  strcpy(p->name, line);

  while (fgets(line, sizeof(line), fp) && index < 50){
      line[strcspn(line, "\n")] = '\0';
      strcpy(p->songs[index], line);
      index++;
  }
  fclose(fp);
}
