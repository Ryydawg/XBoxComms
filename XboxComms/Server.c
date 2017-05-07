#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <linux/joystick.h>

#include "Connections.h"
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

int main(int argc, char* argv[]) {
    if(argc != 4) {
        printf("Invalid number of inputs. Please follow the format:\n");
        printf("\n./Server <xboxfd path> <portno> <IP>\n\n");
        printf("where the xboxfd path is located at /dev/input/ and the\n");
        printf("portno is the port number the connection will be made\n");
        printf("and IP is the address of the client.\n");
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

    // Vars for recv client data
    int clientfd;
    struct sockaddr_in client_addr;


    printf("Performing xbox setup...\n");
    //xbox_setup(&xboxfd, argv[1]);
    printf("Completed xbox setup\n");

    // Create socket connection for sending Xbox input
    printf("\nCreating host socket...\n");
    HostSocket(argv[2], &hostfd, &myaddr);
    printf("Created.\n");

    // LISTEN FOR CONNECTION
    printf("\nListening for a connection...\n");
    ListenForConn(&hostfd, &newfd, &their_addr);

    usleep(2500000);

    // Create a socket connection for receiving data from client
    printf("Creating client socket...\n");
    ClientSocket(argv[3], "8020", &clientfd, &client_addr);
    printf("Created.\n");

    printf("\nConnecting to host...\n");
    while(1) {
        if(ConnectToHost(&clientfd, &client_addr) == -1) {
            perror("ERROR: Failed to connect");
        } else break;
    }
    printf("Connected!\n");



    printf("\n\nReading input\n");
    while(1) {


        //readInput(&xboxfd, LX, LY, drive, buttons);

        
        //usleep(1000);

        //MasterSend(newfd);
    }
    return 0;
}