#include <stdio.h>
#include <tchar.h>
#include <windows.h>

void GetDiskSpace(LPCWSTR mail) {

  ULARGE_INTEGER p1, p2, p3;

  GetDiskFreeSpaceEx(NULL, &p1, &p2, &p3);

  sprintf((char *)mail,
          "free for user: GB: %lld; Total for user: GB: %lld, Free total "
          "GB:%lld \n",
          p1.QuadPart / 1024 / 1024 / 1024, p2.QuadPart / 1024 / 1024 / 1024,
          p3.QuadPart / 1024 / 1024 / 1024);
}
