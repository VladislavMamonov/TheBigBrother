#include "BigBrother.hpp"


void Server::StartServer()
{
    int iResult = WSAStartup(MAKEWORD(2,2), &wData);
    if (iResult != NO_ERROR) {
        cout << "WSA startup failed" << endl;
        return;
    }

    SOCKADDR_IN addr;
    int addrl = sizeof(addr);

    addr.sin_addr.S_un.S_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    sock = socket(AF_INET, SOCK_STREAM, NULL);

    if (sock == SOCKET_ERROR)
        cout << "socket not created" << endl;

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR)
        cout << "socket bind successfully" << endl;

    if (listen(sock, SOMAXCONN) != SOCKET_ERROR)
		cout << "start listening at port: "  << ntohs(addr.sin_port) << endl;

    handle();
}


void Server::handle()
{
    while (true)
    {
        char buf[100] = "";
        SOCKET clientS;
        SOCKADDR_IN addr_c;
        int addrlen = sizeof(addr_c);

        if ((clientS = accept(sock, (struct sockaddr*)&addr_c, &addrlen)) != 0) {
            printf("client connected: %u.%u.%u.%u:%u\n",
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b1,
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b2,
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b3,
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b4,
				ntohs(addr_c.sin_port));
            recv(clientS, buf, sizeof(buf), NULL);
            cout << buf << endl;
        }
        closesocket(clientS);
    }
}


void Server::StopServer()
{
    closesocket(sock);
    WSACleanup();
    cout << "server was stopped" << endl;
}



int main()
{
    Server server;
    server.port = 2020;
    server.StartServer();

    return 0;
}
