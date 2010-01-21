#ifndef GESTSOCKET_H
#define GESTSOCKET_H

#include "util.h"

 
#define LISTEN_ERROR -4
#define SETSOCKOPT_ERROR -2
#define BIND_ERROR -3
#define CREATE_SOCKET_ERROR -1

#endif

int inizialize_socket(struct sockaddr_in* server_socket, int port, int* socket_descriptor);
