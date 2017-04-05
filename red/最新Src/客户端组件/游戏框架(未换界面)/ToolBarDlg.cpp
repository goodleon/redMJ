#include "Stdafx.h"
#include "Resource.h"
#include "ToolBarDlg.h"

#include "GameFrameDlg.h"
#include ".\toolbardlg.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDialogIX, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CDialogIX::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

BOOL CDialogIX::OnEraseBkgnd(CDC* pDC)
{
	return true;
}


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgToolBar, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDB_TOOLBAR_SHOWCHAT, OnBnClickedShowChat)
	ON_BN_CLICKED(IDB_TOOLBAR_SHOWUSER, OnBnClickedShowUserList)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgToolBar::CDlgToolBar()
{
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_MainBg.SetLoadInfo(IDB_TOOLBAR,hInstance,true);
	return;
}

//析构函数
CDlgToolBar::~CDlgToolBar()
{
	SafeDelete(m_btChatSend);
	SafeDelete(m_btChatDict);
	SafeDelete(m_btShowChat);
	SafeDelete(m_btShowUserList);
}

//初始化函数
BOOL CDlgToolBar::OnInitDialog()
{
	__super::OnInitDialog();


	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_btChatSend = new CRgnButton();
	m_btChatDict = new CRgnButton();
	m_btShowChat = new CRgnButton();
	m_btShowUserList = new CRgnButton();


	m_btChatSend->Create(NULL,WS_CHILD|WS_VISIBLE,CRect(234,3,0,0),this,IDOK);
	m_btChatDict->Create(NULL,WS_CHILD|WS_VISIBLE,CRect(254,3,0,0),this,IDB_TOOLBAR_CHATDICT);
	m_btShowChat->Create(NULL,WS_CHILD|WS_VISIBLE,CRect(274,3,0,0),this,IDB_TOOLBAR_SHOWCHAT);
	m_btShowUserList->Create(NULL,WS_CHILD|WS_VISIBLE,CRect(371,3,0,0),this,IDB_TOOLBAR_SHOWUSER);


	m_btChatSend->LoadRgnImage(hInstance,IDB_TOOLBAR_CHATSEND,0xff00ff);
	m_btChatDict->LoadRgnImage(hInstance,IDB_TOOLBAR_CHATDICT,0xff00ff);
	m_btShowChat->LoadRgnImage(hInstance,IDB_TOOLBAR_SHOWCHAT,0xff00ff);
	m_btShowUserList->LoadRgnImage(hInstance,IDB_TOOLBAR_SHOWUSER,0xff00ff);



	m_ChatEdit = new CEdit();
	m_ChatEdit->Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|ES_NOHIDESEL|WS_BORDER, CRect(39,5,232,21),this,0);
	::SendMessage(m_ChatEdit->GetSafeHwnd(),WM_SETFONT,(long)(HFONT)g_GlobalOption.m_Font,0);

	return FALSE;
}

void CDlgToolBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rc;
	GetClientRect(rc);

	CImageHandle MainBgHandle(&m_MainBg);

	//绘画
	m_MainBg.BitBlt(dc,0,0);
}

void CDlgToolBar::OnOK()
{
	//聊天信息
	if(::GetTickCount() - m_dwLastChat<3000)
	{
		MessageBox("您说话太快了，喝杯茶休息一会！");
		return;
	}
	TCHAR szChatMessage[MAX_CHAT_LEN]=TEXT("");
	m_ChatEdit->GetWindowText(szChatMessage,CountArray(szChatMessage));
	if (szChatMessage[0]!=0)
	{
		
		DWORD dwTargetUserID=0L;
		/*
		int nItem=m_ChatObject.GetCurSel();
		if ((nItem!=LB_ERR)&&(nItem!=0)) 
		{
			dwTargetUserID=(DWORD)m_ChatObject.GetItemData(nItem);
		}*/
		m_ChatEdit->SetWindowText(TEXT(""));
		m_pICK->SendChatMessage(dwTargetUserID,szChatMessage,g_GlobalOption.m_crChatTX);
		m_dwLastChat = ::GetTickCount();
	}

	//设置界面
	//((CGameFrameDlg*)AfxGetMainWnd())->m_ChatMsgDlg->Show();
	m_ChatEdit->SetFocus();
	return;
};


void CDlgToolBar::OnLButtonDown(UINT nFlags, CPoint point)
{
}

//显/隐 消息框
afx_msg void CDlgToolBar::OnBnClickedShowChat()
{
	HWND hwnd = ((CGameFrameDlg*)AfxGetMainWnd())->m_ChatMsgDlg->GetSafeHwnd();

	if(::IsWindowVisible(hwnd))
	{
		::ShowWindow(hwnd,SW_HIDE);
	}
	else
	{
		::ShowWindow(hwnd,SW_NORMAL);
	}
}

//显/隐用户框
afx_msg void CDlgToolBar::OnBnClickedShowUserList()
{
	HWND hwnd = ((CGameFrameDlg*)AfxGetMainWnd())->m_DlgUserList->GetSafeHwnd();

	if(::IsWindowVisible(hwnd))
	{
		::ShowWindow(hwnd,SW_HIDE);
	}
	else
	{
		::ShowWindow(hwnd,SW_NORMAL);
	}
}



/////////////////////////////////////////////////////////////////////////////////////////
//用户列表框

BEGIN_MESSAGE_MAP(CDlgUserList, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDB_BT_QUIT_X, OnBnClickedHide)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

CDlgUserList::CDlgUserList()
{
	m_dwTimes=0;
}

CDlgUserList::~CDlgUserList()
{
}

void CDlgUserList::OnClose()
{
	KillTimer(497);
	CDialogIX::OnClose();
}

BOOL CDlgUserList::OnInitDialog()
{
	if(m_ListUsers.Create(NULL, WS_CHILD|SS_LEFT|SS_NOTIFY|WS_VISIBLE,CRect(0,0,0,0), this))
	{
		HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
		m_ListUsers.m_xTop.SetLoadInfo(USERLIST_T,hInstance,true);
		m_ListUsers.m_xLeft.SetLoadInfo(USERLIST_L,hInstance,true);
		m_ListUsers.m_xRight.SetLoadInfo(USERLIST_R,hInstance,true);
		m_ListUsers.m_xBottom.SetLoadInfo(USERLIST_B,hInstance,true);
		m_ListUsers.m_xScrollT.SetLoadInfo(USERLIST_S_T,hInstance,true);
		m_ListUsers.m_xScrollM.SetLoadInfo(USERLIST_S_M,hInstance,true);
		m_ListUsers.m_xScrollB.SetLoadInfo(USERLIST_S_B,hInstance,true);
		m_ListUsers.m_xThumb.SetLoadInfo(USERLIST_S_THUMB,hInstance,true);
		/*
		for(int i=100001;i<=100021;i++)
		{
			int nBtn=m_ListUsers.AddButton(hInstance,USERLIST_BTN,IMG_LISTCTRL_CMD_ID,
							"枯顶替基枯顶替基枯顶替基",
							0,3,0x505050,0);
		}//*/
		m_ListUsers.UpdateButton();
	}
	m_ListUsers.SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE);
	
	m_ListUsers.SetHeight(159);
	m_ListUsers.UpdateButton();
	//SetWindowPos(NULL,0,0,m_ListUsers.GetWidth(),m_ListUsers.GetHeight(),SWP_NOMOVE);

	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_btQuit.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(89,3,0,0),this,IDB_BT_QUIT_X);
	m_btQuit.LoadRgnImage(hInstance,IDB_BT_QUIT_X,0xff00ff);
	SetTimer(497,200,NULL);
	return true;
}

//时间消息
void CDlgUserList::OnTimer(UINT nIDEvent)
{
	//时间处理
	if (nIDEvent==497)
	{
		POINT pt;
		CRect rc_Self,rc_ToolBar;
		GetCursorPos(&pt);
		GetWindowRect(rc_Self);
		::GetWindowRect(((CGameFrameDlg*)AfxGetMainWnd())->m_ToolBar->GetSafeHwnd(),rc_ToolBar);

		if(PtInRect(rc_Self,pt) || PtInRect(rc_ToolBar,pt))
		{//鼠标在窗口上
			m_dwTimes=0;
		}
		else
		{
			if(IsWindowVisible())
			{
				m_dwTimes ++;
				if(m_dwTimes>8)
				{
					ShowWindow(SW_HIDE);
				}
			}
			else
			{
				m_dwTimes=0;
			}
		}
		return;
	}

	__super::OnTimer(nIDEvent);
}

//显/隐
afx_msg void CDlgUserList::OnBnClickedHide()
{
	ShowWindow(SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgChatMsg, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDB_BT_QUIT_X, OnBnClickedHide)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

CDlgChatMsg::CDlgChatMsg()
{
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_Msg_T.SetLoadInfo(IDB_CHATMSG_T,hInstance,true);
	m_Msg_B.SetLoadInfo(IDB_CHATMSG_B,hInstance,true);
	m_Msg_L.SetLoadInfo(IDB_CHATMSG_L,hInstance,true);
	m_Msg_R.SetLoadInfo(IDB_CHATMSG_R,hInstance,true);
}

CDlgChatMsg::~CDlgChatMsg()
{
}

void CDlgChatMsg::OnClose()
{
	KillTimer(498);
	CDialogIX::OnClose();
}

void CDlgChatMsg::Show()
{
	m_dwTimes=0;
	ShowWindow(SW_NORMAL);
}

//时间消息
void CDlgChatMsg::OnTimer(UINT nIDEvent)
{
	//时间处理
	if (nIDEvent==498)
	{
		POINT pt;
		CRect rc_CharMsg,rc_ToolBar;
		GetCursorPos(&pt);
		GetWindowRect(rc_CharMsg);
		::GetWindowRect(((CGameFrameDlg*)AfxGetMainWnd())->m_ToolBar->GetSafeHwnd(),rc_ToolBar);

		if(PtInRect(rc_CharMsg,pt) || PtInRect(rc_ToolBar,pt))
		{//鼠标在窗口上
			m_dwTimes=0;
		}
		else
		{
			if(IsWindowVisible())
			{
				m_dwTimes ++;
				if(m_dwTimes>8)
				{
					ShowWindow(SW_HIDE);
				}
			}
			else
			{
				m_dwTimes=0;
			}
		}
		return;
	}

	__super::OnTimer(nIDEvent);
}

BOOL CDlgChatMsg::OnInitDialog()
{
	m_ChatMsg.Create(WS_CHILD|SS_NOTIFY|WS_VISIBLE|ES_MULTILINE | ES_NOHIDESEL | ES_READONLY | ES_WANTRETURN | WS_VSCROLL,
		CRect(17,13,292,153),this,IDC_CHAT_MESSAGE);
	SetWindowPos(NULL,0,0,299,159,SWP_NOMOVE);
	m_ChatMsg.SetBackgroundColor(FALSE,RGB(222,219,219));
	m_ChatMsg.SetFont(&g_GlobalOption.m_Font);
	m_ChatMsg.ShowWindow(SW_SHOW);

	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_btQuit.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(288,3,0,0),this,IDB_BT_QUIT_X);
	m_btQuit.LoadRgnImage(hInstance,IDB_BT_QUIT_X,0xff00ff);

	SetTimer(498,200,NULL);
	return true;
}

//显/隐
afx_msg void CDlgChatMsg::OnBnClickedHide()
{
	ShowWindow(SW_HIDE);
}

void CDlgChatMsg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CImageHandle Msg_THandle(&m_Msg_T);
	CImageHandle Msg_BHandle(&m_Msg_B);
	CImageHandle Msg_LHandle(&m_Msg_L);
	CImageHandle Msg_RHandle(&m_Msg_R);

	m_Msg_T.BitBlt(dc,17,0);
	m_Msg_B.BitBlt(dc,17,153);
	m_Msg_L.BitBlt(dc,0,0);
	m_Msg_R.BitBlt(dc,292,0);
}




