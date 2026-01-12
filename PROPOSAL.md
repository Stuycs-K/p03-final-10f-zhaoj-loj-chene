# Final Project Proposal: Music Player Extension
## Group Members:
Jason
Jayden
Elizabeth

# Intentions:
Our project is to create a client-server music streaming system. A central server will manage the MP3 music files, as well as user data, and multiple clients will be able to connect at the same time to play music. The system will include account management and password authentication, play history tracking, song recommendations, and shuffle.

# Intended usage:
Client (users):
Terminal interface where users can log in with username and password
Music library menu for browsing and playlist management
Command line prompts to play, pause, skip, shuffle, view history, and get recommendations
Display current song details + playlist/queue
Search to find songs (title or artist)

Server:
Track client log ins
Manage music file and account data storage
Use fork() to handle multiple client connections
Play audio to connected clients  

# Technical Details:
Managing Processes
fork() - multiple client connections
semaphores - shared resource access
signals - server exit

Network programming
Sockets: socket(), bind(), listen(), accept(), connect() for client and server communication
Using program mpg123: pipes
Commands and can send kill signals

File Management and System Calls
file operations: open(), read(), write(), close()
stream audio files
read and write user data base

Directory traversal: opendir(), readdir() for music library scanning

Memory
Dynamic memory allocation with : malloc(), free()

Data structures
playlist & queues: linked lists
song rec: arrays

# Member Responsibilities:
Jason: server and networking
server socket setup and connection handling
fork() multiple clients
parsing messages
audio file streaming

Elizabeth: client & user interface
client socket connection
terminal and command parsing
client side playback
handle received audio data

Jayden: user authentication & database
text format user database
Usernames and passwords
Song recommendations
manage file perms so multiple clients can access data

# Implementation Details:
Audio format: mp3
User database: text file

# Intended pacing:
Days 1-2 (January 6-7)
Project setup: directory structure, Makefile, basic compilation
Make song struct
Header files for major components

Days 3-4 (January 8-9)
Server: server accepting single client connection
Client: client connecting to server
Database: design user struct and file format, implement basic read/write

JAN 10: runnable main

Days 5-6 (January 10-11)
Server: connect multiple clients with fork()
Client: command parsing on terminal
Database: User authentication (login/register)

Days 7-8 (January 12-13)
Server: Audio file streaming
Client: Receive and handle audio data
Playlist: creation and storage


Days 9-10 (January 14-15)
Recommendations: Implement recommendations based on genre of songs in playlist
Shuffle
Testing with all features
Create presentation

Day 11 (January 16)
Final commits with links to presentation
