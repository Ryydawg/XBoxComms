#ifdef __cplusplus
extern "C" {
#endif

#ifndef SERVER_CONNECTIONS_H_
#define SERVER_CONNECTIONS_H_

#include <netdb.h>

void xbox_setup(int* fd, char* path);

void server_socket_setup(int* sockfd, int* newfd, struct sockaddr_in* my_addr, struct sockaddr_in* their_addr, char* port);

#endif

#ifdef __cplusplus
}
#endif