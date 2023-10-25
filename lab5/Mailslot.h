#ifndef MAILSLOT_H
#define MAILSLOT_H
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

BOOL WriteSlot(HANDLE, LPCTSTR);
BOOL ReadSlot();
BOOL runner(char *);
void printer(HANDLE);

#endif