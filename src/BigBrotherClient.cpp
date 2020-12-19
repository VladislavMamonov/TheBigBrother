#include "BigBrother.hpp"


SOCKET client()
{
    WSAData wData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wData);
    if (iResult != NO_ERROR) {
        cout << "WSA startup failed" << endl;
        return 1;
    }

    struct sockaddr_in clientService;

    SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(2020);
    iResult = connect(ConnectSocket, (SOCKADDR*) &clientService, sizeof(clientService));
    if (iResult == SOCKET_ERROR) {
        cout << "connect failed" << endl;
        closesocket(ConnectSocket);
        WSACleanup();
    }

    return ConnectSocket;
}
