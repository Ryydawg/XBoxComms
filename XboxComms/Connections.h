#ifndef CONNECTIONS_H_
#define CONNECTIONS_H_

#include <netdb.h>

void xbox_setup(int* fd);

void socket_setup(int* sockfd, int* newfd, struct sockaddr_in* my_addr, struct sockaddr_in* their_addr);

#endif