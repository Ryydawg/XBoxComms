#ifdef __cplusplus
extern "C" {
#endif

#ifndef XBOXINPUT_H
#define XBOXINPUT_H

void xbox_setup(int* fd, char* path);

void push(int buff[], int n);

int average(int buff[]);

void scaleVals(int LX[], int LY[], double* drive);

void readInput(int* xboxfd, int LX[], int LY[], double* drive, int* buttons);

#endif

#ifdef __cplusplus
}
#endif