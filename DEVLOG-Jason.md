# Dev Log:

This document must be updated daily every time you finish a work session.

## Jason

### 2024-01-06 - Brief description
Finalizing proposal
Start on Server-Client socket connections

### 2024-01-07 - Brief description
Copy Server-Client socket connection from prev lab, works

### 2024-01-08 - Brief description
Analyzing downloaded mpg123 library
Execvp does work but it only executes the file, it can't pause or have volume control or stuff
Tried different ways to use the library
  - Cannot sudo apt install on the lab computers so researching work around
    - not successful lol, there is no mpg123 executable in the downloaded library
  - Use as external program
    - needs sudo apt install, will ask in class
sudo apt-get install mpg123
