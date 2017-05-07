#ifdef __cplusplus
extern "C" {
#endif


#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <netdb.h>

// Creates a socket with the clientfd, then populates host_addr
// with the IP and Port # of the host
void ClientSocket(char* hostip, char* port, int* clientfd, struct sockaddr_in* host_addr);

// Creates a socket with the hostfd, populates host_addr with the
// Port # and IP of the computer running this, then binds the socket
// with the host_addr information
void HostSocket(char* port, int* hostfd, struct sockaddr_in* host_addr);

// Connects to the host described by host_addr, binding the
// hostfd socket with the information from host_addr
// Returns a 0 on success, -1 on failure
int ConnectToHost(int* hostfd, struct sockaddr_in* host_addr);

// Uses the hostfd to listen for any incoming connections
// Accept the first connection to the host, storing the client's
// information in client_addr and creating a new socket file descriptor,
// storing that in clientfd
void ListenForConn(int* hostfd, int* clientfd, struct sockaddr_in* client_addr);

#endif


#ifdef __cplusplus
}
#endif