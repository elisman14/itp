#include <stdio.h>
#include <windows.h>

int main(int argc, TCHAR *argv[]) {
  if (RegisterHotKey(NULL, 1, MOD_ALT, 0x53)) // 0x53 is 's'
  {
    printf("Hotkey 'ALT+s' registered\n");
  }

  MSG msg = {0};
  while (GetMessage(&msg, NULL, 0, 0) != 0) {
    if (msg.message == WM_HOTKEY) {
      printf("WM_HOTKEY received\n");
      int msgboxID = MessageBox(NULL, (LPCWSTR) "Ti hui",
                                (LPCWSTR) "Voobshe poebat", NULL);
    } else {
      return 0;
    }
  }

  return 0;
}
/*
utf8 -> any char need 4b
10gb -> 10gb * 2**30 / 4 / 1000
with open(file, "w", encoding="utf-8") as f:
    for i in range(upper_value):
        f.write("1" * 1000 + "\n")
*/