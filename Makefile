all:
	gcc -g -o xboxtest xboxtest.c JSBuffer.c Graph.c -lm

clean:
	rm xboxtest
