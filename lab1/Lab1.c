#include <stdio.h>
#include <tchar.h>
#include <windows.h>

const size_t PROCESSES_COUNT = 10;

void main(int argc, TCHAR *argv[]) {
  printf("%d\n", argc);

  HANDLE *handlers = malloc(sizeof(HANDLE) * PROCESSES_COUNT);
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  for (size_t i = 0; i < PROCESSES_COUNT; ++i) {
    if (!CreateProcess("C:\\Windows\\system32\\notepad.exe", NULL, NULL, NULL,
                       FALSE, 0, NULL, NULL, &si, &pi)) {
      printf("Error!!");
      return;
    }
    handlers[i] = pi.hProcess;
  }

  Sleep(10000);
  // WaitForInputIdle(pi.hProcess, INFINITE);
  for (size_t i = 0; i < PROCESSES_COUNT; ++i) {
    TerminateProcess(handlers[i], 0);
    CloseHandle(handlers[i]);
  }

  free(handlers);
  ExitProcess(0);
  return;
}