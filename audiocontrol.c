#include "audiocontrol.h"

pid_t mpg123_pid = -1;
char *mp3_file;

int err(){
    printf("errno %d\n",errno);
    printf("%s\n",strerror(errno));
    return 0;
}

void stop_playback() {
    if (mpg123_pid > 0) {
        kill(mpg123_pid, SIGTERM);
        waitpid(mpg123_pid, NULL, 0);
        mpg123_pid = -1;
    }
}

void cleanup(int sig) {
    stop_playback();
    printf("\nexiting.\n");
    exit(0);
}

void play_with_volume(int percent) {
    int scale = (int)(32768 * (percent / 100.0));

    stop_playback();

    mpg123_pid = fork();

    if (mpg123_pid == 0) {
        char scale_arg[16];
        snprintf(scale_arg, sizeof(scale_arg), "%d", scale);

        execlp("mpg123", "mpg123", "-f", scale_arg, mp3_file, NULL);

        perror("mpg123 failed");
        exit(1);
    }
}

void mp3_prompt(){
  if (argc < 2) {
      printf("Usage: %s mp3_file\n", argv[0]);
      return 1;
  }

  mp3_file = argv[1];
  char path[256] = "./music/";
  strcat(path, mp3_file);
  printf("trying to open %s \n", path);
  FILE *file = fopen(path, "rb");
  if(!file){
    printf("error: song not found\n");
    fflush(stdout);
  }

  while(1){
    printf("create account or login? (type crt/lgn): ");
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin)){
      input[strcspn(input, "\n")] = '\0'; //trim \n at end
      if (strcmp("crt", input) == 0){
        while(1){
          char username[100];
          char password[100];
          printf("enter username: ");
          fflush(stdout);
          if (!fgets(username, sizeof(username), stdin)){
            error();
          }
          printf("enter password: ");
          fflush(stdout);
          if (!fgets(password, sizeof(password), stdin)){
            error();
          }
          username[strcspn(username, "\n")] = '\0';
          password[strcspn(password, "\n")] = '\0';
          if (createuser(username, password)){
            break;
          }
          printf("username taken\n");
          fflush(stdout);
        }
      }
      else if (strcmp("lgn", input) == 0){
        while(1){
          char username[100];
          char password[100];
          printf("enter username: ");
          fflush(stdout);
          if (!fgets(username, sizeof(username), stdin)){
            error();
          }
          printf("enter password: ");
          fflush(stdout);
          if (!fgets(password, sizeof(password), stdin)){
            error();
          }
          username[strcspn(username, "\n")] = '\0';
          password[strcspn(password, "\n")] = '\0';
          if (login(username, password, u_ptr)){
            break;
          }
          printf("wrong username or password\n");
          fflush(stdout);
        }
        break;
      }
      else{
        printf("invalid input: enter \"crt\" or \"lgn\"\n");
        fflush(stdout);
      }
    }
    else{
      error();
    }
  }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s mp3_file\n", argv[0]);
        return 1;
    }

    mp3_file = argv[1];
    char path[256] = "./music/";
    strcat(path, mp3_file);
    printf("trying to open %s \n", path);
    FILE *file = fopen(path, "rb");
    if(!file){
      printf("error: song not found\n");
      fflush(stdout);
    }

    signal(SIGINT, cleanup);

    char input[128];

    printf("type: vol <number>  (example: vol 110)\n");
    printf("ctrl+c to exit\n");

    while (1) {
        printf("> ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin))
            break;

        if (strncmp(input, "vol ", 4) == 0) {
            int vol = atoi(input + 4);

            if (vol <= 0) {
                printf("invalid volume\n");
                continue;
            }

            printf("replaying at %d%% volume\n", vol);
            play_with_volume(vol);
        } else {
            printf("unknown command\n");
        }
    }

    cleanup(0);

    test();

    return 0;
}

int test(){
  char path[256] = "piano.mp3";
  char *args[] = {"mpg123", path, NULL};
  if (execvp(args[0], args) < 0){
    err();
  }
  return 0;
}
