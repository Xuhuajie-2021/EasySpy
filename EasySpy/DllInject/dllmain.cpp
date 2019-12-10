// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"

HWND g_MainWnd = NULL;
HHOOK g_hook = NULL;

#ifdef __cplusplus
extern "C" { //only need to export C interface if
	// used by C++ source code
#endif
	__declspec(dllexport) void SetHHook(HHOOK hook)  //�˺�������
	{
		g_hook = hook;
	}

	__declspec(dllexport) LRESULT GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == HC_ACTION && wParam == PM_REMOVE)
		{
			//must process and lParam is MSG struct
			MSG* msg = (MSG*)lParam;
			if (g_MainWnd)
			{
				PostMessage(g_MainWnd, WM_USER + 100 + msg->message, msg->wParam, msg->lParam);
			}
		}

		//�Σ���һ�����������Եģ�����ֱ�Ӵ�NULL
		return CallNextHookEx(g_hook, nCode, wParam, lParam);
	}

#ifdef __cplusplus
}
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//try to get the windows
		g_MainWnd = FindWindow(NULL, L"EasySpy");
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

