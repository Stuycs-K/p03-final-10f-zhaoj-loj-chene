#include "networking.h"

extern pid_t mpg123_pid;
extern int mpg123_stdin;  // pipe to mpg123s stdin
extern int mpg123_stdout; // pipe from mpg123s stdout

void stop_playback();
void pause_playback();
int start_mpg123_remote();
void load_file(char *filename);
void cleanup(int sig);
void  set_volume(int percent);
