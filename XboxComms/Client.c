#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
//#include "i2c-dev.h"
//#include "i2cbusses.h"


#define HOSTPORT 8000  // the port client will be connecting to

// hostname is the IP address of the host computer
char hostname[] = "192.168.7.1";

// socket file descriptor
int sockfd;

// information about the server
struct sockaddr_in serv_addr;

// host info
struct hostent *he;

/**
 * Sets up socket communication with server
 */
void setup(void) {

    printf("Setting up socket...\n");

    // create socket for network communication
    if( (sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("Failed to create socket.\n");


    // get the host info
    he = gethostbyname(hostname);
    if(he == NULL) {
        printf("ERROR: No host\n");
        exit(1);
    }
    
    // populate the server info
    serv_addr.sin_family = AF_INET;     // host byte order
    serv_addr.sin_port = htons(HOSTPORT);   // port # of host
    serv_addr.sin_addr = *((struct in_addr*)he->h_addr_list[0]);    // address of host
    memset(&(serv_addr.sin_zero), '\0', 8);     // zero the rest of the struct

    printf("Setup complete.\nConnecting to host...\n");


    // try to connect
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1) {
        printf("Failed to connect\n");
        return;
    }

    printf("Connected. Receiving bytes...\n");
    
}

/**
 * Receives the data buffer from server,
 * parses it, and then prints out relevant
 * data
 */
void MasterReceive(void) {
    // buffer to hold received data
    int bytes;

    double speed;

    if( (bytes = recv(sockfd, &speed, sizeof(speed), 0)) == -1) {
        printf("Failed to receive bytes from server");
        exit(1);
    }

    printf("Axis: %f\n", speed);


}

void main() {
    // set up socket communication
    setup();
    printf("\nSetup Complete.\n");

    while(1) {
        MasterReceive();
    }
}
