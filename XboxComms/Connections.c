#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>

#include <linux/joystick.h>

#include "Connections.h"

#define MYPORT 8000
#define BACKLOG 1

// Opens the xbox controller file descriptor to read the controller input
void xbox_setup(int* fd) {
    *fd = open("/dev/input/js2", O_RDONLY | O_NONBLOCK);
    if(*fd == -1) {
        perror("Could not find xbox controller at /dev/input/js2");
        exit(0);
    }
    // Prints details of the controller
    int axes=0, buttons=0;
    char name[128];
    ioctl(*fd, JSIOCGAXES, &axes);
    ioctl(*fd, JSIOCGBUTTONS, &buttons);
    ioctl(*fd, JSIOCGNAME(sizeof(name)), &name);
    printf("\n%s\n    %d Axes %d Buttons\n", name, axes, buttons);

    return;
}

// Sets up a socket on a predefined port, then waits for a connection
void socket_setup(int* sockfd, int* newfd, struct sockaddr_in* my_addr,
											struct sockaddr_in* their_addr) {
	int yes = 1;

    // SOCKET SETUP
    if((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("creating socket");
        exit(0);
    }
    if(setsockopt(*sockfd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(int)) == -1) {
        perror("setting socket option");
        exit(0);
    }
    my_addr->sin_family = AF_INET;           // set address family
    my_addr->sin_port = htons(MYPORT);       // set port number
    my_addr->sin_addr.s_addr = INADDR_ANY;   // autofill with my ip
    memset(&(my_addr->sin_zero), '\0', 8);   // zero the rest of the struct
	
    printf("Socket created. Binding socket...\n");

    // bind the listen socket to my address information
    if(bind(*sockfd,(struct sockaddr*)my_addr,sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(0);
    }

    printf("Binding complete. Listening for connections...\n");

    // listen on the port
    if(listen(*sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(0);
    }

    // accept connection
    int sin_size = sizeof(struct sockaddr_in);
    if((*newfd = accept(*sockfd,(struct sockaddr*)their_addr,&sin_size)) == -1) {
        perror("accept");
    }
    printf("Connection received from: %s\n", inet_ntoa(their_addr->sin_addr));

    return;
}