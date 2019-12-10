// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
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
		/* ֻ�е�һ�λ�������֮���ٴ�Attach�������DLL_PROCESS_ATTACH��ֻ������DLL�ĵ��ô��� */
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

