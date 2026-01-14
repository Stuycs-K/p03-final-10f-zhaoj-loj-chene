#include "playlist.h"
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

//takes a playlist pointer and a song name
//appends the song name to the playlist
//returns 0 if playlist is full, 1 otherwise
int add_song(struct playlist* p, char* name){
  for(int i = 0; i < 50; i++){
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
  while(i < 50 && strcmp(name, p->songs[i]) != 0){
    i++;
  }
  if (i == 50){ //song not found
    return 0;
  }
  for(int k = i; k < 50 - 1; k++){ //shift songs to clear gap
    strcpy(p->songs[k], p->songs[k + 1]);
  }
  p->songs[49][0] = '\0';
  return 1;
}

//prints the name and songs in a playlist
void view_playlist(struct playlist* p){
  if (p->name[0] != '\0'){
    printf("name: %s\n", p->name);
    int i = 0;
    while(i < 50 && p->songs[i][0] != '\0'){
      printf("[%d]: %s\n", i, p->songs[i]);
      i++;
    }
  }
  else{
    printf("playlist empty\n");
  }
}

// lists all the user's playlists
void list_playlists(struct user* u){
  for(int i = 0; i < 5; i++){
    if(user->user_playlist[i].name[0] != '\0'){
      printf("%s\n", user->user_playlist[i].name);
    } else {
      break;
    }
  }
}

// sets playlist name to first empty place in playlist array
void make_playlist(struct user* u, char* name){
  for(int i = 0; i < 5; i++){
    if(user->user_playlist[i].name[0] == '\0'){
      strcpy(user->user_playlist[i].name, name);
      printf("%s successfully created. you now have %d out of 5 playlists\n", name, i);
      break;
    } else {
      printf("No slots available. Please delete a playlist.\n");
    }
  }
}

// deletes playlists
void delete_playlist(struct user* u, char* name){
  for(int i = 0; i < 5; i++){
    if(strcmp(user->user_playlist[i].name, name) == 0){
      user->user_playlist[i].name = NULL; //might have to clear the songs in that playlist
      printf("%s successfully deleted.\n", name);
      break;
    } else {
      printf("%s does not exist.\n", name);
    }
  }
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
