#include "./../include/socketutil.h"
#include "./headers/connection.h"

int main() {
    int sockFD = createTCPIPv4Socket();
    struct sockaddr_in * address = createIPv4Address("192.168.1.109", 3000);
    int result = connect(sockFD, (struct sockaddr*)address, sizeof *address);

    if(result == 0) printf("Connect successful!\n");
    else{
        printf("Can't connect to server...");
        close(sockFD);
        return 0;
    };

    setName(sockFD);
    listenResponseAtOtherThread(&sockFD);
    sendReqest(sockFD);

    close(sockFD);
    return 0;
};
