#include "./../include/socketutil.h"

struct AcceptedSocket * acceptIncommingConnection(int serverSocketFD);
void * readDataAndPrint(void * _clientSocket);
void listenConnectionAndCreateThread(int serverSocketFD);
void sendToOtherSocket( struct AcceptedSocket * clientSocket, char * buffer);

void removeSocket(struct AcceptedSocket * clientSocket);
struct userInf
{
    char name[1024];
};
struct AcceptedSocket
{
    struct sockaddr_in clientAddress;
    int clientSocketFD;
    bool error;
    int acceptSuccessfully;
    struct userInf user;
};
struct AcceptedSocket acceptedSockets[10];
int acceptedSocketsCount = 0;
int main() {
    int serverSocketFD = createTCPIPv4Socket();
    int port = 1801;
    struct sockaddr_in * serverAddress = createIPv4Address("192.168.1.109", port);

    int result = bind(serverSocketFD, (struct sockaddr *)(serverAddress), sizeof(*serverAddress));
    if(result == 0) printf("Socket was bound successfully at port %d\n", port);
    else {
        printf("can't bind at port %d \n", port);
        return 0;
    };

    int listenResult = listen(serverSocketFD, 10);

    listenConnectionAndCreateThread(serverSocketFD);

    shutdown(serverSocketFD, SHUT_RDWR);
    return 0;
};

void listenConnectionAndCreateThread(int serverSocketFD) {
    while(1) {
        struct AcceptedSocket * clientSocket = acceptIncommingConnection(serverSocketFD);
        acceptedSockets[acceptedSocketsCount++] = *clientSocket;
        pthread_t id;
        pthread_create(&id, NULL, readDataAndPrint, clientSocket);
    };
};

void* readDataAndPrint(void * _clientSocket) {
    struct AcceptedSocket * clientSocket = _clientSocket;
    char buffer[1024];
    ssize_t recieveAmount;
    char message[1024];
    // get name
    recieveAmount = recv(clientSocket->clientSocketFD, buffer, 1024, 0);
    buffer[recieveAmount - 1] = '\0';

    strcpy(clientSocket->user.name, buffer); 
    memset(message, 0, 1024);
    strcat(message, clientSocket->user.name);
    strcat(message, " joined the group chat!\n");

    sendToOtherSocket(clientSocket, " joined the group chat!\n");
    printf("%s",message);
    /* int result =  send(clientSocket->clientSocketFD, buffer, sizeof buffer, 0); */
    /* printf("send result:%d", result); */
    while(1) {
        recieveAmount = recv(clientSocket->clientSocketFD, buffer, 1024, 0);
        if(recieveAmount > 0) {
            buffer[recieveAmount] = '\0';
            sendToOtherSocket(clientSocket, buffer);

            memset(message, 0, 1024);
            strcat(message, clientSocket->user.name);
            strcat(message, ":");
            strcat(message, buffer);
            printf("%s", message);

            /* send(clientSocket->clientSocketFD, buffer, sizeof buffer, 0); */
        } else {
            break;
        }
    };
    memset(message, 0, 1024);
    strcat(message, clientSocket->user.name);
    strcat(message, " left the group chat!\n");
    sendToOtherSocket(clientSocket, " left the group chat!\n");
    printf("%s", message);
    removeSocket(clientSocket);
    close(clientSocket->clientSocketFD);
    return NULL;
}
void removeSocket(struct AcceptedSocket * clientSocket){
    bool flag = false;
    for(int i = 0; i < acceptedSocketsCount - 1 ; i++){
        if(acceptedSockets[i].clientSocketFD == clientSocket->clientSocketFD){
            flag = true;
        };
        if(flag == true){
            acceptedSockets[i] = acceptedSockets[i+1];
        };
    };
    acceptedSocketsCount--;
};
void sendToOtherSocket( struct AcceptedSocket * clientSocket, char * buffer){
    char message[1024];
    for(int i = 0; i < acceptedSocketsCount; i++){
        if(acceptedSockets[i].clientSocketFD != clientSocket->clientSocketFD){
            memset(message, 0, 1024);
            strcat(message, clientSocket->user.name);
            strcat(message, ":");
            strcat(message, buffer);
            int a = send(acceptedSockets[i].clientSocketFD, message, sizeof message, 0);
        };
    };
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
