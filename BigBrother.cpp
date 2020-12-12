#include "BigBrother.hpp"


char* getMAC()
{
  PIP_ADAPTER_INFO AdapterInfo;
  DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
  char *mac_addr = (char*)malloc(18);

  AdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
  if (AdapterInfo == NULL) {
    printf("Error allocating memory needed to call GetAdaptersinfo\n");
    free(mac_addr);
    return NULL; // it is safe to call free(NULL)
  }

  // Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
  if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
    free(AdapterInfo);
    AdapterInfo = (IP_ADAPTER_INFO *) malloc(dwBufLen);
    if (AdapterInfo == NULL) {
      printf("Error allocating memory needed to call GetAdaptersinfo\n");
      free(mac_addr);
      return NULL;
    }
  }

  if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
    // Contains pointer to current adapter info
    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    for (int i = 0; i < 2; i++) pAdapterInfo = pAdapterInfo->Next;
      sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
        pAdapterInfo->Address[0], pAdapterInfo->Address[1],
        pAdapterInfo->Address[2], pAdapterInfo->Address[3],
        pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
  }
  free(AdapterInfo);
  return mac_addr; // caller must free.
}


BOOL WINAPI SaveBitmap(WCHAR *wPath)
{
  BITMAPFILEHEADER    bfHeader;
  BITMAPINFOHEADER    biHeader;
  BITMAPINFO          bInfo;
  HGDIOBJ             hTempBitmap;
  HBITMAP             hBitmap;
  BITMAP              bAllDesktops;
  HDC hDC, hMemDC;
  LONG lWidth, lHeight;
  BYTE *bBits = NULL;
  HANDLE hHeap = GetProcessHeap();
  DWORD cbBits, dwWritten = 0;
  HANDLE hFile;
  INT x = GetSystemMetrics(SM_XVIRTUALSCREEN);
  INT y = GetSystemMetrics(SM_YVIRTUALSCREEN);

  ZeroMemory(&bfHeader, sizeof(BITMAPFILEHEADER));
  ZeroMemory(&biHeader, sizeof(BITMAPINFOHEADER));
  ZeroMemory(&bInfo, sizeof(BITMAPINFO));
  ZeroMemory(&bAllDesktops, sizeof(BITMAP));

  hDC = GetDC(NULL);
  hTempBitmap = GetCurrentObject(hDC, OBJ_BITMAP);
  GetObjectW(hTempBitmap, sizeof(BITMAP), &bAllDesktops);

  lWidth = bAllDesktops.bmWidth;
  lHeight = bAllDesktops.bmHeight;

  DeleteObject(hTempBitmap);


  bfHeader.bfType = (WORD)('B' | ('M' << 8));
  bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
  biHeader.biSize = sizeof(BITMAPINFOHEADER);
  biHeader.biBitCount = 24;
  biHeader.biCompression = BI_RGB;
  biHeader.biPlanes = 1;
  biHeader.biWidth = lWidth;
  biHeader.biHeight = lHeight;

  bInfo.bmiHeader = biHeader;

  cbBits = (((24 * lWidth + 31)&~31) / 8) * lHeight;

  hMemDC = CreateCompatibleDC(hDC);
  hBitmap = CreateDIBSection(hDC, &bInfo, DIB_RGB_COLORS, (VOID **)&bBits, NULL, 0);
  SelectObject(hMemDC, hBitmap);
  BitBlt(hMemDC, 0, 0, lWidth, lHeight, hDC, x, y, SRCCOPY);


  hFile = CreateFileW(wPath, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  WriteFile(hFile, &bfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
  WriteFile(hFile, &biHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
  WriteFile(hFile, bBits, cbBits, &dwWritten, NULL);

  CloseHandle(hFile);

  DeleteDC(hMemDC);
  ReleaseDC(NULL, hDC);
  DeleteObject(hBitmap);

  return TRUE;
}
