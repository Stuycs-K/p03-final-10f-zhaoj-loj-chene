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

        execlp(
            "mpg123",
            "mpg123",
            "-f", scale_arg,
            mp3_file,
            NULL
        );

        perror("mpg123 failed");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <mp3_file>\n", argv[0]);
        return 1;
    }

    mp3_file = argv[1];
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
