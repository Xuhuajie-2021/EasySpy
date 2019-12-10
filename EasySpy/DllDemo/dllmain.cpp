// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

#include <windows.h>
#include <tchar.h>

extern "C"
{
	__declspec(dllexport) BOOL test()
	{
		MessageBox(NULL, TEXT("SetWindowsHookEx Success!"), TEXT("Tips"), MB_OK);
		return 0;
	}
}


BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpvRevered)
{
	// MessageBox(NULL, TEXT("This is DllMain!"), TEXT("Tips"), MB_OK);
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		/* 只有第一次会进到这里，之后再次Attach不会产生DLL_PROCESS_ATTACH，只会增加DLL的调用次数 */
		MessageBox(NULL, TEXT("Process Load Dll Success!"), TEXT("Tips"), MB_OK);
		break;
	case DLL_PROCESS_DETACH:
		// MessageBox(NULL, TEXT("Process Unload Dll Success!"), TEXT("Tips"), MB_OK);
		break;
	case DLL_THREAD_ATTACH:
		// MessageBox(NULL, TEXT("Thread load Dll Success!"), TEXT("Tips"), MB_OK);
		break;
	case DLL_THREAD_DETACH:
		// MessageBox(NULL, TEXT("Thread Unload Dll Success!"), TEXT("Tips"), MB_OK);
		break;
	}
	return TRUE;
}

