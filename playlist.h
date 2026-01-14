#ifndef playlist_h
  #define playlist_h
  struct playlist {char name[100]; char songs[50][100];};
  struct user {char username[100]; char password[100]; struct playlist user_playlists[5];};
  void error();
  int add_song(struct playlist* p, char* name);
  int remove_song(struct playlist* p, char* name);
  void view_playlist(struct playlist* p);
  void write_playlist(char* filename, struct playlist* p);
  void read_playlist(char* filename, struct playlist* p);
  void list_playlists(struct user* u);
  void make_playlist(struct user* u, char* name);
  void delete_playlist(struct user* u, char* name);
#endif
