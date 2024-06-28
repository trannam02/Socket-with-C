#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H

#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
int createTCPIPv4Socket();
struct sockaddr_in * createIPv4Address(char * ip, int port);


#endif
