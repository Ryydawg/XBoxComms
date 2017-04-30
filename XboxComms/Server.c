#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <linux/joystick.h>

#include "ServerConnections.h"
#include "XboxInput.h"

double* drive;
int* buttons;
double inputs[17];

// Sends the data out
void MasterSend(int fd) {
    int i;

    inputs[0] = *drive;
    inputs[1] = *(drive+1);
    for(i = 2; i < 17; i++) {
        inputs[i] = *(buttons+i-2);
    }

    if(send(fd, inputs, sizeof(inputs), 0) == -1 ) {
        free(drive);
        free(buttons);
        perror("failed to send values");
    }
    return;
}

void main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("Invalid number of inputs. Please follow the format:\n");
        printf("\n./Server <xboxfd path> <portno>\n\n");
        printf("where the xboxfd path is located at /dev/input/ and the\n");
        printf("portno is the port number the connection will be made.\n");
        return;
    }

    int xboxfd;
    int sockfd;
    int newfd;
    struct sockaddr_in myaddr;
    struct sockaddr_in their_addr;

    JSBuffer LX = {{0,0,0,0,0}};
    JSBuffer LY = {{0,0,0,0,0}};

    drive = malloc(2 * sizeof(double));
    buttons = malloc(15 * sizeof(int));

// SETUP
    printf("Performing xbox setup...\n");
    xbox_setup(&xboxfd, argv[1]);
    printf("\nCompleted xbox setup\n");

    printf("\nPerforming connection setup...\n\n");
    server_socket_setup(&sockfd, &newfd, &myaddr, &their_addr, argv[2]);
    printf("\nCompleted connection setup\n");
// DONE SETUP

    printf("\n\nReading input\n");
    while(1) {
        readInput(&xboxfd, &LX, &LY, drive, buttons);
        usleep(1000);

        MasterSend(newfd);
    }
    return;    
}