#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <linux/joystick.h>

#include "Connections.h"
#include "XboxInput.h"

struct sockaddr_in myaddr;
struct sockaddr_in their_addr;

JSBuffer LX = {{0,0,0,0,0}, 0, 0};
JSBuffer LY = {{0,0,0,0,0}, 0, 0};

double* drive;
int* buttons;
double inputs[17];

void MasterSend(int fd) {

    inputs[0] = *drive;
    inputs[1] = *(drive+1);
    for(int i = 2; i < 17; i++) {
        inputs[i] = *(buttons+i-2);
    }

    if(send(fd, inputs, sizeof(inputs), 0) == -1 ) {
        free(drive);
        free(buttons);
        perror("failed to send values");
    }
    return;
}

void main(void) {

    int xboxfd;
    int sockfd;
    int newfd;

    drive = malloc(2 * sizeof(double));
    buttons = malloc(15 * sizeof(int));

// SETUP
    printf("Performing xbox setup...\n");
    xbox_setup(&xboxfd);
    printf("\nCompleted xbox setup\n");

    printf("\nPerforming connection setup...\n\n");
    socket_setup(&sockfd, &newfd, &myaddr, &their_addr);
    printf("\nCompleted connection setup\n");
// DONE SETUP


    printf("\n\nReading input\n");
    while(1) {
        readInput(&xboxfd, &LX, &LY, drive, buttons);

        //printf("Speed %f            angle %f\n", *drive, *(drive+1));

        usleep(1000);

        MasterSend(newfd);
    }
    return;    
}