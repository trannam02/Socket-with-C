#include "./../include/socketutil.h"

void listenResponseAtOtherThread(int * sockFD);
void setName(int sockFD);
void * listenResponse(void * sockFD);

int main() {
    int sockFD = createTCPIPv4Socket();

    struct sockaddr_in * address = createIPv4Address("192.168.1.109", 1801);
    int result = connect(sockFD, (struct sockaddr*)address, sizeof *address);

    if(result == 0) printf("Connect successful!\n");
    else{
        printf("Can't connect to server...");
        close(sockFD);
        return 0;
    };

    setName(sockFD);

    listenResponseAtOtherThread(&sockFD);

    char * line = NULL;
    size_t lineSize = 0;
    while(1) {
        ssize_t numChar = getline(&line, &lineSize, stdin);
        if(numChar > 0){
            send(sockFD,line,numChar, 0);
        };
    }
    close(sockFD);
    return 0;
};
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
    send(sockFD, line, numChar , 0); 
};
void listenResponseAtOtherThread(int * sockFD){
    pthread_t id;
    int result = pthread_create(&id, NULL, listenResponse,  sockFD);
};

void * listenResponse(void * _sockFD){
    int * sockFD = _sockFD;
    char buffer[1024];
    /* ssize_t recieveAmount; */
    while(1) {
        ssize_t recieveAmount = recv(*sockFD, buffer, 1024, 0);
        if(recieveAmount > 0) {
            buffer[recieveAmount] = '\0';
            printf("%s", buffer);
        }else{
            printf("Disconnect to server!");
            break;
        };
    };
    close(*sockFD);
    return NULL;
}
