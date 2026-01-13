#include "playlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int add_song(struct playlist* p, char* name){
  
}

int remove_song(struct playlist* p, char* name){

}

int change_name(struct playlist* p, char* name){

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

//takes in a filename and a playlist pointer
//writes the file to the playlist (assuming valid format)
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