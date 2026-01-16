#include "mp3control.h"

pid_t mpg123_pid = -1;
char *mp3_file;

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

void adjust_volume(int percent) {
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

int main() {
  while(fgets(buffer, BUFFER_SIZE, stdin) != NULL){

  }
  execlp("mpg123", "mpg123", "-R", NULL);

  FILE *fptr = fopen(STDIN_FILENO, "wb");

  int a[] = {1, 2, 3, 4, 5};
  int n = sizeof(a) / sizeof(a[0]);

  // Write array a[] into the file using fwrite
  fwrite(a, sizeof(int), n, fptr);

  fclose(fptr);
  execlp("load", "load","./music/piano.mp3", NULL);
  execlp("mpg123", "mpg123","V", 200, NULL);
  return 0;
}
