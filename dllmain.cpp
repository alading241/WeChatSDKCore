// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi")
#include "config.h"
#include "rpcutil_s.h"
#include "windows.h"

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif


DWORD WINAPI dllinject_callback(LPVOID lpparentet)
{
	StartSDKServer();
	return 0;
}

// 在微信进程中执行线程
int Invoke() {
	unsigned long ulThreadId = 0;
	HANDLE hThread = CreateThread(NULL, 0, dllinject_callback, NULL, 0, &ulThreadId);
	return ulThreadId;
}

// dll注入成功后执行指定的回调方法
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		if(Invoke() <= 0){
			MessageBox(NULL, L"RPC线程创建失败。", L"信息", MB_ICONINFORMATION);
		}
		break;
	case DLL_PROCESS_DETACH:
		StopSDKServer();
		break;
	default:
		break;
	}
	return TRUE;
}

