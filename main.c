#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t mpg123_pid = -1;

void cleanup(int sig) {
    if (mpg123_pid > 0) {
        kill(mpg123_pid, SIGTERM);
        waitpid(mpg123_pid, NULL, 0);
    }
    printf("\nPlayback stopped.\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <mp3_file>\n", argv[0]);
        return 1;
    }

    signal(SIGINT, cleanup);

    mpg123_pid = fork();

    if (mpg123_pid == -1) {
        perror("fork");
        return 1;
    }

    if (mpg123_pid == 0) {
        execlp("mpg123", "mpg123", argv[1], NULL);

        perror("Failed to run mpg123 - make sure it's installed (sudo apt-get install mpg123)");
        exit(1);
    }
    int status;
    waitpid(mpg123_pid, &status, 0);

    printf("Playback finished.\n");
    return 0;
}
