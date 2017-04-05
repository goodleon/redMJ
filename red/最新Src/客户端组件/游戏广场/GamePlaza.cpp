#include "Stdafx.h"
#include "GamePlaza.h"
#include "GameFrame.h"
#include "GlobalUnits.h"




//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGamePlazaApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//应用程序对象说明
CGamePlazaApp theApp;

//构造函数
CGamePlazaApp::CGamePlazaApp()
{
	m_intTheme = 1;
}

//初始化函数
BOOL CGamePlazaApp::InitInstance()
{
	__super::InitInstance();

	//判断是否重入
#ifndef ADMIN_PLAZA
#ifndef _DEBUG
	bool bExistIGame=false;
	CMutex Mutex(FALSE,MAIN_DLG_CLASSNAME,NULL);
	if (Mutex.Lock(0)==FALSE) bExistIGame=true;
	CWnd * pIGameWnd=CWnd::FindWindow(MAIN_DLG_CLASSNAME,szProductName);
	if (pIGameWnd!=NULL) 
	{
		bExistIGame=true;
		//pIGameWnd->ShowWindow(SW_RESTORE);
		//pIGameWnd->SetActiveWindow();
		//pIGameWnd->BringWindowToTop();
		//pIGameWnd->SetForegroundWindow();
	}
	if (bExistIGame==true) return FALSE;
#endif
#endif


	//程序初始化
	AfxOleInit();
	AfxInitRichEdit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//设置注册表
	SetRegistryKey(szSystemRegKey);

	//加载界面库
	CSkinAttribute::LoadSkinAttribute(GET_OBJECT_INTERFACE(m_SkinRecordFile,IUnknownEx));

	//加载配置
	g_GlobalOption.LoadOptionParameter();
	g_GlobalOption.m_bDisableBgSound=AfxGetApp()->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("DisableBgSound"),g_GlobalOption.m_bDisableBgSound)?true:false;


	//全局信息
	if (g_GlobalUnits.InitGlobalUnits()==false)
	{
		AfxMessageBox(TEXT("游戏广场启动失败，即将退出！"),MB_ICONSTOP);
		return FALSE;
	}

	//注册窗口类
	WNDCLASS WndClasss;
	memset(&WndClasss,0,sizeof(WndClasss));
	WndClasss.lpfnWndProc=::DefWindowProc;
	WndClasss.hInstance=AfxGetInstanceHandle();
	WndClasss.lpszClassName=MAIN_DLG_CLASSNAME;
	if (!AfxRegisterClass(&WndClasss)) AfxThrowResourceException();

	//建立窗口
	//WS_SYSMENU|WS_MINIMIZEBOX|WS_OVERLAPPED|WS_CLIPCHILDREN
	//DS_SETFONT | DS_CENTER | WS_POPUP | WS_CLIPCHILDREN | WS_SYSMENU
	const DWORD dwStyte=DS_SETFONT | DS_CENTER | WS_POPUP | WS_CLIPCHILDREN | WS_SYSMENU;
	CGameFrame * pMainFrame=new CGameFrame();
	pMainFrame->Create(MAIN_DLG_CLASSNAME,TEXT(""),dwStyte,CRect(0,0,0,0));
	m_pMainWnd=pMainFrame;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

