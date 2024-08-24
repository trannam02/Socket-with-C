#include "./../include/socketutil.h"
#include "./headers/connection.h"

struct AcceptedSocket * acceptedSockets[100];
int acceptedSocketsCount = 0;

int main() {
    int serverSocketFD = createTCPIPv4Socket();
    struct sockaddr_in * serverAddress = createIPv4Address(IP, PORT);

    int result = bind(serverSocketFD, (struct sockaddr *)(serverAddress), sizeof(*serverAddress));
    if(result == 0) printf("Socket was bound successfully at port %d\n", PORT);
    else {
        printf("can't bind at port %d \n", PORT);
        return 0;
    };

    int listenResult = listen(serverSocketFD, 10);

    listenConnectionAndCreateThread(serverSocketFD, acceptedSockets, &acceptedSocketsCount);

    shutdown(serverSocketFD, SHUT_RDWR);
    return 0;
};
