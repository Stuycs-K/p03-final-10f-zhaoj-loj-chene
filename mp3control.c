#include "mp3control.h"

pid_t mpg123_pid = -1;
int mpg123_stdin = -1;
int mpg123_stdout = -1;

int start_mpg123_remote() {
  int pipe_to_mpg123[2];   // parent writes, mpg123 reads
  int pipe_from_mpg123[2]; // mpg123 writes, parent reads

  mpg123_pid = fork();

  if (mpg123_pid == 0) {
      close(pipe_to_mpg123[1]);    // close write end
      close(pipe_from_mpg123[0]);  // close read end

      dup2(pipe_to_mpg123[0], STDIN_FILENO);
      dup2(pipe_from_mpg123[1], STDOUT_FILENO);

      close(pipe_to_mpg123[0]);
      close(pipe_from_mpg123[1]);

      execlp("mpg123", "mpg123", "-R", NULL);
      exit(1);
  }
  close(pipe_to_mpg123[0]);    // close read end
  close(pipe_from_mpg123[1]);  // close write end

  mpg123_stdin = pipe_to_mpg123[1];
  mpg123_stdout = pipe_from_mpg123[0];

  printf("mpg123 started in remote mode (pid: %d)\n", mpg123_pid);
  return 0;
}

void load_file(char *filename) {
    if (mpg123_stdin == -1) {
        fprintf(stderr, "mpg123 not running\n");
        return;
    }

    char command[BUFFER_SIZE];
    snprintf(command, sizeof(command), "LOAD %s\n", filename);
    write(mpg123_stdin, command, strlen(command));
    printf("loading: %s\n", filename);
}

void stop_playback() {
    if (mpg123_stdin == -1) {
        fprintf(stderr, "mpg123 not running\n");
        return;
    }
    kill(mpg123_pid, SIGTERM);
    waitpid(mpg123_pid, NULL, 0);
    mpg123_pid = -1;
}

void pause_playback() {
    if (mpg123_stdin == -1) {
        fprintf(stderr, "mpg123 not running\n");
        return;
    }
    write(mpg123_stdin, "PAUSE\n", 6);
    printf("paused\n");
}

void cleanup(int sig) {
    stop_playback();
    printf("\nexiting.\n");
    exit(0);
}

void set_volume(int percent) {
    if (mpg123_stdin == -1) {
        fprintf(stderr, "mpg123 not running\n");
        return;
    }
    if (percent < 0) {
      printf("volume cannot negative, setting volume to 0");
      percent = 0;
    }
    if (percent > 200) printf("be careful of volume over 200% \n");
    char command[32];
    snprintf(command, sizeof(command), "V %d\n", percent);
    write(mpg123_stdin, command, strlen(command));
    printf("Volume set to %d%%\n", percent);
}
