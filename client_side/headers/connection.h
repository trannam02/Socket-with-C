#ifndef CONNECTION_H
#define CONNECTION_H
#include "./../../include/socketutil.h"

void listenResponseAtOtherThread(int * sockFD);
void sendReqest(int socketFD);

void setName(int sockFD);
void * listenResponse(void * sockFD);

#endif
