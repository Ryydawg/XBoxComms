#ifndef JSBUFFER_H
#define JSBUFFER_H

typedef struct JSBuffer {
	int vals[5];
	int pos;
	int average;
} JSBuffer;

void push(JSBuffer *buff, int n);

int average(JSBuffer *buff);

#endif
