#ifndef XBOXINPUT_H_
#define XBOXINPUT_H_

// Holds five values corresponding to the joystick position
// with the most recent value at the front and the least recent
// at the back.
typedef struct JSBuffer {
	int vals[5];
} JSBuffer;

void push(JSBuffer *buff, int n);

int average(JSBuffer *buff);

void scaleVals(JSBuffer* LX, JSBuffer* LY, double* drive);

void readInput(int* xboxfd, JSBuffer* LX, JSBuffer* LY, double* drive, int* buttons);

#endif