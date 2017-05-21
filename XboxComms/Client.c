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

#include "Connections.h"

double input[17];

void MasterSend(int fd) {
    char* string = "Hello world!";
    if(send(fd, string, sizeof(string), 0) == -1) {
        perror("failed to send values");
    }
}

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
    

    
    for(i = 2; i < 17; i++) {
        if(input[i] != 0) {
            printf("Button  %d  On\n", i);
        }
    }
    

    //MasterSend(sockfd);
}



int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("Invalid number of inputs. Please follow the format:\n");
        printf("\n./Server <IP address> <portno>\n\n");
        printf("where the IP address is the IP of the host and the\n");
        printf("portno is the port number the connection will be made.\n");
        return -1;
    }

    // Vars for recv xbox input
    int sockfd;
    struct sockaddr_in serv_addr;


    // Create a socket connection for receiving Xbox input
    printf("Creating client socket...\n");
    ClientSocket(argv[1], argv[2], &sockfd, &serv_addr);
    printf("Created.\n");

    printf("\nConnecting to host...\n");
    while(1) {
        if(ConnectToHost(&sockfd, &serv_addr) == -1) {
            perror("ERROR: Failed to connect");
        } else break;
    }
    printf("Connected!\n");
    


    // read sent data
    printf("\n\nReceiving bytes...\n");
    while(1) {
        MasterReceive(sockfd);
        usleep(1000);
        //MasterSend(sockfd);
    }
    return 0;
}