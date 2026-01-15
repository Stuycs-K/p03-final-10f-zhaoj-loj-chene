#ifndef playlist_h
  #define playlist_h
  #include "structs.h"
  int add_song(struct playlist* p, char* name);
  int remove_song(struct playlist* p, char* name);
  void view_playlist(struct playlist* p);
  void list_playlists(struct user* u);
  void make_playlist(struct user* u, char* name);
  void delete_playlist(struct user* u, char* name);
  void write_playlist(char* filename, struct playlist* p);
  void read_playlist(char* filename, struct playlist* p);
  void list_playlists(struct user* u);
  void make_playlist(struct user* u, char* name);
  void delete_playlist(struct user* u, char* name);
#endif
