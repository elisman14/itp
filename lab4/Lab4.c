#include <stdio.h>
#include <tchar.h>
#include <windows.h>

HANDLE hSlot;
LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");

BOOL WINAPI MakeSlot(LPCTSTR lpszSlotName) {
  hSlot = CreateMailslot(lpszSlotName, 0, MAILSLOT_WAIT_FOREVER,
                         (LPSECURITY_ATTRIBUTES)NULL);

  if (hSlot == INVALID_HANDLE_VALUE) {
    printf("CreateMailslot failed with %d\n", GetLastError());
    return FALSE;
  } else
    printf("Mailslot created successfully.\n");
  return TRUE;
}

BOOL WriteSlot(HANDLE hSlot, LPCTSTR lpszMessage) {
  BOOL fResult;
  DWORD cbWritten;

  fResult = WriteFile(hSlot, lpszMessage,
                      (DWORD)(lstrlen(lpszMessage) + 1) * sizeof(TCHAR),
                      &cbWritten, (LPOVERLAPPED)NULL);

  if (!fResult) {
    printf("WriteFile failed with %d.\n", GetLastError());
    return FALSE;
  }

  printf("Slot written to successfully.\n");

  return TRUE;
}

BOOL ReadSlot() {
  DWORD cbMessage, cMessage, cbRead;
  BOOL fResult;
  LPTSTR lpszBuffer;
  TCHAR achID[200];
  DWORD cAllMessages;
  HANDLE hEvent;
  OVERLAPPED ov;

  cbMessage = cMessage = cbRead = 0;

  hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ExampleSlot"));
  if (NULL == hEvent)
    return FALSE;
  ov.Offset = 0;
  ov.OffsetHigh = 0;
  ov.hEvent = hEvent;

  fResult = GetMailslotInfo(hSlot, (LPDWORD)NULL, &cbMessage, &cMessage,
                            (LPDWORD)NULL);

  if (!fResult) {
    printf("GetMailslotInfo failed with %d.\n", GetLastError());
    return FALSE;
  }

  if (cbMessage == MAILSLOT_NO_MESSAGE) {
    printf("Waiting for a message...\n");
    return TRUE;
  }

  cAllMessages = cMessage;

  while (cMessage != 0) {
    sprintf((LPTSTR)achID, TEXT("\nMessage #%d of %d\n"),
            cAllMessages - cMessage + 1, cAllMessages);

    lpszBuffer = (LPTSTR)GlobalAlloc(
        GPTR, lstrlen((LPTSTR)achID) * sizeof(TCHAR) + cbMessage);
    if (NULL == lpszBuffer)
      return FALSE;
    lpszBuffer[0] = '\0';

    fResult = ReadFile(hSlot, lpszBuffer, cbMessage, &cbRead, &ov);

    if (!fResult) {
      printf("ReadFile failed with %d.\n", GetLastError());
      GlobalFree((HGLOBAL)lpszBuffer);
      return FALSE;
    }

    printf(TEXT("Contents of the mailslot: %s\n"), lpszBuffer);

    GlobalFree((HGLOBAL)lpszBuffer);

    fResult = GetMailslotInfo(hSlot, (LPDWORD)NULL, &cbMessage, &cMessage,
                              (LPDWORD)NULL);

    if (!fResult) {
      printf("GetMailslotInfo failed (%d)\n", GetLastError());
      return FALSE;
    }
  }
  CloseHandle(hEvent);
  return TRUE;
}

int main(int argc, TCHAR *argv[]) {
  HANDLE hFile;
  char mail[200];
  ULARGE_INTEGER p1, p2, p3;

  MakeSlot(SlotName);

  hFile = CreateFile(SlotName, GENERIC_WRITE, FILE_SHARE_READ,
                     (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

  if (hFile == INVALID_HANDLE_VALUE) {
    printf("CreateFile failed with %d.\n", GetLastError());
    return FALSE;
  }

  for (int i = 0; i < 2; ++i) {
    GetDiskFreeSpaceEx(NULL, &p1, &p2, &p3);

    sprintf(mail,
            "free for user: GB: %lld; Total for user: GB: %lld, Free total "
            "GB:%lld \n",
            p1.QuadPart / 1024 / 1024 / 1024, p2.QuadPart / 1024 / 1024 / 1024,
            p3.QuadPart / 1024 / 1024 / 1024);

    WriteSlot(hFile, mail);
    Sleep(1000);
  }

  ReadSlot();
  Sleep(1000);

  CloseHandle(hSlot);
}