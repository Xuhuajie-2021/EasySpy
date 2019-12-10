// HookTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <windows.h>

int main(int argc, char *argv)
{
	int i;
	HMODULE hdll = LoadLibrary(_T("DllDemo"));
	HOOKPROC hproc = (HOOKPROC)GetProcAddress(hdll, "test");
	HHOOK hook = SetWindowsHookEx(WH_KEYBOARD, hproc, hdll, 0);
	for (i = 0; i < 30; i++){
		Sleep(2000);
	}
	UnhookWindowsHookEx(hook);
	return 0;
}

