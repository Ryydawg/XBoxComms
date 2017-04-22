#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <math.h>

#include "XboxInput.h"

#define MAXAXIS 32767
#define DEADZONELX (int)(0.25*MAXAXIS)
#define DEADZONELY (int)(0.25*MAXAXIS)
#define PI 3.14159265

struct js_event xbox;

int lastLX = 0;
int lastLY = 0;

int avgLX = 0;
int avgLY = 0;
double scaledLX = 0.0;
double scaledLY = 0.0;
int angleLX = 0;
int angleLY = 0;

void push(JSBuffer *buff, int n) {
	buff->vals[buff->pos] = n;

	if(buff->pos == 4)
		buff->pos = 0;
	else
		buff->pos = buff->pos + 1;

	return;
}

int average(JSBuffer *buff) {
	buff->average = (double)((buff->vals[0]) + 4*(buff->vals[1]) + 6*(buff->vals[2])
					+ 4*(buff->vals[3]) + (buff->vals[4])) / 16.0;
	return buff->average;
}

void scaleVals(JSBuffer* LX, JSBuffer* LY, double* drive) {
	push(LX, lastLX);
	push(LY, lastLY);

	avgLX = average(LX);
	avgLY = (-1) * average(LY);

	// Calculate the scaled values
	if(abs(avgLX) >= DEADZONELX) {
		if(avgLX > 0) {
			scaledLX = (1.0*(avgLX - DEADZONELX) / (MAXAXIS-DEADZONELX));
			angleLX = avgLX - DEADZONELX;
		} else {
			scaledLX = (1.0*(avgLX + DEADZONELX) / (MAXAXIS-DEADZONELX));
			angleLX = avgLX + DEADZONELX;
		}
	} else {
		scaledLX = 0;
		angleLX = 0;
	}

	if(abs(avgLY) >= DEADZONELY) {
		if(avgLY > 0) {
			scaledLY = (1.0*(avgLY - DEADZONELY) / (MAXAXIS-DEADZONELY));
			angleLY = avgLY - DEADZONELY;
		} else {
			scaledLY = (1.0*(avgLY + DEADZONELY) / (MAXAXIS-DEADZONELY));
			angleLY = avgLY + DEADZONELY;
		}
	} else {
		scaledLY = 0;
		angleLY = 0;
	}

	double speed = sqrt(scaledLX*scaledLX + scaledLY*scaledLY);
	if(speed > 1.0) { *drive = 1.0; }
	else { *drive = speed; }

	*(drive+1) = atan2(angleLY, angleLX) * 180.0 / PI;

	return;
}

void readInput(int* xboxfd, JSBuffer* LX, JSBuffer* LY, double* drive, int* buttons) {

	int a = read(*xboxfd, &xbox, sizeof(xbox));

	if(a == -1) {

	} else {
		if(xbox.type == 2) { // JOYSTICK
			if(xbox.number == 0) {
				lastLX = xbox.value;
			} else if(xbox.number == 1) {
				lastLY = xbox.value;
			}
		} else if(xbox.type == 1) { // BUTTONS
			*(buttons + xbox.number) = xbox.value;
		}
	}

	scaleVals(LX, LY, drive);
	return;
}