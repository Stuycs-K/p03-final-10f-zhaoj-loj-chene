#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include "account.h"

#define BUFFER_SIZE 1024

extern pid_t mpg123_pid;
extern int mpg123_stdin;  // pipe to mpg123s stdin
extern int mpg123_stdout; // pipe from mpg123s stdout

void stop_playback();
int start_mpg123_remote();
void load_file(const char *filename)
void cleanup(int sig);
void  set_volume(int percent);
