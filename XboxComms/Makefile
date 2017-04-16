# Programs

CC = gcc

all:    Client Server

Client: Client.o
	$(CC) $(LDFLAGS) -std=c99 -o $@ $^

Server: Server.o
	$(CC) $(LDFLAGS) -std=c99 -o $@ $^

# Objects
%.o: %.c
	$(CC) $(CFLAGS) -std=c99 -c $< -o $@

clean:
	rm Client Client.o Server Server.o
