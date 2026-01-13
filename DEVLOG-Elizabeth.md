# Dev Log:

This document must be updated daily every time you finish a work session.

## Elizabeth Chen

### 2026-01-06 - project proposal
Pushed project proposal into repo

### 2026-01-07 - mpg123 documentation
- read through mpg123 documentation
- start testing how to use integrate mpg123 into program by trying to run commands  

### 2026-01-08 - mpg123 usage
- successfully integrated mpg123 into programming
- tested running whoosh.mp3

### 2026-01-09 - network logic
- server and client networking (mostly previous lab)
- started parsing input from terminal (unfinished)

### 2026-01-11 - server-client logic + play music
- updated server to check user authentication (reordered user prompting function originally in account.c to server.c) + subserver logic to check user input (play and exit) to play music with mpg123 (2 hr)
- fixed client logic to just connect to server (doesn't send any messages) (30 min)
- tested and fixed infinite loop error that occurred after logging in (30 min)

### 2026-01-12 - testing player
- testing new server logic: old version played on server but now creates temp mp3 file for client (20 min)
- update makefile to remove temp file each time (5 min)
- remove debug print statements (bytes copied, etc.) (5 min)

### 2026-01-13 - exit command + account creation verification
- output for successful account creation (5 min)
- debug naming typos and type warnings (5 min)
- fix exit command to send to redirect to client and exit client (15 min)
- test (10 min)
