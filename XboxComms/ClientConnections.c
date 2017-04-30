#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "ClientConnections.h"


void client_socket_setup(char* hostip, char* hostport, int* sockfd,
						struct sockaddr_in* serv_addr, struct hostent* he) {

    if((*sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Failed to create socket.\n");
        exit(0);
    }

    he = gethostbyname(hostip);
    if(he == NULL) {
        perror("ERROR: No host\n");
        exit(0);
    }

    // populate the server info
    serv_addr->sin_family = AF_INET; // host byte order
    serv_addr->sin_port = htons(atoi(hostport)); // port # of host
    serv_addr->sin_addr = *((struct in_addr*)he->h_addr_list[0]); // address of host
    memset(&(serv_addr->sin_zero), '\0', 8); // zero the rest of the struct

    printf("Socket created.\nConnecting to host...\n");
    if( connect(*sockfd, (struct sockaddr *)serv_addr, sizeof(struct sockaddr)) == -1) {
        perror("Failed to connect\n");
        return;
    }
    printf("Connected.\n");

}
