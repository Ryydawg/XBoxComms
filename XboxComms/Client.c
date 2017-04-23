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

int mysockfd;
struct sockaddr_in serv_addr;
struct hostent *he; // Host info

double input[17];

// Sets up a socket on a predefined port, then connects to a predefined
// IP address
void setup(char* hostip, char* hostport) {
    printf("Setting up socket...\n");
    if((mysockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Failed to create socket.\n");
        exit(0);
    }

    he = gethostbyname(hostip);
    if(he == NULL) {
        perror("ERROR: No host\n");
        exit(0);
    }

    // populate the server info
    serv_addr.sin_family = AF_INET; // host byte order
    serv_addr.sin_port = htons(atoi(hostport)); // port # of host
    serv_addr.sin_addr = *((struct in_addr*)he->h_addr_list[0]); // address of host
    memset(&(serv_addr.sin_zero), '\0', 8); // zero the rest of the struct

    printf("Socket setup complete.\nConnecting to host...\n");
    if( connect(mysockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1) {
        perror("Failed to connect\n");
        return;
    }
    printf("Connected. Receiving bytes...\n");
}

// Gets the data from the host then prints out the values
void MasterReceive(void) {
    int bytes;
    int i;

    if( (bytes = recv(mysockfd, input, sizeof(input), 0)) == -1) {
        perror("Failed to receive bytes from server");
        exit(0);
    }

    printf("Speed: %f       Angle: %f\n", input[0], input[1]);
    for(i = 2; i < 17; i++) {
        if(input[i] != 0) {
            printf("Button  %d  On\n", i);
        }
    }
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

    // set up socket communication
    setup(argv[1], argv[2]);
    printf("\nSetup Complete.\n");

    while(1) {
        MasterReceive();
        usleep(500);
    }
}
