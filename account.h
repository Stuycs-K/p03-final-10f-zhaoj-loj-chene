#ifndef account_h
  #define account_h
  struct user {char username[100]; char password[100];};
  int user_prompt();
  int createuser(struct user u);
  int login(struct user u);
#endif
