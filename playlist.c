#include "playlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

//takes a playlist pointer and a song name
//appends the song name to the playlist
//returns 0 if playlist is full, 1 otherwise
int add_song(struct playlist* p, char* name){
  for(int i = 0; i < 50, i++){
    if(p->songs[i][0] == '\0'){
      strcpy(p->songs[i], name);
      return 1;
    }
  }
  return 0;
}

//takes a playlist pointer and a song name
//removes the song from playlist and shifts to remove gap
//returns 0 if song isn't in playlist, 1 otherwise
int remove_song(struct playlist* p, char* name){
  int i = 0;
  while(strcmp(name, p->songs[i]) != 0){
    i++;
  }
  if (i == 50){ //song not found
    return 0;
  }
  for(int k = i; k < 50 - 1; k++){ //shift songs to clear gap
    strcpy(p->songs[i], p->songs[i + 1]);
  }
  p->songs[49][0] = '\0';
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