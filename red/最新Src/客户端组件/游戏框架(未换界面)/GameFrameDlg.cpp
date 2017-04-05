#include "Stdafx.h"

#include "Resource.h"
#include "GameFrameDlg.h"
#include "GlobalOption.h"
#include "ClientKernelSink.h"

#include <Mmsystem.h>
#include "WaveShow.h"
//#include "Msocket.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	//ON_WM_WINDOWPOSCHANGING()
	ON_BN_CLICKED(IDB_BT_QUIT, OnBnClickedQuit)
	ON_BN_CLICKED(IDB_BT_MAX, OnBnClickedMax)
	ON_BN_CLICKED(IDB_BT_MIN, OnBnClickedMin)
	ON_BN_CLICKED(IDB_BT_OPTION, OnBnClickedOption)
	ON_BN_CLICKED(IDB_BT_GAME_RULE, OnBnClickedHelp)
	ON_MESSAGE(WM_HIT_EXPMESSTION,OnHitExpression)
	ON_WM_MOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

bool GetHttpString(LPCSTR szPage,CString &s,int nPort=80)
{/*
SOCKET sock;
	s = "";
	sock = TcpClient(ServerDomain,nPort);//ServerDomain|"127.0.0.1"
	if(!sock)return false;
	s.Format("GET %s HTTP/1.1\r\nHost: %s\r\nConnection: Close\r\nCache-Control: no-cache\r\n\r\n",
		szPage,ServerDomain);
	int nLen = lstrlen(s);
	if(TcpSend(sock,s,nLen)!=nLen)
	{
		s = "";
		TcpClose(sock);
		return false;
	}
	s = "";
	do 
	{
		char buf[1024];
		memset(buf,0,sizeof(buf));
		if(TcpRecv(sock,buf,1000)<1)break;
		s += buf;
	} while(1);

	TcpClose(sock);

	char *p = strstr(s,"\r\n\r\n");
	if(!p)
	{
		s = "";
		return false;
	}
	s = p+4;
*/
	return true;
}


//构造函数
CGameFrameDlg::CGameFrameDlg(CGameFrameView * pGameFrameView)  : CDialog(IDD_EMPTY_MAINFRAME)
{
	//设置变量
	m_bReset=false;
	m_bRectify=false;
	m_bInitDialog=false;
	m_pKernelSink=NULL;
	m_pGameFrameView=pGameFrameView;

	//配置变量
	m_bAllowSound=true;

	//创建钩子
	m_pKernelSink=new CClientKernelSink(*this);

	//指定声音包文件
	SetWavPackage("Waves.fip");
	
	m_ToolBar = new CDlgToolBar();
	m_bMaxed = false;
	m_DlgUserList = new CDlgUserList();
	m_ChatMsgDlg = new CDlgChatMsg();
	g_GlobalOption.m_crChatTX = 0x505050;
	return;
}

//析构函数
CGameFrameDlg::~CGameFrameDlg()
{
	SafeDelete(m_btQuit);
	SafeDelete(m_btMax);
	SafeDelete(m_btMin);
	SafeDelete(m_btOption);
	SafeDelete(m_btHelp);

	SafeDelete(m_ToolBar);
	SafeDelete(m_DlgUserList);
	SafeDelete(m_ChatMsgDlg);


	SafeDelete(m_pKernelSink);
	return;
}

//指定声音包文件
bool CGameFrameDlg::SetWavPackage(LPCSTR szWavFile)
{
char szPackFile[MAX_PATH];
	GetModuleFileName(NULL,szPackFile,MAX_PATH);
	*(strrchr(szPackFile,'\\')+1)=0;
	strcat(szPackFile,szWavFile);

	//return g_WaveShow.SetWaveFile(szPackFile);
	return true;
}



//指定声音脚本文件
bool CGameFrameDlg::SetWavScript(LPCSTR szWavScriptFile)
{
	CString s;
	CString szUrl = szWavScriptFile;
	szUrl += "&SessionId=";
	GetCommandParam("/sid:",s);
	szUrl += s;


	//::MessageBox(0,szUrl,0,0);
	if(GetHttpString(szUrl,s))
	{
//		return g_WaveShow.SetScript(s);
		return true;
	}
	return false;
}


//调整界面
void CGameFrameDlg::RectifyControl(int nWidth, int nHeight)
{
	//状态判断
	if (m_bInitDialog==false) return;

	CString szTitle;
	if(GetCommandParam("/Title:",szTitle) || 1)
	{
		SetWindowText(szTitle);
	}
	//变量定义
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//锁定屏幕
	LockWindowUpdate();

	CRect rc;
	GetWindowRect(rc);

	m_ToolBar->SetWindowPos(NULL,0,0,397,25,SWP_NOMOVE);
	m_ToolBar->SetWindowPos(NULL,rc.left + 7,rc.bottom - 34,0,0,SWP_NOSIZE);

	m_ChatMsgDlg->SetWindowPos(NULL,0,0,299,159,SWP_NOMOVE);
	m_ChatMsgDlg->SetWindowPos(NULL,rc.left+7,rc.bottom - 195,0,0,SWP_NOSIZE);

	m_DlgUserList->SetWindowPos(NULL,0,0,m_DlgUserList->m_ListUsers.GetWidth(),159,SWP_NOMOVE);
	m_DlgUserList->SetWindowPos(NULL,rc.left+307,rc.bottom - 195,0,0,SWP_NOSIZE);



	//移动控件
	HDWP hDwp=BeginDeferWindowPos(32);
	DeferWindowPos(hDwp,m_pGameFrameView->GetSafeHwnd(),NULL,0,0,nWidth,nHeight,uFlags);
	EndDeferWindowPos(hDwp);




	//重画界面
	//Invalidate(FALSE);
	UpdateWindow();

	//解除锁定
	UnlockWindowUpdate();

	return;
}

//网络消息
bool CGameFrameDlg::OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	return false;
}

//发送函数
bool CGameFrameDlg::SendData(WORD wSubCmdID)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendSocketData(MDM_GF_GAME,wSubCmdID);
}

//发送函数
bool CGameFrameDlg::SendData(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendSocketData(MDM_GF_GAME,wSubCmdID,pData,wDataSize);
}

//发送准备
bool CGameFrameDlg::SendUserReady(void * pBuffer, WORD wDataSize)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendUserReady(pBuffer,wDataSize);
}

//设置定时器
bool CGameFrameDlg::SetGameTimer(WORD wChairID, UINT nTimerID, UINT nElapse)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SetGameTimer(wChairID,nTimerID,nElapse);
}

//删除定时器
bool CGameFrameDlg::KillGameTimer(UINT nTimerID)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->KillGameTimer(nTimerID);
}

//播放声音,//bluker:这个在游戏中实际没有用到，所以没有处理
bool CGameFrameDlg::PlayGameSound(LPCTSTR pszSoundName)
{
	if ((m_bAllowSound==true)&&(IsWindowVisible()))
	{
		::PlaySound(pszSoundName,AfxGetInstanceHandle(),SND_ASYNC|SND_NODEFAULT);
		return true;
	}
	return false;
}

//播放声音://bluker:读加密了的数据
bool CGameFrameDlg::PlayGameSoundFromFip(LPCTSTR pszSoundName)
{
/*	if ((!m_bAllowSound)||(!IsWindowVisible()))return false;
	g_WaveShow.PlayWav(pszSoundName);*/
	return true;
}

//播放声音
bool CGameFrameDlg::PlayGameSoundFromFip2(UINT uEventType,WORD wToWho)
{
	if ((!m_bAllowSound)||(!IsWindowVisible()))return false;

	if(wToWho==INVALID_CHAIR)
		wToWho = 0;
	else
	{
		if(wToWho==GetMeChairID())
		{
	//		g_WaveShow.PlayWav(uEventType);
			return true;
		}
		wToWho ++;
	}
	IClientKernel *icp=m_ClientKernelHelper.GetInterface();
	LPCSTR pszWavFile;//=g_WaveShow.GetEventWaveFile(uEventType);
	if(icp && pszWavFile)
	{
		CString s;
		s.Format("%s%d:%s",VOICE_SIGN,wToWho,pszWavFile);
		//::MessageBox(0,"aa",0,0);
		icp->SendChatMessage(0,s,0);
		//::MessageBox(0,"bb",0,0);
	}
	return true;
}


//播放声音
bool CGameFrameDlg::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	if ((m_bAllowSound==true)&&(IsWindowVisible()))
	{
		//加载资源
		HRSRC hResour=FindResource(hInstance,pszSoundName,TEXT("WAVE"));
		if (hResour==NULL) return false;
		HGLOBAL hGlobal=LoadResource(hInstance,hResour);
		if (hGlobal==NULL) return false;

		//播放声音
		LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
		::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY);

		//清理资源
		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}

	return true;
}

//找出大厅窗口名柄
HWND CGameFrameDlg::FindPlazaWindow()
{
	HWND hwnd=NULL;
	CString szWnd;

	if(GetCommandParam("/WndPlaza:",szWnd))
	{
		hwnd = (HWND)atol(szWnd);
		if(!::IsWindow(hwnd))
		{	
			hwnd=NULL;
		}
		else
		{
			::SendMessage(hwnd,WM_GAMECLIENT,MSG_SENDWND,(LPARAM)this->m_hWnd);
		}
	}
	if(!hwnd)
		hwnd=::FindWindow(MAIN_DLG_CLASSNAME,szProductName);
	return hwnd;
}

//插入图片
bool CGameFrameDlg::InsertImage(CBitmap * pBitmap)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertImage(pBitmap);
}

//插入名字
bool CGameFrameDlg::InsertUserName(LPCTSTR pszUserName)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertUserName(pszUserName);
}

//系统消息
bool CGameFrameDlg::InsertSystemString(LPCTSTR pszString)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertSystemString(pszString,MS_NORMAL);
}

//特色消息
bool CGameFrameDlg::InsertShtickString(LPCTSTR pszString, COLORREF crColor, bool bNewLine)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertShtickString(pszString,crColor,MS_NORMAL,bNewLine);
}

//普通消息
bool CGameFrameDlg::InsertGeneralString(LPCTSTR pszString, COLORREF crColor, bool bNewLine)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertGeneralString(pszString,crColor,MS_NORMAL,bNewLine);
}

//激活界面
void CGameFrameDlg::ActiveGameFrame()
{
	BringWindowToTop(); 
	SetForegroundWindow();
	return;
}

//切换椅子
WORD CGameFrameDlg::SwitchViewChairID(WORD wChairID)
{
	//变量定义
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	const WORD wChairCount = GetChairCount();

	//转换椅子
	WORD wViewChairID=(wChairID+wChairCount-GetMeChairID());
	switch (wChairCount)
	{
	case 2: { wViewChairID+=1; break; }
	case 3: { wViewChairID+=1; break; }
	case 4: { wViewChairID+=2; break; }
	case 5: { wViewChairID+=2; break; }
	case 6: { wViewChairID+=3; break; }
	case 7: { wViewChairID+=3; break; }
	case 8: { wViewChairID+=4; break; }
	}
	return wViewChairID%wChairCount;
}

//得到椅子数
WORD CGameFrameDlg::GetChairCount()
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->GetServerAttribute()->wChairCount;
}

//从服务器id到位置(用于CGoldFlowerPlayer)
WORD CGameFrameDlg::XChair(WORD wChairID)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	const WORD wChairCount = GetChairCount();
	if(6==wChairCount)
	{
		int fix= (wChairCount+5) - GetMeChairID();
		return (wChairID + fix) % wChairCount;
	}
	if(4==wChairCount)
	{
		int fix= (wChairCount+2) - GetMeChairID();
		return (wChairID + fix) % wChairCount;
	}
	if(3==wChairCount)
	{
		int fix= (wChairCount+1) - GetMeChairID();
		return (wChairID + fix) % wChairCount;
	}
	return 65535;
}

//从服务器id到位置(tagUserData *GetUserInfo())
WORD CGameFrameDlg::XChairUserInfo(WORD wChairID)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	const WORD wChairCount = GetChairCount();
	return (wChairID+4)%wChairCount;
	return 0;
}

//自动派桌 的 响应
bool CGameFrameDlg::SetReadyIfAutoStart(CButton *pBtnStart)
{
	CString szbAutoStart;
	if(GetCommandParam("/AutoStart:",szbAutoStart))
	{
		if(atol(szbAutoStart)==1)
		{
			if(pBtnStart)pBtnStart->ShowWindow(SW_HIDE);
			return SendUserReady(NULL,0);
		}
	}
	return false;
}

//是否启用
bool CGameFrameDlg::IsEnableSound()
{
	return m_bAllowSound;
}

//启用声音
void CGameFrameDlg::EnableSound(bool bEnable)
{
	//设置变量
	m_bAllowSound=bEnable;
	
	//读写注册
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("EnableSound"),m_bAllowSound);

	return;
}

//游戏状态
BYTE CGameFrameDlg::GetGameStatus()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return GS_FREE;
	return m_ClientKernelHelper->GetGameStatus();
}

//旁观模式
bool CGameFrameDlg::IsLookonMode()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return true;
	return m_ClientKernelHelper->IsLookonMode();
}

//允许旁观
bool CGameFrameDlg::IsAllowLookon()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return false;
	return m_ClientKernelHelper->IsAllowLookon();
}

//游戏状态
void CGameFrameDlg::SetGameStatus(BYTE bGameStatus)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return;
	m_ClientKernelHelper->SetGameStatus(bGameStatus);
	if(m_ClientKernelHelper->GetExitFlags()==false && bGameStatus==GS_FREE)
		m_ClientKernelHelper->SetUserFlags(US_LOOKON);
	return;
}

//自己位置
WORD CGameFrameDlg::GetMeChairID()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return INVALID_CHAIR;
	return m_ClientKernelHelper->GetMeChairID();
}

//时间位置
WORD CGameFrameDlg::GetTimeChairID()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return INVALID_CHAIR;
	return m_ClientKernelHelper->GetTimeChairID();
}

//获取用户
const tagUserData * CGameFrameDlg::GetUserData(WORD wChairID)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return NULL;
	return m_ClientKernelHelper->GetUserInfo(wChairID);
}

//内核接口
void * CGameFrameDlg::GetClientKernel(const IID & Guid, DWORD dwQueryVer)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return NULL;
	return m_ClientKernelHelper->QueryInterface(Guid,dwQueryVer);
}

//控件绑定
void CGameFrameDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//初始化函数
BOOL CGameFrameDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//设置属性
	ModifyStyle(WS_CAPTION|WS_BORDER,0,0);
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//显示窗口
	HWND hwnd=FindPlazaWindow();
	if(hwnd)
	{
		CRect rc;
		::GetWindowRect(hwnd,rc);
		MoveWindow(rc);
		::ShowWindow(hwnd,SW_HIDE);
	}
	else
	{
		SetWindowPos(NULL,0,0,800,600,SWP_NOMOVE);
		CenterWindow();
	}


	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_btQuit = new CRgnButton();
	m_btMax = new CRgnButton();
	m_btMin = new CRgnButton();
	m_btOption = new CRgnButton();
	m_btHelp = new CRgnButton();



	m_btQuit->Create(NULL,WS_CHILD|WS_VISIBLE,CRect(774,4,0,0),this,IDB_BT_QUIT);
	m_btMax->Create(NULL,WS_CHILD|WS_VISIBLE,CRect(754,4,0,0),this,IDB_BT_MAX);
	m_btMin->Create(NULL,WS_CHILD|WS_VISIBLE,CRect(734,4,0,0),this,IDB_BT_MIN);
	m_btOption->Create(NULL,WS_CHILD|WS_VISIBLE,CRect(714,4,0,0),this,IDB_BT_OPTION);
	m_btHelp->Create(NULL,WS_CHILD|WS_VISIBLE,CRect(694,4,0,0),this,IDB_BT_GAME_RULE);

	m_btQuit->LoadRgnImage(hInstance,IDB_BT_QUIT,0xff00ff);
	m_btMax->LoadRgnImage(hInstance,IDB_BT_MAX,0xff00ff);
	m_btMin->LoadRgnImage(hInstance,IDB_BT_MIN,0xff00ff);
	m_btOption->LoadRgnImage(hInstance,IDB_BT_OPTION,0xff00ff);
	m_btHelp->LoadRgnImage(hInstance,IDB_BT_GAME_RULE,0xff00ff);

	//工具栏
	m_ToolBar->Create(IDD_EMPTY_DLG,this);
	SetWindowRgnFromRes(m_ToolBar->GetSafeHwnd(),hInstance,TEXT("RGN_TOOLBAR"),TEXT("BIN"));
	LayerWindow(*m_ToolBar,200);
	m_ToolBar->ShowWindow(SW_SHOW);

	//用户列表框
	m_DlgUserList->Create(IDD_EMPTY_DLG,this);
	SetWindowRgnFromRes(m_DlgUserList->GetSafeHwnd(),hInstance,TEXT("RGN_USERLIST"),TEXT("BIN"));
	LayerWindow(*m_DlgUserList,200);
	m_DlgUserList->ShowWindow(SW_HIDE);
	
	//聊天消息框
	m_ChatMsgDlg->Create(IDD_EMPTY_DLG,this);
	SetWindowRgnFromRes(m_ChatMsgDlg->GetSafeHwnd(),hInstance,TEXT("RGN_CHATMSG"),TEXT("BIN"));
	LayerWindow(*m_ChatMsgDlg,200);
	m_ChatMsgDlg->ShowWindow(SW_HIDE);

	m_bAllowSound=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("EnableSound"),TRUE)?true:false;


	//创建组件
	if (m_MessageProxyHelper.CreateInstance()==false) throw TEXT("信息组件创建失败");
	m_MessageProxyHelper->SetRichEditHwnd(&m_ChatMsgDlg->m_ChatMsg);


	//创建组件
	if (m_UserFaceResHelper.CreateInstance()==false) throw TEXT("头像组件加载失败");
	if (m_ClientKernelHelper.CreateInstance()==false) throw TEXT("游戏框架内核模块加载失败");

	//创建游戏视图
	CRect rcGameView;

	m_pGameFrameView->Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcGameView,this,10);
	if (m_pGameFrameView->SetUserFaceRes(m_UserFaceResHelper.GetInterface())==false) throw TEXT("头像资源接口设置失败");

	//初始化内核
	LPCTSTR lpszCmdLine=AfxGetApp()->m_lpCmdLine;
	bool bSuccess=m_ClientKernelHelper->InitClientKernel(lpszCmdLine,m_pKernelSink);
	if (bSuccess==false) throw TEXT("游戏框架逻辑模块初始化失败");
	bSuccess=m_ClientKernelHelper->SetMessageProxy(m_MessageProxyHelper.GetInterface());
	if (bSuccess==false) throw TEXT("获取消息输出模块失败");



	m_ToolBar->m_pICK = m_ClientKernelHelper.GetInterface();
	if (InitGameFrame()==false) throw TEXT("游戏框架初始化失败");

	//显示窗口
	m_bInitDialog=true;
	m_pGameFrameView->ShowWindow(SW_SHOW);


	ShowWindow(SW_SHOW);


	if (GetSystemMetrics(SM_CXSCREEN)<=800)
	{
		if(g_GlobalOption.SetDisplay(800,600,32,g_GlobalOption.m_ScreenFrequency) || g_GlobalOption.SetDisplay(800,600,32,60))
		{
			SetWindowPos(&wndTop,0,0,800,600,SWP_DRAWFRAME|SWP_SHOWWINDOW);
		}
		m_bMaxed=true;
	}

	CRect rc;
	GetClientRect(rc);
	RectifyControl(rc.right,rc.bottom);
	return TRUE;
}

//消息解释
BOOL CGameFrameDlg::PreTranslateMessage(MSG * pMsg)
{
	//过滤消息
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

//确定消息
void CGameFrameDlg::OnOK()
{
	return;
}

//取消消息
void CGameFrameDlg::OnCancel()
{
	//退出提示
	if ((m_ClientKernelHelper.GetInterface()!=NULL)&&(m_ClientKernelHelper->GetExitFlags()==false))
	{
		WORD wChairID=GetMeChairID();
		const tagUserData * pUserData=GetUserData(wChairID);
		if (pUserData!=NULL)
		{
			int iRet=AfxMessageBox(TEXT("你正在游戏中，强行退出会扣游戏币，确实要强退吗？"),MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
			if (iRet!=IDYES) return;
		}
	}

	HWND hwnd=FindPlazaWindow();
	if(hwnd)
	{
		CRect pRect;
		GetWindowRect(pRect);
		::SetWindowPos(hwnd,NULL,pRect.left,pRect.top,0,0,
			SWP_NOSIZE);

		::ShowWindow(hwnd,SW_RESTORE);
		::SetActiveWindow(hwnd);
		::BringWindowToTop(hwnd);
		::SetForegroundWindow(hwnd);
	}

	//保存配置
	g_GlobalOption.SaveOptionParameter();

	//销毁组件
	SafeDelete(m_pKernelSink);
	m_UserFaceResHelper.CloseInstance();
	m_ClientKernelHelper.CloseInstance();

	//关闭窗口
	DestroyWindow();

	return;
}
/*
//位置改变
void CGameFrameDlg::OnWindowPosChanging(WINDOWPOS * lpwndpos)
{
	if (m_bInitDialog==false) lpwndpos->flags&=~SWP_SHOWWINDOW;
	__super::OnWindowPosChanging(lpwndpos);
}
*/
//位置改变
void CGameFrameDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;

}

//退出
void CGameFrameDlg::OnBnClickedQuit()
{
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
	return;
}

//最大化
void CGameFrameDlg::OnBnClickedMax()
{
	if(m_bMaxed)
	{
		m_bMaxed=false;
		g_GlobalOption.SetDisplayDefault();
		SetWindowPos(&wndTop,g_GlobalOption.m_ScreenW/2 - 400,g_GlobalOption.m_ScreenH/2 - 300,800,600,SWP_DRAWFRAME|SWP_SHOWWINDOW);
		
	}
	else
	{
		if(g_GlobalOption.SetDisplay(800,600,32,g_GlobalOption.m_ScreenFrequency) || g_GlobalOption.SetDisplay(800,600,32,60))
		{
			m_bMaxed=true;
			SetWindowPos(&wndTop,0,0,800,600,SWP_DRAWFRAME|SWP_SHOWWINDOW);
		}
		
	}
}

//最小化
void CGameFrameDlg::OnBnClickedMin()
{
	ShowWindow(SW_SHOWMINIMIZED);
}

//系统设置
void CGameFrameDlg::OnBnClickedOption()
{
	OnGameOptionSet();
}

//游戏帮助
void CGameFrameDlg::OnBnClickedHelp()
{
	OnGameOptionHelp();
	//TCHAR szRuleUrl[256];
	//_snprintf(szRuleUrl,sizeof(szRuleUrl),TEXT("http://"ServerDomain"/Help/?KindId=%d"),888);
	//ShellExecute(NULL,TEXT("open"),szRuleUrl,NULL,NULL,SW_SHOWDEFAULT);
}

void CGameFrameDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
CRect rc;
	GetWindowRect(rc);
	RectifyControl(rc.Width(),rc.Height());
	// TODO: 在此处添加消息处理程序代码
}

//表情消息
LRESULT CGameFrameDlg::OnHitExpression(WPARAM wParam, LPARAM lParam)
{
	CExpressionItem * pExpressionItem=m_Expression.GetExpressItem((WORD)wParam);
	if (pExpressionItem!=NULL)
	{
		CString strBuffer;
		m_ChatMsgDlg->m_ChatMsg.GetWindowText(strBuffer);
		strBuffer+=pExpressionItem->m_szTrancelate;
		m_ChatMsgDlg->m_ChatMsg.SetWindowText(strBuffer);
		m_ChatMsgDlg->m_ChatMsg.SetFocus();
		m_ChatMsgDlg->m_ChatMsg.SetSel(strBuffer.GetLength(),strBuffer.GetLength());
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////


