#include <conio.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define BUF_SIZE 256
TCHAR szName[] = TEXT("MyFileMappingObject");

typedef struct {
  int *num;
} hMapStruct, *PhMapStruct;

int main(int argc, TCHAR *argv[]) {
  HANDLE hMapFile;
  int *pBuf;

  hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
                               BUF_SIZE, szName);
  if (hMapFile == NULL) {
    printf(TEXT("Could not create file mapping object (%d).\n"),
           GetLastError());
    return 1;
  }
  pBuf = (int *)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);

  int a[] = {0};
  CopyMemory((PVOID)pBuf, a, sizeof(int));

  HANDLE handlers[3];
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  for (size_t i = 0; i < 2; ++i) {
    if (!CreateProcess("Lab3_increment.exe", NULL, NULL, NULL, FALSE, 0, NULL,
                       NULL, &si, &pi)) {
      printf("Error increment i=%d\n", i);
      return 1;
    }
    handlers[i] = pi.hProcess;
  }
  if (!CreateProcess("Lab3_decrement.exe", NULL, NULL, NULL, FALSE, 0, NULL,
                     NULL, &si, &pi)) {
    printf("Error decrement\n");
    return 1;
  }
  handlers[2] = pi.hProcess;
  Sleep(10000);

  for (size_t i = 0; i < 3; ++i) {
    TerminateProcess(handlers[i], 0);
    CloseHandle(handlers[i]);
  }

  UnmapViewOfFile(pBuf);
  CloseHandle(hMapFile);

  return 0;
}
