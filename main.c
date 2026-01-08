#include "mpg123-1.33.4/src/mpg123.c"
#include <stdio.h>

int main(int argc, char *argv[]) {
    mpg123_init();
    mpg123_handle *mh = mpg123_new(NULL, NULL);
    mpg123_open(mh, "male-death-scream-horror-352706.mp3");
    mpg123_play(mh);
    mpg123_delete(mh);
    mpg123_exit();
    return 0;
}
