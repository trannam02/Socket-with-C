#ifndef CONNECTION_H
#define CONNECTION_H
#include "./../../include/socketutil.h"
// global const
extern int PORT;
extern char IP[20];

struct Room{
    unsigned int id;
    struct User * player[2];
    struct User * viewer[20];
    int * table[16*16];
};
struct User
{
    unsigned int id;
    char userName[1024];
    unsigned int roomId;
};
struct AcceptedSocket
{
    struct sockaddr_in clientAddress;
    int clientSocketFD;
    bool error;
    int acceptSuccessfully;
    struct User user;
};
struct AcceptedSocket * acceptIncommingConnection(int serverSocketFD);
void * listenClientSocket(void * _clientSocket);
void listenConnectionAndCreateThread(int serverSocketFD,struct AcceptedSocket ** acceptedSockets, int* acceptedSocketsCount);
void storeName(struct AcceptedSocket * clientSocket);

void sendToOtherSocket(struct AcceptedSocket * clientSocket, char * buffer, struct AcceptedSocket ** acceptedSockets, int * acceptedSocketsCount);
void removeSocket(struct AcceptedSocket * clientSocket, struct AcceptedSocket ** acceptedSockets, int * acceptedSocketsCount);
#endif
