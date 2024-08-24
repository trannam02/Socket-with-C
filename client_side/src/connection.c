#include "./../headers/connection.h"
void sendReqest(int socketFD){
    char * line = NULL;
    size_t lineSize = 0;
    while(1) {
        ssize_t numChar = getline(&line, &lineSize, stdin);
        if(numChar > 0){
            // CODE HERE
            send(socketFD,line,numChar, 0);
        };
    }
};

void * listenResponse(void * _sockFD){
    int * sockFD = _sockFD;
    char buffer[1024];
    while(1) {
        ssize_t recieveAmount = recv(*sockFD, buffer, 1024, 0);
        if(recieveAmount > 0) {
            buffer[recieveAmount] = '\0';
            // CODE HERE
            printf("%s", buffer);
        }else{
            printf("Disconnect to server!");
            break;
        };
    };
    close(*sockFD);
    return NULL;
}
void setName(int sockFD){
    printf("Enter your name:");
    char * line = NULL;
    size_t lineSize;
    int numChar = 0;
    while(1){
        numChar = getline(&line, &lineSize, stdin);
        /* line[numChar - 1] = '\0'; */
        if(numChar > 1) break;
    };
    send(sockFD, line, numChar, 0); 
};
void listenResponseAtOtherThread(int * sockFD){
    pthread_t id;
    int result = pthread_create(&id, NULL, listenResponse, sockFD);
};


