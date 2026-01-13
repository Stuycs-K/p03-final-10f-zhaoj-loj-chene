#ifndef account_h
  #define account_h
  struct playlist {char name[100]; char songs[50][100];};
  struct user {char username[100]; char password[100]; struct playlist user_playlists[5];};
  void error();
  int createuser(char* username, char* password);
  int login(char* username, char* password, struct user* u_ptr);
  int delete_account(char* username, char* passowrd);
  void write_playlist(char* filename, struct playlist* p);
  void read_playlist(char* filename, struct playlist* p);
  int add_song(struct playlist* p);
  int remove_song(struct playlist* p);
  int name_playlist(struct playlist* p, char* name);
#endif
