#include "mp3control.h"

pid_t mpg123_pid = -1;
int mpg123_stdin = -1;
int mpg123_stdout = -1;

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
