#include "Stdafx.h"
#include "GameFrame.h"
#include "DlgOption.h"
#include "GlobalUnits.h"
#include "SystemOption.h"
#include "DlgControlBar.h"

//////////////////////////////////////////////////////////////////////////

//控制栏指针
CDlgControlBar		* g_pControlBar=NULL;

//消息定义
#define WM_CLOSE_ROOM_VIEW_ITEM			WM_USER+10						//关闭消息

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgControlBar, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_PLAZA, OnBnClickedPlaza)
	ON_BN_CLICKED(IDC_QUIT_GAME, OnBnClickedQuitGame)
	ON_BN_CLICKED(IDC_VIEW_ITEM1, OnBnClickedViewItem1)
	ON_BN_CLICKED(IDC_VIEW_ITEM2, OnBnClickedViewItem2)
	ON_BN_CLICKED(IDC_VIEW_ITEM3, OnBnClickedViewItem3)
	ON_BN_CLICKED(IDC_VIEW_ITEM4, OnBnClickedViewItem4)
	ON_BN_CLICKED(IDC_SYSTEM_OPTION, OnBnClickedGlobalOption)
	ON_BN_CLICKED(IDC_COMPANION_MANAGER, OnBnClickedCompanion)
	ON_BN_CLICKED(IDC_SWITCH_ACCOUNTS, OnBnClickedSwitchAccounts)
	ON_BN_CLICKED(IDB_BAR_BUTTON_PERSIONSET, OnBnClickedPrisonSet)
	ON_BN_CLICKED(IDC_SKIN, OnBnClickedChangeTheme)
	ON_MESSAGE(WM_CLOSE_ROOM_VIEW_ITEM,OnCloseRoomViewItem)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//析构函数
CDlgControlBar::CDlgControlBar() : CDialog(IDD_CONTROL_BAR)
{
	g_pControlBar=this;
	m_pPlazaViewItem=NULL;
	m_pActiveViewItem=NULL;
	m_pRoomViewItemCreate=NULL;
	memset(m_pRoomViewItem,0,sizeof(m_pRoomViewItem));
}

//析构函数
CDlgControlBar::~CDlgControlBar()
{
	//删除房间
	for (int i=0;i<CountArray(m_pRoomViewItem);i++)
	{
		if (m_pRoomViewItem[i]==NULL) break;
		SafeDelete(m_pRoomViewItem[i]);
		m_pRoomViewItem[i]=NULL;
	}

	return;
}

//控件绑定
void CDlgControlBar::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAZA, m_btGamePlaza);
	DDX_Control(pDX, IDC_SWITCH_ACCOUNTS, m_btSwitchUser);
	DDX_Control(pDX, IDC_SYSTEM_OPTION, m_btGlobalOption);
	DDX_Control(pDX, IDB_BAR_BUTTON_PERSIONSET, m_btPrisonSet);

	
	DDX_Control(pDX, IDC_COMPANION_MANAGER, m_btCompanion);
	DDX_Control(pDX, IDC_VIEW_ITEM1, m_btViewItem1);
	DDX_Control(pDX, IDC_VIEW_ITEM2, m_btViewItem2);


	DDX_Control(pDX, IDC_SKIN, m_btChangeTheme);
	
	m_btViewItem1.SetTextColor(0);
	m_btViewItem2.SetTextColor(0);
}

//初始化函数
BOOL CDlgControlBar::OnInitDialog()
{
	__super::OnInitDialog();

	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_Frame_BL.SetLoadInfo(IDB_FRAME_BL,hInstance);
	m_Frame_BM.SetLoadInfo(IDB_FRAME_BM,hInstance);

	//m_ImageMin.SetLoadInfo(IDB_BAR_MIN,hInstance);
	//m_ImageRight.SetLoadInfo(IDB_BAR_RIGHT,hInstance);

	//设置按钮
	m_btGamePlaza.SetButtonImage(IDB_BAR_ITEM_HOME,hInstance,false);
	m_btViewItem1.SetButtonImage(IDB_BAR_ITEM,hInstance,false);
	m_btViewItem2.SetButtonImage(IDB_BAR_ITEM,hInstance,false);
	m_btSwitchUser.SetButtonImage(IDB_BAR_BUTTON_NUMBER,hInstance,false);
	m_btPrisonSet.SetButtonImage(IDB_BAR_BUTTON_PERSIONSET,hInstance,false);
	m_btGlobalOption.SetButtonImage(IDB_BAR_BUTTON_SET,hInstance,false);
	m_btCompanion.SetButtonImage(IDB_BAR_BUTTON_FRIEND,hInstance,false);
	m_btChangeTheme.SetButtonImage(IDB_BAR_BUTTON_PERSIONSET,hInstance,false);
	

	return TRUE;
}

//重画函数
void CDlgControlBar::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClientRect;
	GetClientRect(&rcClientRect);

	//加载位图
	CImageHandle FrameBLHandle(&m_Frame_BL);
	CImageHandle FrameBMHandle(&m_Frame_BM);

	//del:CImageHandle ImageMHandle(&m_ImageMin);
	//del:CImageHandle ImageRHandle(&m_ImageRight);

	//创建缓冲图
	CImage ImageBuffer;
	ImageBuffer.Create(rcClientRect.Width(),rcClientRect.Height(),16);
	HDC hDCBuffer=ImageBuffer.GetDC();

	//绘画界面
	m_Frame_BL.BitBlt(hDCBuffer,0,0);

	m_Frame_BM.StretchBlt(hDCBuffer,
			CRect(m_Frame_BL.GetWidth(),0,rcClientRect.right,m_Frame_BM.GetHeight()),
			CRect(0,0,m_Frame_BM.GetWidth(),m_Frame_BM.GetHeight())
			);


	ImageBuffer.ReleaseDC();

	//绘画界面
	ImageBuffer.BitBlt(dc,0,0);

	return;
}

//位置消息
void CDlgControlBar::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//判断状态
	if (m_btGamePlaza.GetSafeHwnd()==NULL) return;

	//获取位置
	CRect rcItemRect,rcButtonRect;
	m_btGamePlaza.GetWindowRect(&rcItemRect);

	//变量定义
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE|SWP_NOCOPYBITS;
	CButton * pButton[]={&m_btCompanion,&m_btGlobalOption,&m_btSwitchUser,&m_btPrisonSet, &m_btChangeTheme};

	//移动控件
	HDWP hDwp=BeginDeferWindowPos(CountArray(pButton)+5);
	m_btGlobalOption.GetWindowRect(&rcButtonRect);

	DeferWindowPos(hDwp,m_btGamePlaza,NULL,84,0,0,0,uFlags);
	DeferWindowPos(hDwp,m_btViewItem1,NULL,84+rcItemRect.Width()+10,0,0,0,uFlags);
	DeferWindowPos(hDwp,m_btViewItem2,NULL,84+rcItemRect.Width()*2+20,0,0,0,uFlags);
	for (int i=0;i<CountArray(pButton);i++)
	{
		DeferWindowPos(hDwp,pButton[i]->m_hWnd,NULL,cx-80*(i+1),0,0,0,uFlags);
	}
	EndDeferWindowPos(hDwp);

	return;
}

//关闭房间
LRESULT CDlgControlBar::OnCloseRoomViewItem(WPARAM wParam, LPARAM lParam)
{
	//释放房间
	IRoomViewItem * pIRoomViewItem=(IRoomViewItem *)wParam;
	if (pIRoomViewItem!=NULL) pIRoomViewItem->Release();

	return 0;
}

//绘画背景
BOOL CDlgControlBar::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//设置位置
void CDlgControlBar::SetViewItemRect(int nXPos, int nYPos, int nWidth, int nHeight)
{
	//设置变量
	m_rcViewItem.SetRect(nXPos,nYPos,nXPos+nWidth,nYPos+nHeight);

	//移动控件
	if (m_pActiveViewItem!=NULL) 
	{
		HWND hWndActive=m_pActiveViewItem->GetWindowHandle();
		if (hWndActive!=NULL)
		{
			int nXPos=m_rcViewItem.left,nYPos=m_rcViewItem.top;
			int nWidth=m_rcViewItem.Width(),nHeight=m_rcViewItem.Height();
			::MoveWindow(hWndActive,nXPos,nYPos,nWidth,nHeight,TRUE);
		}
	}

	return;
}

//查找房间
CRoomViewItem * CDlgControlBar::SearchRoomViewItem(WORD wKindID, WORD wServerID)
{
	tagGameServer * pGameServer=NULL;
	for (int i=0;i<CountArray(m_pRoomViewItem);i++)
	{
		if (m_pRoomViewItem[i]==NULL) return NULL;
		pGameServer=m_pRoomViewItem[i]->GetServerInfo();
		if ((pGameServer->wKindID==wKindID)&&(pGameServer->wServerID==wServerID)) return m_pRoomViewItem[i];
	}

	return NULL;
}
//进入房间
CRoomViewItem * CDlgControlBar::CreateRoomViewItem(WORD wKindID,WORD wServerID)
{
	CListServer *pListServer;
	int i=0;
	while(true)
	{
		//枚举房间
		pListServer=g_GlobalUnits.m_ServerListManager.EnumServerItem(i++);
		if(!pListServer)break;
		tagGameServer * pGameServer=pListServer->GetItemInfo();
		if(pGameServer && pGameServer->wKindID==wKindID && pGameServer->wServerID==wServerID)
		{
			return CreateRoomViewItem(pListServer);
		}
	}
	return NULL;
}

//进入房间
CRoomViewItem * CDlgControlBar::CreateRoomViewItem(CListServer * pListServer)
{
	//判断状态
	tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
	if (GlobalUserData.dwUserID==0L)
	{
		ShowMessageBox(TEXT("你还没有登陆，请先登陆游戏广场！"),MB_ICONQUESTION);
		return NULL;
	}

	//效验参数
	ASSERT(pListServer!=NULL);
	CListKind * pListKind=pListServer->GetListKind();
	tagGameKind * pGameKind=pListKind->GetItemInfo();
	tagGameServer * pGameServer=pListServer->GetItemInfo();

	//判断连接
	if (m_pRoomViewItemCreate!=NULL)
	{
		tagGameServer * pGameServerCreate=m_pRoomViewItemCreate->GetServerInfo();
		if ((pGameServer->wKindID==pGameServerCreate->wKindID)&&
			(pGameServer->wServerID==pGameServerCreate->wServerID))
			return NULL;
	}

	//寻找房间
	CRoomViewItem * pRoomViewItem=SearchRoomViewItem(pGameServer->wKindID,pGameServer->wServerID);
	if (pRoomViewItem!=NULL) 
	{
		ActiveRoomViewItem(pRoomViewItem);
		return pRoomViewItem;
	}

	//进入房间数目
	if (m_pRoomViewItem[CountArray(m_pRoomViewItem)-1]!=NULL)
	{
		ShowMessageBox(TEXT("进入的游戏房间太多了，请关闭部分游戏房间再试！"),MB_ICONINFORMATION);
		return NULL;
	}

	//获取版本
	CWinFileInfo WinFileInfo;

	if (WinFileInfo.OpenWinFile(pGameKind->szProcessName)==false)
	{
		//TCHAR szBuffer[512]=TEXT("");
		//_snprintf(szBuffer,sizeof(szBuffer),TEXT("【%s】还没有安装，现在是否下载？"),pGameKind->szKindName);
		//int nResult=ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
		//if (nResult==IDYES)
			g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
		return NULL;
	}

	//获取版本
	DWORD dwFileVerMS=0L,dwFileVerLS=0L;
	WinFileInfo.GetFileVersion(dwFileVerMS,dwFileVerLS);

	//版本分析
	BYTE cbFileVer1=(BYTE)(HIWORD(dwFileVerMS));
	BYTE cbFileVer2=(BYTE)(LOWORD(dwFileVerMS));
	BYTE cbFileVer3=(BYTE)(HIWORD(dwFileVerLS));
	BYTE cbFileVer4=(BYTE)(LOWORD(dwFileVerLS));
	BYTE cbListVer1=(BYTE)(LOWORD(LOWORD(pGameKind->dwMaxVersion)));
	BYTE cbListVer2=(BYTE)(HIBYTE(LOWORD(pGameKind->dwMaxVersion)));
	BYTE cbListVer3=(BYTE)(LOBYTE(HIWORD(pGameKind->dwMaxVersion)));
	BYTE cbListVer4=(BYTE)(HIBYTE(HIWORD(pGameKind->dwMaxVersion)));

	//李明改
	//////////判断版本
	////////if ((cbFileVer1!=cbListVer1)||(cbFileVer2!=cbListVer2)||(cbFileVer3!=cbListVer3))
	////////{
	////////	TCHAR szBuffer[512]=TEXT("");
	////////	//_snprintf(szBuffer,sizeof(szBuffer),TEXT("【%s】已经更新为 %ld.%ld.%ld.%ld 版本，你的版本不能继续使用，现在是否下载？"),pGameKind->szKindName,
	////////	//	cbListVer1,cbListVer2,cbListVer3,cbListVer4);
	////////	//int nResult=ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
	////////	//if (nResult==IDYES)
	////////		g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
	////////	return NULL;
	////////}

	//////////兼容版本
	////////if (cbFileVer4!=cbListVer4)
	////////{
	////////	TCHAR szBuffer[512]=TEXT("");
	////////	_snprintf(szBuffer,sizeof(szBuffer),TEXT("【%s】已经更新为 %ld.%ld.%ld.%ld 版本，现在是否下载升级？"),pGameKind->szKindName,
	////////		cbListVer1,cbListVer2,cbListVer3,cbListVer4);
	////////	if (ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1)==IDYES) 
	////////	{
	////////		//g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
	////////		char strPathBuf[MAX_PATH];
	////////		::GetCurrentDirectory(MAX_PATH,strPathBuf);
	////////		strcat(strPathBuf,"\\update.exe");

	////////		STARTUPINFO stinfo;
 ////////   PROCESS_INFORMATION procinfo;
 ////////   ZeroMemory(&stinfo, sizeof(stinfo));
 ////////   stinfo.cb = sizeof(stinfo);
 ////////   ZeroMemory(&procinfo, sizeof(procinfo));
	////////CreateProcess(NULL,_T(strPathBuf),
	////////	NULL,NULL,FALSE, NORMAL_PRIORITY_CLASS,NULL,NULL,&stinfo,&procinfo);
	////////		return NULL;
	////////	}
	////////}

	//创建房间
	try
	{
		pRoomViewItem=new CRoomViewItem;
		if (pRoomViewItem==NULL) return NULL;
		pRoomViewItem->Create(IDD_GAME_ROOM,AfxGetMainWnd());
		if (pRoomViewItem->InitRoomViewItem(pListServer)==false) throw TEXT("游戏房间初始化失败！");
	}
	catch (...) { SafeDelete(pRoomViewItem); }
	if (pRoomViewItem==NULL)
	{
		ShowMessageBox(TEXT("游戏房间创建失败"),MB_ICONQUESTION);
		return NULL;
	}

	//连接游戏
	bool bSuccess=pRoomViewItem->ConnectGameServer();
	if (bSuccess==false)
	{
		pRoomViewItem->DestroyWindow();
		SafeDelete(pRoomViewItem);
		return NULL;
	}

	//设置变量
	m_pRoomViewItemCreate=pRoomViewItem;

	//设置提示
	CString strBuffer;
	strBuffer.Format(TEXT("正在连接房间，请稍候..."));
	g_GlobalAttemper.ShowStatusMessage(strBuffer,pRoomViewItem);
	
	return m_pRoomViewItemCreate;
}

//显示网页
void CDlgControlBar::WebBrowse(LPCTSTR lpszUrl, bool bForceBrowse)
{
	//激活广场
	ActivePlazaViewItem();

	//打开网页
	//if (m_pPlazaViewItem!=NULL) m_pPlazaViewItem->WebBrowse(lpszUrl,bForceBrowse);

	return;
}

//显示消息
int CDlgControlBar::ShowMessageBox(LPCTSTR pszMessage, UINT nType)
{
	int nResult=MessageBox(pszMessage,TEXT("游戏大厅"),nType);
	return nResult;
}

//激活广场
void CDlgControlBar::ActivePlazaViewItem()
{
	if(GetParent()->IsWindowVisible())g_GlobalOption.PlayMedia("Welcome.mp3");
	//获取窗口
	HWND hWndActive=NULL;
	if (m_pActiveViewItem!=NULL) hWndActive=m_pActiveViewItem->GetWindowHandle();
	if (m_pPlazaViewItem->GetSafeHwnd()!=hWndActive)
	{
		m_pActiveViewItem=m_pPlazaViewItem;
		m_pPlazaViewItem->MoveWindow(&m_rcViewItem);
		m_pPlazaViewItem->ShowWindow(SW_SHOW);
		if (hWndActive!=NULL) ::ShowWindow(hWndActive,SW_HIDE);
	}

	return;
}

//激活房间
void CDlgControlBar::ActiveRoomViewItem(CRoomViewItem * pRoomViewItem)
{
	g_GlobalOption.PlayMedia("Desk.mp3");
	//效验参数
	ASSERT(pRoomViewItem!=NULL);
	if (pRoomViewItem==NULL) return;

	//判断创建
	if (m_pRoomViewItemCreate==pRoomViewItem)
	{
		m_pRoomViewItemCreate=NULL;
		MoveMemory(m_pRoomViewItem+1,m_pRoomViewItem,sizeof(m_pRoomViewItem[0])*(CountArray(m_pRoomViewItem)-1));
		m_pRoomViewItem[0]=pRoomViewItem;
	}

	//激活房间
	HWND hWndActive=NULL;
	if (m_pActiveViewItem!=NULL) hWndActive=m_pActiveViewItem->GetWindowHandle();
	if (pRoomViewItem->GetSafeHwnd()!=hWndActive)
	{
		m_pActiveViewItem=pRoomViewItem;
		pRoomViewItem->MoveWindow(&m_rcViewItem);
		pRoomViewItem->ShowWindow(SW_SHOW);
		if (hWndActive!=NULL) ::ShowWindow(hWndActive,SW_HIDE);
	}

	//更新控制
	UpdateControlButton();

	return;
}

//关闭房间
void CDlgControlBar::CloseRoomViewItem(IRoomViewItem * pIRoomViewItem)
{
	//效验参数
	ASSERT(pIRoomViewItem!=NULL);
	if (pIRoomViewItem==NULL) return;

	//设置变量
	if (m_pRoomViewItemCreate!=pIRoomViewItem)
	{
		for (int i=0;i<CountArray(m_pRoomViewItem);i++)
		{
			if (m_pRoomViewItem[i]==NULL) break;
			if (m_pRoomViewItem[i]==pIRoomViewItem) 
			{
				//移动数组
				if ((i+1)!=CountArray(m_pRoomViewItem))
				{
					int nMoveSize=(CountArray(m_pRoomViewItem)-i-1)*sizeof(m_pRoomViewItem[0]);
					MoveMemory(m_pRoomViewItem+i,m_pRoomViewItem+i+1,nMoveSize);
				}
				m_pRoomViewItem[CountArray(m_pRoomViewItem)-1]=NULL;

				//激活视图
				if (m_pActiveViewItem==pIRoomViewItem)
				{
					int nActivePos=CountArray(m_pRoomViewItem);
					for (int i=CountArray(m_pRoomViewItem)-1;i>=0;i--)
					{
						if (m_pRoomViewItem[i]!=NULL)
						{
							nActivePos=i;
							break;
						}
					}
					if (nActivePos==CountArray(m_pRoomViewItem)) ActivePlazaViewItem();
					else ActiveRoomViewItem(m_pRoomViewItem[nActivePos]);
				}

				//更新控制
				UpdateControlButton();

				break;
			}
		}
	}
	else m_pRoomViewItemCreate=NULL;

	//关闭房间
	pIRoomViewItem->CloseRoomViewItem();
	PostMessage(WM_CLOSE_ROOM_VIEW_ITEM,(WPARAM)pIRoomViewItem,(LPARAM)pIRoomViewItem);

	return;
}

//关闭房间
void CDlgControlBar::CloseAllRoomViewItem()
{
	//关闭房间
	IRoomViewItem * pIRoomViewItem=NULL;
	for (int i=0;i<CountArray(m_pRoomViewItem);i++)
	{
		if (m_pRoomViewItem[i]==NULL) break;

		//关闭房间
		pIRoomViewItem=GET_OBJECTPTR_INTERFACE(m_pRoomViewItem[i],IRoomViewItem);
		pIRoomViewItem->CloseRoomViewItem();
		PostMessage(WM_CLOSE_ROOM_VIEW_ITEM,(WPARAM)pIRoomViewItem,(LPARAM)pIRoomViewItem);

		//设置变量
		m_pRoomViewItem[i]=NULL;
	}

	//关闭创建房间
	if (m_pRoomViewItemCreate!=NULL) 
	{
		//关闭房间
		g_GlobalAttemper.DestroyStatusWnd(NULL);
		m_pRoomViewItemCreate->CloseRoomViewItem();
		pIRoomViewItem=GET_OBJECTPTR_INTERFACE(m_pRoomViewItemCreate,IRoomViewItem);
		PostMessage(WM_CLOSE_ROOM_VIEW_ITEM,(WPARAM)pIRoomViewItem,(LPARAM)pIRoomViewItem);

		//设置变量
		m_pRoomViewItemCreate=NULL;
	}

	//更新控制
	UpdateControlButton();

	return;
}

//关闭当前
void CDlgControlBar::CloseCurrentViewItem()
{
	//房间判断
	ASSERT(m_pActiveViewItem!=NULL);
	if (m_pActiveViewItem!=m_pPlazaViewItem)
	{
		IRoomViewItem * pIRoomViewItem=GET_OBJECTPTR_INTERFACE(m_pActiveViewItem,IRoomViewItem);
		ASSERT(pIRoomViewItem!=NULL);
		CloseRoomViewItem(pIRoomViewItem);
	}
	else AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0); 

	return;
}


//更新控制
void CDlgControlBar::UpdateControlButton()
{
	if (m_pRoomViewItem[0]!=NULL)
	{
		tagGameKind * pGameKind=m_pRoomViewItem[0]->GetKindInfo();
		m_btViewItem1.ShowWindow(SW_SHOW);
		m_btViewItem1.SetWindowText(pGameKind->szKindName);
		m_btViewItem1.SetTitlePos(26-(int)strlen(pGameKind->szKindName)*3,12);
	}
	else m_btViewItem1.ShowWindow(SW_HIDE);

	if (m_pRoomViewItem[1]!=NULL)
	{
		tagGameKind * pGameKind=m_pRoomViewItem[1]->GetKindInfo();
		m_btViewItem2.ShowWindow(SW_SHOW);
		m_btViewItem2.SetWindowText(pGameKind->szKindName);
		m_btViewItem2.SetTitlePos(26-(int)strlen(pGameKind->szKindName)*3,12);
	}
	else m_btViewItem2.ShowWindow(SW_HIDE);

	return;
}

//游戏广场
void CDlgControlBar::OnBnClickedPlaza()
{
	if (m_pActiveViewItem!=m_pPlazaViewItem)
	{
		if (ShowMessageBox(TEXT("您要退出当前房间吗?"),MB_ICONINFORMATION|MB_YESNO)==IDYES)
		{
			CloseCurrentViewItem();
		}
	}
	//ActivePlazaViewItem();
	return;
}

#include "DlgChangeTheme.h"
//更换主题
void CDlgControlBar::OnBnClickedChangeTheme()
{
	CDlgChangeTheme dlg;
	dlg.DoModal();
	return;
}

//游戏房间
void CDlgControlBar::OnBnClickedViewItem1()
{
	ActiveRoomViewItem(m_pRoomViewItem[0]);
	return;
}

//游戏房间
void CDlgControlBar::OnBnClickedViewItem2()
{
	ActiveRoomViewItem(m_pRoomViewItem[1]);
	return;
}

//游戏房间
void CDlgControlBar::OnBnClickedViewItem3()
{
	ActiveRoomViewItem(m_pRoomViewItem[2]);
	return;
}

//游戏房间
void CDlgControlBar::OnBnClickedViewItem4()
{
	ActiveRoomViewItem(m_pRoomViewItem[3]);
	return;
}

//切换按钮
void CDlgControlBar::OnBnClickedSwitchAccounts()
{
	//状态判断
	tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
	if (GlobalUserData.dwUserID!=0L)
	{
		//切换询问
		if (m_pRoomViewItem[0]!=NULL)
		{
			const TCHAR szMessage[]=TEXT("切换用户帐号，将会关闭所有游戏房间，确实要切换用户帐号吗？ ");
			int iResult=ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
			if (iResult!=IDYES) return;
		}
		else
		{
			const TCHAR szMessage[]=TEXT("确实要注销当前用户，切换用户帐号吗？ ");
			int iResult=ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
			if (iResult!=IDYES) return;
		}

		//关闭房间
		ActivePlazaViewItem();
		try{
			CloseAllRoomViewItem();
		}catch(...){;}
	}
	CGameFrame *pWnd = (CGameFrame *)AfxGetMainWnd();
	if(pWnd)pWnd->m_DlgGamePlaza.ClearGameButtons();
	//设置变量
	memset(&GlobalUserData,0,sizeof(GlobalUserData));

	//投递消息
	m_pPlazaViewItem->SendLogonMessage();

	return;
}

//个人设置
void CDlgControlBar::OnBnClickedPrisonSet()
{
	g_GlobalUnits.GoWeb("/ModInformation.asp",true);
	return;
}

//关系按钮
void CDlgControlBar::OnBnClickedCompanion()
{
	g_GlobalUnits.m_CompanionManager->ShowCompanionManager();
	return;
}

//配置按钮
void CDlgControlBar::OnBnClickedGlobalOption()
{

	//获取参数
	CGameOption * pGameOption=NULL;
	CServerOption * pServerOption=NULL;
	if (m_pActiveViewItem!=m_pPlazaViewItem)
	{
		for (int i=0;i<CountArray(m_pRoomViewItem);i++)
		{
			if (m_pRoomViewItem[i]==NULL) break;
			if (m_pActiveViewItem==m_pRoomViewItem[i])
			{
				pGameOption=m_pRoomViewItem[i]->GetGameOption();
				pServerOption=m_pRoomViewItem[i]->GetServerOption();
				break;
			}
		}
	}

	//显示窗口
	CDlgOption DlgOption(pGameOption,pServerOption);
	INT_PTR iResult=DlgOption.DoModal();
	if (iResult==IDOK)
	{
		for (int i=0;i<CountArray(m_pRoomViewItem);i++)
		{
			if (m_pRoomViewItem[i]==NULL) break;
			m_pRoomViewItem[i]->SendUserRule();
		}
	}

	return;
}

//退出按钮
void CDlgControlBar::OnBnClickedQuitGame()
{
	CloseCurrentViewItem();
	return;
}

//////////////////////////////////////////////////////////////////////////
