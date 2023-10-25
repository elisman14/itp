#include <stdio.h>
#include <windows.h>

typedef int(__cdecl *MYPROC)(LPCWSTR);
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

int main(void) {
  HANDLE hFile;
  HINSTANCE DiskSpaceLib, MailslotLib;
  MYPROC DiskSpaceProc, MailWriteProc, MailReadProc;
  BOOL fFreeResult1, fFreeResult2, fFreeResult3, fRunTimeLinkSuccess = FALSE;
  char *mail = (char *)malloc(1024 * sizeof(char));

  MakeSlot(SlotName);

  DiskSpaceLib = LoadLibrary(TEXT("DiskSpace.dll"));
  MailslotLib = LoadLibrary(TEXT("Mailslot.dll"));

  if (DiskSpaceLib != NULL && MailslotLib != NULL) {
    DiskSpaceProc = (MYPROC)GetProcAddress(DiskSpaceLib, "GetDiskSpace");
    MailWriteProc = (MYPROC)GetProcAddress(MailslotLib, "runner");
    MailReadProc = (MYPROC)GetProcAddress(MailslotLib, "printer");

    if (NULL == DiskSpaceProc || NULL == MailWriteProc ||
        NULL == MailReadProc) {
      return 1;
    }

    for (size_t i = 0; i < 2; i++) {
      (DiskSpaceProc)((LPCWSTR)mail);
      (MailWriteProc)((LPCWSTR)mail);
    }

    (MailReadProc)((LPCWSTR)hSlot);
    fFreeResult1 = FreeLibrary(DiskSpaceLib);
    fFreeResult2 = FreeLibrary(MailslotLib);
  }

  printf("Message printed from executable\n");

  return 0;
}