# Programs

CC = gcc

all:    Client Server

Client: Connections.o Client.o
	$(CC) -o Client Connections.o Client.o

Server: Connections.o XboxInput.o Server.o
	$(CC) -o Server Connections.o XboxInput.o Server.o -lm

# Objects

Client.o: Client.c
	$(CC) -c Client.c -o Client.o

Connections.o: Connections.c
	$(CC) -c Connections.c -o Connections.o

XboxInput.o: XboxInput.c
	$(CC) -c XboxInput.c -o XboxInput.o

Server.o: Server.c
	$(CC) -c Server.c -o Server.o

clean:
	rm Client Client.o Server XboxInput.o Server.o Connections.o
