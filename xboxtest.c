#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/un.h>
#include <signal.h>
#include <linux/joystick.h>
#include <math.h>

#include "JSBuffer.h"
#include "Graph.h"

#define MAXAXIS 32767
#define DEADZONELX (int)(0.25*MAXAXIS)
#define DEADZONELY (int)(0.25*MAXAXIS)
#define PI 3.14159265

JSBuffer LX = {{0,0,0,0,0}, 0, 0};
JSBuffer LY = {{0,0,0,0,0}, 0, 0};

JSBuffer RX = {{0,0,0,0,0}, 0, 0};
JSBuffer RY = {{0,0,0,0,0}, 0, 0};

int lastLX = 0;
int lastLY = 0;
int lastRX = 0;
int lastRY = 0;

void main(void) {

    struct js_event xbox;

    printf("Setting up the Xbox controller...\n");
    
    int fd = open("/dev/input/js2", O_RDONLY | O_NONBLOCK);
    if(fd == -1) {
        perror("failed to open xbox controller\n");
        exit(1);
    }

    printf("Completed Xbox controller setup.\n");

    int axes = 0, buttons = 0;
    char name[128];
    ioctl(fd, JSIOCGAXES, &axes);
    ioctl(fd, JSIOCGBUTTONS, &buttons);
    ioctl(fd, JSIOCGNAME(sizeof(name)), &name);
    printf("%s\n    %d Axes %d Buttons\n", name, axes, buttons);


	// Create graph
	//makeGraph();

	// TESTING INPUT HERE
	//printf("Reading input...\n");
	
	int avgLX = 0;
	int avgLY = 0;
    double scaledLX = 0.0;
    double scaledLY = 0.0;
    int angleLX = 0;
    int angleLY = 0;
    double angle = 0;
    double dir = 0;

    while(1) {

		int a = read(fd, &xbox, sizeof(xbox));

		// Read the input
		if(a == -1) {
			
        } else {
        	if(xbox.type == 2) { // reading JS input
        		if(xbox.number == 0) {
        			lastLX = xbox.value;
        		} else if(xbox.number == 1) {
        			lastLY = xbox.value;
        		}
        	} else if(xbox.type == 1) {
        		if(xbox.number == 3) {
        			break;
        		}
        	}
        }
        
        push(&LX, lastLX);
        push(&LY, lastLY);
        
        avgLX = average(&LX);
        avgLY = (-1) * average(&LY);

        // Calculate the scaled values and values to compute angle
        if(abs(avgLX) >= DEADZONELX) {
            if(avgLX > 0) {
                scaledLX = (1.0*(avgLX-DEADZONELX) / (MAXAXIS-DEADZONELX));
                angleLX = avgLX - DEADZONELX;
            } else {
                scaledLX = (1.0*(avgLX+DEADZONELX) / (MAXAXIS-DEADZONELX));
                angleLX = avgLX + DEADZONELX;
            }
        } else {
            scaledLX = 0;
            angleLX = 0;
        }

        if(abs(avgLY) >= DEADZONELY) {
            if(avgLY > 0) {
                scaledLY = (1.0*(avgLY-DEADZONELY) / (MAXAXIS-DEADZONELY));
                angleLY = avgLY - DEADZONELY;
            } else {
                scaledLY = (1.0*(avgLY+DEADZONELY) / (MAXAXIS-DEADZONELY));
                angleLY = avgLY + DEADZONELY;
            }
        } else {
            scaledLY = 0;
            angleLY = 0;
        }



        // Compute angle and direction
        angle = atan2(angleLY, angleLX) * 180.0 / PI;
        dir = sqrt(angleLX*angleLX + angleLY*angleLY);



        printf("X-Axis: %f      Y-Axis: %f      ", scaledLX, scaledLY);
        printf("Angle: %f       Direction: %f\n", angle, dir);
        
        //plot(average(&LX), average(&LY));
        // Do stuff with the input

    }
    //printf("Finished reading input. Printing graph...\n");
    //printGraph();
}




