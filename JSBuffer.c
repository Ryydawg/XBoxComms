#include "JSBuffer.h"

void push(JSBuffer *buff, int n) {
	buff->vals[buff->pos] = n;

	if(buff->pos == 4)
		buff->pos = 0;
	else
		buff->pos = buff->pos + 1;
}

int average(JSBuffer *buff) {
	buff->average = (double)((buff->vals[0]) + 4*(buff->vals[1]) + 6*(buff->vals[2])
					+ 4*(buff->vals[3]) + (buff->vals[4])) / 16.0;
	return buff->average;
}
