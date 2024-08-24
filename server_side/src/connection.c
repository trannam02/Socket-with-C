#include "./../../include/socketutil.h"
#include "./../headers/connection.h"
// global variable
int PORT = 3000;
char IP[20] = "192.168.1.109";


void* listenClientSocket(void * _clientSocket) {
    struct AcceptedSocket * clientSocket = _clientSocket;
    char buffer[1024];
    ssize_t recieveAmount;
    char message[1024];
    // get name
    storeName(clientSocket);
    // send list room
    while(1) {
        recieveAmount = recv(clientSocket->clientSocketFD, buffer, 1024, 0);
        if(recieveAmount > 0) {
            buffer[recieveAmount] = '\0';
            // create room
                
            // join room
                // is full ?
                // is wrong ID ?
            // chat

            // replay
                // is end ?
            // play X O

            // quit room

            /* sendToOtherSocket(clientSocket, buffer, ); */

            memset(message, 0, 1024);
            strcat(message, clientSocket->user.userName);
            strcat(message, ":");
            strcat(message, buffer);
            printf("%s", message);

            /* send(clientSocket->clientSocketFD, buffer, sizeof buffer, 0); */
        } else {
            break;
        }
    };
    memset(message, 0, 1024);
    strcat(message, clientSocket->user.userName);
    strcat(message, " disconnect to the server!\n");
    /* sendToOtherSocket(clientSocket, " left the group chat!\n"); */
    printf("%s", message);
    // process disconnect
        // check delete room
    /* removeSocket(clientSocket, ); */


    close(clientSocket->clientSocketFD);
    return NULL;
};
struct AcceptedSocket * acceptIncommingConnection(int serverSocketFD) {
    struct sockaddr_in clientAddress;
    unsigned int clientAddressSize;
    int clientSocketFD = accept(serverSocketFD, (struct sockaddr *)(&clientAddress), &clientAddressSize);
    struct AcceptedSocket * acceptedSocket = malloc(sizeof(struct AcceptedSocket));
    acceptedSocket->clientSocketFD = clientSocketFD;
    acceptedSocket->clientAddress = clientAddress;
    acceptedSocket->acceptSuccessfully = (clientSocketFD > 0);
    if( !(clientSocketFD > 0) ) {
        acceptedSocket->error = clientSocketFD;
    }
    return acceptedSocket;
};

void listenConnectionAndCreateThread(int serverSocketFD,struct AcceptedSocket ** acceptedSockets, int * acceptedSocketsCount){
    while(1) {
        struct AcceptedSocket * clientSocket = acceptIncommingConnection(serverSocketFD);
        clientSocket->user.id = clientSocket->clientSocketFD;
        clientSocket->user.roomId = 0;
        acceptedSockets[(*acceptedSocketsCount)++] = clientSocket;

        pthread_t id;
        pthread_create(&id, NULL, listenClientSocket, clientSocket);
    };
};
void storeName(struct AcceptedSocket * clientSocket){
    char buffer[1024];
    ssize_t recieveAmount;
    // get name
    recieveAmount = recv(clientSocket->clientSocketFD, buffer, 1024, 0);
    buffer[recieveAmount - 1] = '\0';
    strcpy(clientSocket->user.userName, buffer); 
};

void removeSocket(struct AcceptedSocket * clientSocket, struct AcceptedSocket ** acceptedSockets, int * acceptedSocketsCount){
    bool flag = false;
    for(int i = 0; i < *acceptedSocketsCount - 1 ; i++){
        if(acceptedSockets[i]->clientSocketFD == clientSocket->clientSocketFD){
            flag = true;
        };
        if(flag == true){
            acceptedSockets[i] = acceptedSockets[i+1];
        };
    };
    acceptedSocketsCount--;
};
void sendToOtherSocket(struct AcceptedSocket * clientSocket, char * buffer, struct AcceptedSocket ** acceptedSockets, int * acceptedSocketsCount){
    char message[1024];
    for(int i = 0; i < *acceptedSocketsCount; i++){
        if(acceptedSockets[i]->clientSocketFD != clientSocket->clientSocketFD){
            memset(message, 0, 1024);
            strcat(message, clientSocket->user.userName);
            strcat(message, ":");
            strcat(message, buffer);
            int a = send(acceptedSockets[i]->clientSocketFD, message, sizeof message, 0);
        };
    };
};
