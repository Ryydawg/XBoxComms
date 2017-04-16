#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

// Xbox libraries
#include <netdb.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/un.h>
#include <signal.h>
#include <linux/joystick.h>


#define MYPORT 8000     // the port users will be connecting to
#define BACKLOG 1      // how many pending connections queue will hold

// socket vars
int sockfd, newfd;      // listen on sockfd, new connection on newfd
struct sockaddr_in my_addr;     // my address information
struct sockaddr_in their_addr;  // connector's address information
int sin_size;
int yes=1;

int average = 0;

void xbox_setup(int* fd) {
    *fd = open("/dev/input/js2", O_RDONLY | O_NONBLOCK);
    if(*fd == -1) {
        perror("Could not find xbox controller at /dev/input/js2");
        exit(1);
    }
    int axes=0, buttons=0;
    char name[128];
    ioctl(*fd, JSIOCGAXES, &axes);
    ioctl(*fd, JSIOCGBUTTONS, &buttons);
    ioctl(*fd, JSIOCGNAME(sizeof(name)), &name);
    printf("%s\n    %d Axes %d Buttons\n", name, axes, buttons);

    return;
}

void socket_setup(void) {

    // SOCKET SETUP
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("creating socket");
        exit(1);
    }
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setting socket option");
        exit(1);
    }
    my_addr.sin_family = AF_INET;           // set address family
    my_addr.sin_port = htons(MYPORT);       // set port number
    my_addr.sin_addr.s_addr = INADDR_ANY;   // autofill with my ip
    memset(&(my_addr.sin_zero), '\0', 8);   // zero the rest of the struct
	
    printf("Setup Complete. Binding socket...\n");

    // bind the listen socket to my address information
    if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    printf("Binding complete. Listening for connections...\n");

    // listen on the port
    if(listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    // accept connection
    sin_size = sizeof(struct sockaddr_in);
    if((newfd = accept(sockfd,(struct sockaddr*)&their_addr,&sin_size)) == -1) {
        perror("accept");
    }
    printf("Connection received from: %s\n", inet_ntoa(their_addr.sin_addr));

    return;
}

void Xbox_Input(int *xbox_fd, struct js_event *xbox) {

    int value = 0;

    if(read(*xbox_fd, xbox, sizeof(*xbox)) == -1) {
        if(errno != EAGAIN) {
            perror("error reading xbox controller");
            exit(1);
        }


        if(average < 4) {
            // add joystick values to average
        }

    } else {

        int type = xbox.type;
        int axis = xbox.number;

        if(type == 2) {
            switch(axis) {
            case 0: // LJ X

            case 1: // LJ Y

            case 3: // RJ X

            case 4: // RJ Y

            default:
                break;
        }
    }
    return;
}

double scale(int num) {
    return (double)num / 32767;
}

void MasterSend(int val) {

    double speed = scale(val);

    if(send(newfd, &speed, sizeof(speed), 0) == -1)
        perror("ERROR reading from new socket\n");
    return;
}

void main(void) {
    // Xbox vars
    struct js_event xbox;
    int xbox_fd = -1;

    // set up connections
    printf("Preparing Xbox Controller setup...\n");
    xbox_setup(&xbox_fd);
    printf("Xbox controller connected.\n");

    printf("\nPreparing socket setup...\n");
    socket_setup();


    // connections established. listen to xbox
    printf("\nNow listening for xbox input\n\n");
    while(1) {
        Xbox_Input(&xbox_fd, &xbox);
    }

    return;
}
 
