#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <linux/joystick.h>

#include "Connections.h"
#include "XboxInput.h"

double* drive;
int* buttons;
double inputs[17];
char* string;

void MasterReceive(int fd) {
    int bytes;
    char buffer[512];

    if( (bytes=recv(fd, buffer, sizeof(buffer), 0)) == -1) {
        perror("failed to receive bytes from server");
    }


    printf("Received: %d\n", bytes);
}

// Sends the data out
void MasterSend(int fd) {}
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
    //MasterReceive(fd);
}



int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("Invalid number of inputs. Please follow the format:\n");
        printf("\n./Server <xboxfd path> <portno>\n\n");
        printf("where the xboxfd path is located at /dev/input/ and the\n");
        printf("portno is the port number the connection will be made\n");
        return -1;
    }

    // Vars for sending xbox input
    int xboxfd;
    int hostfd;
    int newfd;
    struct sockaddr_in myaddr;
    struct sockaddr_in their_addr;

    int LX[] = {0,0,0,0,0};
    int LY[] = {0,0,0,0,0};

    drive = (double*)malloc(2 * sizeof(double));
    buttons = (int*)malloc(15 * sizeof(int));

    string = (char*)malloc(12 * sizeof(char));



    printf("Performing xbox setup...\n");
    xbox_setup(&xboxfd, argv[1]);
    printf("Completed xbox setup\n");


    // Create socket connection for sending Xbox input
    printf("\nCreating host socket...\n");
    HostSocket(argv[2], &hostfd, &myaddr);
    printf("Created.\n");

    // LISTEN FOR CONNECTION
    printf("\nListening for a connection...\n");
    ListenForConn(&hostfd, &newfd, &their_addr);


    printf("\n\nReading input\n");
    while(1) {
        readInput(&xboxfd, LX, LY, drive, buttons);
        
        usleep(1000);

        MasterSend(newfd);
    }
    return 0;
}