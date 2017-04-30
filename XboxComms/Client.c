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

#include "ClientConnections.h"

double input[17];

// Gets the data from the host then prints out the values
void MasterReceive(int sockfd) {
    int bytes;
    int i;

    if( (bytes = recv(sockfd, input, sizeof(input), 0)) == -1) {
        perror("Failed to receive bytes from server");
        exit(0);
    }

    printf("Speed: %f       Angle: %f\n", input[0], input[1]);

    //input 2-16 are the 0-14 buttons
    // D-PAD: 13(left), 14(right), 15(up), 16(down)
    if(input[13]) printf("moving left\n");
    if(input[14]) printf("moving right\n");
    if(input[15]) printf("moving up\n");
    if(input[16]) printf("moving down\n");

/*
    for(i = 2; i < 17; i++) {
        if(input[i] != 0) {
            printf("Button  %d  On\n", i);
        }
    }
    */
    return;
}

void main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("Invalid number of inputs. Please follow the format:\n");
        printf("\n./Server <IP address> <portno>\n\n");
        printf("where the IP address is the IP of the host and the\n");
        printf("portno is the port number the connection will be made.\n");
        return;
    }

    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *he;




    // set up socket communication
    printf("Setting up socket...\n");
    client_socket_setup(argv[1], argv[2], &sockfd, &serv_addr, he);
    printf("\nSetup Complete. Receiving bytes...\n");

    // read sent data
    while(1) {
        MasterReceive(sockfd);
        usleep(500);
    }
}
