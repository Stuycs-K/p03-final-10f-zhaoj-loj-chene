#ifndef playlist_h
  #define playlist_h
  struct playlist {char name[100]; char songs[50][100];};
  int add_song(struct playlist* p, char* name);
  int remove_song(struct playlist* p, char* name);
  int change_name(struct playlist* p, char* name);
  void write_playlist(char* filename, struct playlist* p);
  void read_playlist(char* filename, struct playlist* p);
#endif