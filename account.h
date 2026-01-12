#ifndef account_h
  #define account_h
  struct playlist {char name[100]; char songs[50][100];};
  struct user {char username[100]; char password[100]; struct playlist user_playlists[5];};
  int error();
  void user_prompt(struct user* u_ptr);
  int createuser(char* username, char* password);
  int login(char* username, char* password, struct user* u_ptr);
#endif
