#include "Stdafx.h"
#include <AfxDLLx.h>
#include "GameFrame.h"

static AFX_EXTENSION_MODULE GameFrameDLL={NULL,NULL};

//////////////////////////////////////////////////////////////////////////

//DLL 主函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		//初始化 SOCKET
		WSADATA WSAData;
		WORD wVersionRequested=MAKEWORD(2,2);
		int iErrorCode=WSAStartup(wVersionRequested,&WSAData);

		if (!AfxInitExtensionModule(GameFrameDLL, hInstance)) return 0;
		new CDynLinkLibrary(GameFrameDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(GameFrameDLL);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
