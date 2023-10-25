#include <stdio.h>
#include <windows.h>

#define BUF_SIZE 256
TCHAR szName[] = TEXT("MyFileMappingObject");

int main(int argc, TCHAR *argv[]) {
  HANDLE hMapFile;
  HANDLE ghMutex;
  int *pBuf;

  hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szName);

  if (hMapFile == NULL) {
    printf(TEXT("Could not open file mapping object from increment (%d).\n"),
           GetLastError());
    return 1;
  }

  pBuf = (int *)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);

  ghMutex = CreateMutex(NULL, FALSE, NULL);

  printf("[increment]: start value = %d\n", pBuf[0]);
  ++pBuf[0];
  printf("[increment]: finish value = %d\n", pBuf[0]);

  ReleaseMutex(ghMutex);

  UnmapViewOfFile(pBuf);
  CloseHandle(hMapFile);

  return 0;
}