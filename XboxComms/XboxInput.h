#ifndef XBOXINPUT_H_
#define XBOXINPUT_H_

typedef struct JSBuffer {
	int vals[5];
	int average;
} JSBuffer;

void push(JSBuffer *buff, int n);

int average(JSBuffer *buff);

void scaleVals(JSBuffer* LX, JSBuffer* LY, double* drive);

void readInput(int* xboxfd, JSBuffer* LX, JSBuffer* LY, double* drive, int* buttons);

#endif