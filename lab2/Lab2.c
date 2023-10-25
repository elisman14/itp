#include <stdio.h>
#include <windows.h>

#define THREADS_COUNT 9

typedef struct {
  char *ch;
  CRITICAL_SECTION *cs;
} IncCharRequest, *PIncCharRequest;

DWORD WINAPI inc_char(LPVOID lpParam) {
  PIncCharRequest req = (PIncCharRequest)lpParam;
  EnterCriticalSection(req->cs);
  printf("%c\n", *req->ch);
  ++(*req->ch);
  LeaveCriticalSection(req->cs);

  return 0;
}

void main(int argc, char *argv[]) {
  DWORD threads[THREADS_COUNT];
  HANDLE threads_handlers[THREADS_COUNT];
  char ch = 'a';
  CRITICAL_SECTION cs;
  InitializeCriticalSection(&cs);

  for (size_t i = 0; i < THREADS_COUNT; i++) {
    IncCharRequest req;
    req.ch = &ch;
    req.cs = &cs;
    threads_handlers[i] = CreateThread(NULL, 0, inc_char, &req, 0, &threads[i]);
  }
  WaitForMultipleObjects(THREADS_COUNT, threads_handlers, TRUE, INFINITE);
  DeleteCriticalSection(&cs);
}
