#include "networking.h"

/*Create and bind a socket.
* Place the socket in a listening state.
* returns the socket descriptor
*/

int server_setup() {
  //setup structs for getaddrinfo
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, PORT, hints, &results);

  //create the socket
  int clientd =  socket(results->ai_family, results->ai_socktype, results->ai_protocol);//store the socket descriptor here

  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  error\n");

  //bind the socket to address and port
  int s = bind(clientd, results->ai_addr, results->ai_addrlen);
  err(s, "bind error\n");

  //set socket to listen state
  int l = listen(clientd, 10);
  err(l, "listen error\n");

  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);

  return clientd;
}

/*Accept a connection from a client
 *return the socket descriptor for the new socket connected to the client
 *blocks until connection is made.
 */
int server_tcp_handshake(int listen_socket){
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);

    //accept() the client connection
    int client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);

    return client_socket;
}


/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;

  //getaddrinfo
  getaddrinfo(server_address, PORT, hints, &results);

  //create the socket
  int serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);//store the socket descriptor here

  //connect() to the server
  connect(serverd, results->ai_addr, results->ai_addrlen);

  free(hints);
  freeaddrinfo(results);
  return serverd;
}

void err(int i, char*message){
  if(i < 0){
      printf("Error: %s - %s\n",message, strerror(errno));
      exit(1);
  }
}
