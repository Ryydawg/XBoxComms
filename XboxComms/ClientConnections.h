#ifdef __cplusplus
extern "C" {
#endif

#ifndef CLIENT_CONNECTIONS_H_
#define CLIENT_CONNECTIONS_H_

#include <netdb.h>

void client_socket_setup(char* hostip, char* hostport, int* sockfd, struct sockaddr_in* serv_addr, struct hostent* he);

#endif

#ifdef __cplusplus
}
#endif