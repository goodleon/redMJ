#include "Stdafx.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include ".\gameframe.h"

//////////////////////////////////////////////////////////////////////////


//CTextRenderWnd GameFrame_TR;											//文本链接对象

//宏定义
#define TITLE_HEIGHT						48							//标题高度
#define MIN_CLIENT_WIDTH					800							//最小宽度
#define MIN_CLIENT_HEIGHT					500							//最小高度



//按钮标示
#define IDC_BT_MIN							1000						//最小按钮
#define IDC_BT_CLOSE						1001						//关闭按钮
#define IDC_BT_MAX							1002						//最大化按钮
#define IDC_BT_BUTTON_1						1003						//功能按钮
#define IDC_BT_BUTTON_2						1004						//功能按钮
#define IDC_BT_BUTTON_3						1005						//功能按钮
#define IDC_BT_BUTTON_4						1006						//功能按钮
#define IDC_BT_BUTTON_5						1007						//功能按钮
//#define IDC_BT_BUTTON_6						1008						//功能按钮
//#define IDC_BT_BUTTON_7						1009						//功能按钮

//消息定义
#define WM_SETUP_FINISH						WM_USER+100					//安装完成

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrame, CFrameWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SETTINGCHANGE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BT_MIN, OnBnClickedMin)
	ON_BN_CLICKED(IDC_BT_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDC_BT_MAX, OnBnClickedMax)
	ON_MESSAGE(WM_SETUP_FINISH,OnMessageSetupFinish)
	ON_BN_CLICKED(IDC_BT_BUTTON_1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BT_BUTTON_2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BT_BUTTON_3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BT_BUTTON_4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BT_BUTTON_5, OnBnClickedButton5)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	ON_MESSAGE(WM_GAMECLIENT, OnGameClient)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrame::CGameFrame()
{
	//设置变量
	m_bMaxed =  false;
	m_bMaxedC = false;

	m_pHtmlBrower1	= NULL;;

	//加载资源
	HINSTANCE hInstance=AfxGetInstanceHandle();

	m_ImageTL.SetLoadInfo(IDB_FRAME_TL,hInstance);
	m_ImageTM.SetLoadInfo(IDB_FRAME_TM,hInstance);
	m_ImageTR.SetLoadInfo(IDB_FRAME_TR,hInstance);
	//m_StatusL.SetLoadInfo(IDB_STATUS_L,hInstance);
	//m_StatusM.SetLoadInfo(IDB_STATUS_M,hInstance);

	return;
}

//析构函数
CGameFrame::~CGameFrame()
{
}

//消息解释
BOOL CGameFrame::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//建立消息
int CGameFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	//设置属性
	//ModifyStyle(WS_CAPTION|WS_BORDER,0,0);

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//设置标题
	CString strTitle;
	strTitle.LoadString(AfxGetInstanceHandle(),IDS_MAIN_TITLE);
	SetWindowText(strTitle);

	//创建按钮
	m_btMin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(-999,0,0,0),this,IDC_BT_MIN);
	m_btMax.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(-999,0,0,0),this,IDC_BT_MAX);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(-999,0,0,0),this,IDC_BT_CLOSE);

	m_btButton1.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(-999,0,0,0),this,IDC_BT_BUTTON_1);
	m_btButton2.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(-999,0,0,0),this,IDC_BT_BUTTON_2);
	m_btButton3.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(-999,0,0,0),this,IDC_BT_BUTTON_3);
	m_btButton4.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(-999,0,0,0),this,IDC_BT_BUTTON_4);
	m_btButton5.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(-999,0,0,0),this,IDC_BT_BUTTON_5);

	//设置图片
	m_btMin.SetButtonImage(IDB_FRAME_MIN,AfxGetInstanceHandle(),false);
	m_btMax.SetButtonImage(IDB_FRAME_MAX,AfxGetInstanceHandle(),false);
	m_btClose.SetButtonImage(IDB_FRAME_CLOSE,AfxGetInstanceHandle(),false);

	m_btButton1.SetButtonImage(IDB_FRAME_BT_BUTTON_1,AfxGetInstanceHandle(),false);
	m_btButton2.SetButtonImage(IDB_FRAME_BT_BUTTON_2,AfxGetInstanceHandle(),false);
	m_btButton3.SetButtonImage(IDB_FRAME_BT_BUTTON_3,AfxGetInstanceHandle(),false);
	m_btButton4.SetButtonImage(IDB_FRAME_BT_BUTTON_4,AfxGetInstanceHandle(),false);
	m_btButton5.SetButtonImage(IDB_FRAME_BT_BUTTON_5,AfxGetInstanceHandle(),false);

	//建立提示控件
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BT_MIN),TEXT("最小化游戏广场"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BT_CLOSE),TEXT("退出游戏广场"));

	//创建控件
	m_DlgControlBar.Create(IDD_CONTROL_BAR,this);
	m_DlgGamePlaza.Create(IDD_GAME_PLAZA,this);
	m_DlgControlBar.ActivePlazaViewItem();
	m_DlgControlBar.ShowWindow(SW_HIDE);

	//显示窗口
	CRect rcRect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcRect,SPIF_SENDCHANGE);
	
	//状态栏滚动
	//m_pHtmlStatus = new CHtmlBrower();
	//m_pHtmlStatus->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,20,NULL);
	//m_pHtmlStatus->EnableBrowser(true);
	//m_pHtmlStatus->Navigate("http://"ServerDomain"/StatusAd.asp");

	//创建控件
	//m_pHtmlBrower1->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(320,521,751,580),this,20,NULL);
	m_pHtmlBrower1 = new CHtmlBrower;
	m_pHtmlBrower1->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(109,7,300,45),this,20,NULL);
	m_pHtmlBrower1->EnableBrowser(true);
	m_pHtmlBrower1->Navigate("http://"ServerDomain"/Client/PlazaAD.asp");

	//bluker: SWP_HIDEWINDOW,SWP_SHOWWINDOW
	SetWindowPos(&wndTop,rcRect.Width()/2 - 400,rcRect.Height()/2 - 300,800,600,SWP_DRAWFRAME|SWP_SHOWWINDOW);
	//SetWindowPos(&wndTop,rcRect.Width()/2 - 512,rcRect.Height()/2 - 374,1024,748,SWP_DRAWFRAME|SWP_SHOWWINDOW);
	//del:SetWindowPos(&wndTop,rcRect.left,rcRect.top,rcRect.Width(),rcRect.Height(),SWP_DRAWFRAME|SWP_SHOWWINDOW);

	RegisterHotKey(m_hWnd, (int)m_hWnd, HOTKEY_MOD, HOTKEY_VK);

	return 0;
}


//绘画背景
BOOL CGameFrame::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//设置改变
void CGameFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags, lpszSection);

	//调整框架大小
	//CRect rcClient;
	//SystemParametersInfo(SPI_GETWORKAREA,0,&rcClient,SPIF_SENDCHANGE);
	//MoveWindow(&rcClient,TRUE);

	return;
}

//关闭消息
void CGameFrame::OnClose()
{
	//关闭提示
	if (g_GlobalUnits.GetGolbalUserData().dwUserID!=0L)
	{
		int nCode=AfxMessageBox(TEXT("您确实要关闭游戏广场吗？"),MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION);
		if (nCode!=IDYES) return;
	}

	//隐藏界面
	ShowWindow(SW_HIDE);

	//关闭房间
	g_pControlBar->CloseAllRoomViewItem();

	//保存配置
	g_GlobalOption.SaveOptionParameter();
	g_GlobalUnits.m_CompanionManager->SaveCompanion();

	UnregisterHotKey(m_hWnd,(int)m_hWnd);

	__super::OnClose();
}

//功能按钮
void CGameFrame::OnBnClickedButton1()
{//官方网站
	//g_GlobalUnits.GoWeb("/",true);
	return;
}


//功能按钮
void CGameFrame::OnBnClickedButton2()
{//金币充值
	//g_GlobalUnits.GoWeb("/Client/Pay",true);
	return;
}

//功能按钮
void CGameFrame::OnBnClickedButton3()
{//个人银行
	//g_GlobalUnits.GoWeb("/Client/Bank",true);
	return;
}

//功能按钮
void CGameFrame::OnBnClickedButton4()
{//金币兑换
	//g_GlobalUnits.GoWeb("/",true);
	return;
}

//功能按钮
void CGameFrame::OnBnClickedButton5()
{//金币兑换
	//g_GlobalUnits.GoWeb("/",true);
	return;
}


//获取最大位置
void CGameFrame::OnGetMinMaxInfo(MINMAXINFO FAR * lpMMI) 
{
	lpMMI->ptMinTrackSize.x=MIN_CLIENT_WIDTH;
	lpMMI->ptMinTrackSize.y=MIN_CLIENT_HEIGHT;
	__super::OnGetMinMaxInfo(lpMMI);
}

//绘画消息
void CGameFrame::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//加载资源
	int nXPos=0,nYPos=0;

	CImageHandle ImageTLHandle(&m_ImageTL);
	CImageHandle ImageTMHandle(&m_ImageTM);
	CImageHandle ImageTRHandle(&m_ImageTR);

	//CImageHandle StatusLHandle(&m_StatusL);
	//CImageHandle StatusMHandle(&m_StatusM);
	

	//绘画左上角
	m_ImageTL.BitBlt(dc,0,0);
	//绘画中上
	m_ImageTM.StretchBlt(dc,
		CRect(m_ImageTL.GetWidth(),0,rcClient.right-m_ImageTR.GetWidth(),m_ImageTM.GetHeight()),
			CRect(0,0,m_ImageTM.GetWidth(),m_ImageTM.GetHeight())
			);
	//绘画右上角
	m_ImageTR.BitBlt(dc,rcClient.right-m_ImageTR.GetWidth(),0);

	//m_StatusL.BitBlt(dc,275,rcClient.bottom-83);
	//m_StatusM.StretchBlt(dc,
	//		CRect(573,rcClient.bottom-83,rcClient.right,rcClient.bottom),
	//		CRect(0,0,m_StatusM.GetWidth(),m_StatusM.GetHeight())
	//		);
/*
	char text[] =	"\\b0怎样获得游戏币？您可以通过在网上商店购买到虚拟卡进行充值。[\\a1立即购买\\a0](9.5折)"
		"　3.固定电话：拨找1601234567进行充值。\\par";
	DWORD textAlignment = THA_LEFT | TVA_TOP;
	RECT margins = {2,2,2,2};
	dc.SetBkMode(TRANSPARENT);
	GameFrame_TR.m_color=0x942525;//0x111197
	GameFrame_TR.SetFontFace("宋体",12);
	GameFrame_TR.EnhDrawText( dc, text, strlen(text), &CRect(100,rcClient.bottom-24,rcClient.right-5,rcClient.bottom-5), &margins, textAlignment );
*/

	return;
}

//位置消息
void CGameFrame::OnSize(UINT nType, int cx, int cy) 
{
	__super::OnSize(nType, cx, cy);
	
	switch(nType)   
	{
	case   SIZE_RESTORED:         //还原   
		if(m_bMaxedC)
		{
			if(g_GlobalOption.SetDisplay(800,600,32,g_GlobalOption.m_ScreenFrequency) || g_GlobalOption.SetDisplay(800,600,32,60))
			{
				SetWindowPos(&wndTop,0,0,800,600,SWP_DRAWFRAME|SWP_SHOWWINDOW);
			}
	}
	break;
	}
	//设置控件
	//48为标题栏高度,83为status高度
	m_DlgControlBar.SetViewItemRect(0,48,cx,cy-48-0);

	//锁定屏幕
	LockWindowUpdate();

	//移动控件
	HDWP hDwp=BeginDeferWindowPos(20);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//其他控件
	DeferWindowPos(hDwp,m_btMin,NULL,cx-123,10,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btMax,NULL,cx-96,6,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btClose,NULL,cx-65,10,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_DlgControlBar,NULL,0,/*m_ImageTL.GetHeight()*/44,cx,CONTROL_BAR_HEIGHT,uFlags);

	////状态栏滚动
	//DeferWindowPos(hDwp,m_pHtmlStatus->GetSafeHwnd(),NULL,88,cy-27,cx-93,22,uFlags);
	

	//导航按钮
	CRect rcButton;
	m_btButton1.GetWindowRect(&rcButton);
	int nSpace=rcButton.right-rcButton.left+2;
	DeferWindowPos(hDwp,m_btButton1,NULL,311,6,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton2,NULL,311+nSpace,6,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton3,NULL,311+nSpace*2,6,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton4,NULL,311+nSpace*3,6,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton5,NULL,311+nSpace*4,6,0,0,uFlags|SWP_NOSIZE);

	//移动控件
	EndDeferWindowPos(hDwp);

	//解除锁定
	UnlockWindowUpdate();

	return;
}

//最小按钮
void CGameFrame::OnBnClickedMin()
{
	ShowWindow(SW_MINIMIZE);	
	g_GlobalOption.SetDisplayDefault();
	return;
}

//最大化按钮
void CGameFrame::OnBnClickedMax()
{
	if(m_bMaxed)
	{
		g_GlobalOption.SetDisplayDefault();
		SetWindowPos(&wndTop,g_GlobalOption.m_ScreenW/2 - 400,g_GlobalOption.m_ScreenH/2 - 300,800,600,SWP_DRAWFRAME|SWP_SHOWWINDOW);
		m_bMaxed=false;
		m_bMaxedC = false;
	}
	else
	{
		if(g_GlobalOption.SetDisplay(800,600,32,g_GlobalOption.m_ScreenFrequency) || g_GlobalOption.SetDisplay(800,600,32,60))
		{
			SetWindowPos(&wndTop,0,0,800,600,SWP_DRAWFRAME|SWP_SHOWWINDOW);
		}
		m_bMaxed=true;
		m_bMaxedC = true;
	}
}

//关闭按钮
void CGameFrame::OnBnClickedClose()
{
	g_pControlBar->CloseCurrentViewItem();
	return;
}

//安装完成
LRESULT CGameFrame::OnMessageSetupFinish(WPARAM wParam, LPARAM lParam)
{
	g_GlobalUnits.m_ServerListManager.UpdateGameKind((WORD)wParam);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
void CGameFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(point.y<TITLE_HEIGHT && !m_bMaxed)
	{
		SendMessage(WM_NCLBUTTONDOWN, HTCAPTION,0);
	}
	//GameFrame_TR.OnLButtonDown(nFlags,point);
	CFrameWnd::OnLButtonDown(nFlags, point);
}


BOOL CGameFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if(GameFrame_TR.OnSetCursor(pWnd))return true;
	return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CGameFrame::OnHotKey(WPARAM wParam, LPARAM lParam)
{
CWnd *pGameWnd = AfxGetMainWnd();
	
	if ((g_GameProcessInfo.hProcess!=NULL)&&(WaitForSingleObject(g_GameProcessInfo.hProcess,0)==WAIT_TIMEOUT))
	{
		ShowWindow(SW_HIDE);
		if(!IsWindow(g_GameProcessWnd))return 0;
		pGameWnd = CWnd::FromHandle(g_GameProcessWnd);
	}
	if (wParam==(WPARAM)m_hWnd)
	{
		if(pGameWnd->IsWindowVisible())
		{
			pGameWnd->ShowWindow(SW_MINIMIZE);
			pGameWnd->ShowWindow(SW_HIDE);
		}
		else
		{
			pGameWnd->ShowWindow(SW_NORMAL);
			pGameWnd->SetActiveWindow();
			pGameWnd->BringWindowToTop();
			pGameWnd->SetForegroundWindow();
		}
	}return 0;
}

//游戏进程发窗口句柄给大厅
LRESULT CGameFrame::OnGameClient(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case MSG_SENDWND:
		g_GameProcessWnd = (HWND)lParam;
		break;
	}
	return 0;
}
