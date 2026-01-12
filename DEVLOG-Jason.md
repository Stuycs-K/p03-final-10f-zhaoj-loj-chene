# Dev Log:

This document must be updated daily every time you finish a work session.

## Jason

### 2024-01-06 - proposal, server client connection
Finalizing proposal
Start on Server-Client socket connections

### 2024-01-07 - server client connection
Copy Server-Client socket connection from prev lab, works

### 2024-01-08 - mpg123 library download and testing
Analyzing downloaded mpg123 library
Execvp does work but it only executes the file, it can't pause or have volume control or stuff
Tried different ways to use the library
  - Use as external program is promising, deleting the mpg123 download
sudo apt-get install mpg123 on personal computers

### 2024-01-09 - volume adjustments using mpg123
dpkg -s mpg123 to check version
volume adjustments works through terminal commands (2 hrs, including frees)

### 2024-01-11 - merging main to jason, compiling, other fixes, revamp the mp3 client server system
merged everything from main to jason, compile works and so does test file (1 hr)
notes from my idea of music player continuing:
- maybe have clients download only client and like networking and the necessary stuff, just not server and music stuff so that server can just send the mp3 data to client to run
  - could probably work by having client connect to  server ip
- this way server jsut manages the library and client does the mpg123 stuff
before, the server's child tried to play the music, fixed so that the child receives the mp3 file and saves as temp to then play on the clients side (1.5 hr)
- comments included on what each part does
server now sends mp3 file to client and client copies onto a temp.mp3 on its own folder (1.5 hr)
now use that to combine with mpg123 with volume adjusting, pausing, etc

### 2024-01-12 - audio control, pause, volume, playlists?
