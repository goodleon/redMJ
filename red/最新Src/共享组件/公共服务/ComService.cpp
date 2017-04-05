#include "Stdafx.h"
#include <afxdllx.h>

#include <AtlBase.h>

//组件头文件
#include "ComService.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

static AFX_EXTENSION_MODULE ComServiceDLL={NULL,NULL};

//DLL 导出主函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(ComServiceDLL,hInstance)) return 0;
		new CDynLinkLibrary(ComServiceDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(ComServiceDLL);
	}
	return 1;
}


DWORD COM_SERVICE_CLASS GetPlazaRoomVersion()
{
	//打开注册表
	CRegKey RegParamter;
	TCHAR szRegName[MAX_PATH];
	_snprintf(szRegName,sizeof(szRegName),TEXT("Software\\%s\\LogonServer"),szSystemRegKey);
	if (RegParamter.Open(HKEY_CURRENT_USER,szRegName)!=ERROR_SUCCESS) return 0;

	//变量定义
	DWORD dwValue=0;


	if (RegParamter.QueryDWORDValue(TEXT("PlazaVersion"),dwValue)!=ERROR_SUCCESS)
	{
		RegParamter.SetDWORDValue(TEXT("PlazaVersion"),0);
		return 0;
	}

	return dwValue;
}

bool COM_SERVICE_CLASS GetCommandParam(LPCSTR szParamName,CString &pOut)
{
	pOut = "";
	TCHAR *p = _tcsstr(GetCommandLine(),szParamName);
	if(!p)return false;
	p += lstrlen(szParamName);
	char cSign = ' ';
	if(*p=='\"')
	{
		cSign='\"';
		p++;
	}
	TCHAR *p1 = _tcschr(p,cSign);
	if(p1)
	{
		TCHAR c= *p1;
		*p1 = 0;
		pOut = p;
		*p1 = c;
	}
	else
        pOut = p;
	return true;
}

void COM_SERVICE_CLASS MsgBox(TCHAR *format,...)
{
	va_list args;
	CString s;
	va_start (args,format);
	s.FormatV(format,args);
	va_end(args);
	::MessageBox(0,s,0,0);
}

//////////////////////////////////////////////////////////////////////////
