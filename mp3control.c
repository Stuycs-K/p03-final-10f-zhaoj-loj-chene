#include "mp3control.h"

pid_t mpg123_pid = -1;
int mpg123_stdin = -1;
int mpg123_stdout = -1;

int start_mpg123_remote() {
}

void load_file(const char *filename) {
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
