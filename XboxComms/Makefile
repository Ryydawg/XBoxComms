# Programs

CC = gcc

all:    Client Server

Client: ClientConnections.o Client.o
	$(CC) -o Client ClientConnections.o Client.o

Server: ServerConnections.o XboxInput.o Server.o
	$(CC) -o Server ServerConnections.o XboxInput.o Server.o -lm

# Objects
ClientConnections.o: ClientConnections.c
	$(CC) -c ClientConnections.c -o ClientConnections.o

Client.o: Client.c
	$(CC) -c Client.c -o Client.o

ServerConnections.o: ServerConnections.c
	$(CC) -c ServerConnections.c -o ServerConnections.o

XboxInput.o: XboxInput.c
	$(CC) -c XboxInput.c -o XboxInput.o

Server.o: Server.c
	$(CC) -c Server.c -o Server.o

clean:
	rm Client ClientConnections.o Client.o Server ServerConnections.o XboxInput.o Server.o
