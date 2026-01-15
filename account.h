#ifndef account_h
  #define account_h
  #include "structs.h"
  void error();
  int createuser(char* username, char* password);
  int login(char* username, char* password, struct user* u_ptr);
  void delete_account(char* username);
#endif
