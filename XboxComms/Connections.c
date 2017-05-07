#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>

#include "Connections.h"

#define BACKLOG 5

// Creates a socket with the hostfd, then populates host_addr
// with the IP and Port # of the host
void ClientSocket(char* hostip, char* port, int* clientfd,
                                    struct sockaddr_in* host_addr) {
    struct hostent* he;

    if((*clientfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Failed to create socket.\n");
        exit(0);
    }

    he = gethostbyname(hostip);
    if(he == NULL) {
        perror("ERROR: No host\n");
        exit(0);
    }

    // populate the server info
    host_addr->sin_family = AF_INET; // host byte order
    host_addr->sin_port = htons(atoi(port)); // port # of host
    host_addr->sin_addr = *((struct in_addr*)he->h_addr_list[0]); // address of host
    memset(&(host_addr->sin_zero), '\0', 8); // zero the rest of the struct
}

// Creates a socket with the hostfd, populates host_addr with the
// Port # and IP of the computer running this, then binds the socket
// with the host_addr information
void HostSocket(char* port, int* hostfd, struct sockaddr_in* host_addr) {
	int yes = 1;

    // SOCKET SETUP
    if((*hostfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("creating socket");
        exit(0);
    }
    if(setsockopt(*hostfd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(int)) == -1) {
        perror("setting socket option");
        exit(0);
    }
    host_addr->sin_family = AF_INET;           // set address family
    host_addr->sin_port = htons(atoi(port));   // set port number
    host_addr->sin_addr.s_addr = INADDR_ANY;   // autofill with my ip
    memset(&(host_addr->sin_zero), '\0', 8);   // zero the rest of the struct
	
    // Socket created, binding the socket to the host address information
    if(bind(*hostfd,(struct sockaddr*)host_addr,sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(0);
    }
}

// Connects to the host described by host_addr, binding the
// hostfd socket with the information from host_addr
// Returns a 0 on success, -1 on failure
int ConnectToHost(int* hostfd, struct sockaddr_in* host_addr) {
    return connect(*hostfd, (struct sockaddr *)host_addr, sizeof(struct sockaddr));
}

// Uses the hostfd to listen for any incoming connections
// Accept the first connection to the host, storing the client's
// information in client_addr and creating a new socket file descriptor,
// storing that in clientfd
void ListenForConn(int* hostfd, int* clientfd, struct sockaddr_in* client_addr) {
    // listen on the port
    if(listen(*hostfd, BACKLOG) == -1) {
        perror("listen");
        exit(0);
    }

    // accept connection
    unsigned int sin_size = sizeof(struct sockaddr_in);
    if((*clientfd = accept(*hostfd,(struct sockaddr*)client_addr,&sin_size)) == -1) {
        perror("accept");
    }
    printf("Connection received from: %s\n", inet_ntoa(client_addr->sin_addr));
}