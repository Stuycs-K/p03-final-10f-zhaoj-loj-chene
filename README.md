[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QfAIDTGi)
# Music Player Manager

### ChickenNuggies

Elizabeth Chen, Jason Zhao, Jayden Lo

### Project Description:

Music player using mpg123 that supports multiple users and stores their playlists and data over multiple uses. Runs on terminal with features such as user logins, recommendations, and basic commands like pause and skip.

### Instructions:

1. make compile
2. run ./server in one window and ./client in another (or multiple other windows)
3. crt to create an account if no account exists, lgn to log in to existing account (user data is saved into user.dat)
4. to create a playlist, type "make <playlist_name>" (there's a max of 5 playlists per user)
5. to view all playlists (created) user has, type "playlists"
6. to add a song to playlist, type "add <playlist_name> <song_name.mp3>"
7. to view the songs in a specific playlist, type "view <playlist_name>"
8. to play a song, type "play <song_name.mp3>"
9. to play all the songs in a playlist, type "play playlist <playlist_name>"
10. to delete a song from a playlist, type "delete <playlist_name> <song_name.mp3>"
11. to delete a playlist, type "delete <playlist_name>"
12. to save any changed made to your account and log out (client is killed), type "exit"
13. once exited, user can sign back in during another session
14. to delete an account, user must be signed in and type "delete account" 

### Resources/ References:

mpg123 audio player

Link to demo: https://drive.google.com/file/d/1PwRpp8W0qre1c_k-B27mNGwFye19ZLbm/view?usp=sharing 
