#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"

FILE* gnuplotPipe;
FILE* temp;

char* commands[] = { "set terminal png",
					"set output 'foo.png'",
					"plot 'data.bin' u 1:2 every 5 with points"};

void makeGraph() {

	temp = fopen("data.bin", "w");
	gnuplotPipe = popen("gnuplot -persistent", "w");
	
	return;
}

void plot(int x, int y) {
	fprintf(temp, "%d %d\n", x, y);
}

void printGraph() {

	fprintf(gnuplotPipe, "%s \n", commands[0]);
	fprintf(gnuplotPipe, "%s \n", commands[1]);
	fprintf(gnuplotPipe, "%s \n", commands[2]);
	
	fflush(gnuplotPipe);
	
	fclose(temp);
	pclose(gnuplotPipe);
	
	return;
}
