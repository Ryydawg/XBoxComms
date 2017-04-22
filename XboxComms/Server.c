#include <stdio.h>
#include <stdlib.h>

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

double** inputs;

void MasterSend(int fd) {

    double poop[2];
    poop[0] = *drive;
    poop[1] = *(drive+1);

    if(send(fd, poop, sizeof(poop), 0) == -1 ) {
        perror("failed to send values");
    }
    return;
}

void main(void) {

    int xboxfd;
    int sockfd;
    int newfd;

    double speed;
    double angle;

    drive = malloc(2 * sizeof(double));
    buttons = malloc(15 * sizeof(int));

    inputs = malloc(2 * sizeof(double*));

    *inputs = drive;
    *(inputs + 1) = buttons;

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

        speed = *drive;
        angle = *(drive+1);

        usleep(1000);

        MasterSend(newfd);
    }
    
}