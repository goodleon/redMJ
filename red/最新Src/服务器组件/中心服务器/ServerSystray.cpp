#include "StdAfx.h"
#include "AtlBase.h"
#include "ServerSystray.h"

CServerSystray::CServerSystray()
{
	::GetModuleFileName(NULL,m_ExePath,MAX_PATH);
	*strrchr(m_ExePath,'\\')=0;
}

CServerSystray::~CServerSystray()
{
}


CWnd *CServerSystray::GetCenterServer()
{
	return CWnd::FindWindow(TEXT("#32770"),CenterServerTitle);
}

CWnd *CServerSystray::GetLogonServer()
{
	return CWnd::FindWindow(TEXT("#32770"),LogonServerTitle);
}


void CServerSystray::ShowCenterServer()
{
	this->ShowWindow(GetCenterServer()->GetSafeHwnd());
}

void CServerSystray::ShowLogonServer()
{
	if(GetLogonServer())
	{
		this->ShowWindow(GetLogonServer()->GetSafeHwnd());
	}
	else
	{
	CString s;
		s= m_ExePath;
		s += "\\LogonServer.exe";
		ShellExecute(NULL,TEXT("open"),s,NULL,NULL,SW_SHOW);
	}
}


void CServerSystray::HideAll()
{
	GetCenterServer()->ShowWindow(SW_HIDE);
	GetLogonServer()->ShowWindow(SW_HIDE);
	for(ULONG i=0;i<m_GameServerHwnds.size();i++)
	{
		if(::IsWindow(m_GameServerHwnds[i]))
		{
			::ShowWindow(m_GameServerHwnds[i],SW_HIDE);
		}
	}
}

void CServerSystray::ShowAll()
{
	for(ULONG i=0;i<m_GameServerHwnds.size();i++)
	{
		if(::IsWindow(m_GameServerHwnds[i]))
		{
			this->ShowWindow(m_GameServerHwnds[i]);
		}
	}
	ShowCenterServer();
	ShowLogonServer();
}

void CServerSystray::AddIcon_To_Systray(HWND hwnd)
{
	NOTIFYICONDATA tnd = {
		sizeof(NOTIFYICONDATA),
		hwnd,
		IDR_MAINFRAME,//nID
		NIF_ICON | NIF_MESSAGE | NIF_TIP,
		MY_WM_SYSTRAYICON,//uCallbackMessage
		AfxGetApp()->LoadIcon(IDR_MAINFRAME),
		CenterServerTitle};
	Shell_NotifyIcon(NIM_ADD, &tnd);
}

void CServerSystray::DeleteIcon_From_Systray(HWND hwnd)
{
	NOTIFYICONDATA tnd = {
		sizeof(NOTIFYICONDATA),
		hwnd,
		IDR_MAINFRAME,//nID
		NIF_ICON | NIF_MESSAGE | NIF_TIP,
		MY_WM_SYSTRAYICON,//uCallbackMessage
		AfxGetApp()->LoadIcon(IDR_MAINFRAME),
		CenterServerTitle};
	Shell_NotifyIcon(NIM_DELETE, &tnd);
}

void CServerSystray::ShowWindow(HWND hwnd)
{
	::ShowWindow(hwnd,SW_RESTORE);
	::SetActiveWindow(hwnd);
	::BringWindowToTop(hwnd);
	::SetForegroundWindow(hwnd);
}

void CServerSystray::ShowGameServer(WPARAM nMenuId)
{
	HWND hwnd=m_GameServerHwnds[nMenuId - IDM_SHOW_GAMESERVER_IDSTART];
	if(::IsWindow(hwnd))
	{
		this->ShowWindow(hwnd);
	}
}

void CServerSystray::AddGameServerWindow(HWND hwnd)
{
	for(ULONG i=0;i<m_GameServerHwnds.size();i++)
	{
		if(m_GameServerHwnds[i]==hwnd)return;
	}
	m_GameServerHwnds.push_back(hwnd);
}

void CServerSystray::StopAndCloseAllService()
{
	for(ULONG i=0;i<m_GameServerHwnds.size();i++)
	{
		if(::IsWindow(m_GameServerHwnds[i]))
		{
			::SendMessage(m_GameServerHwnds[i],WM_COMMAND,
					MAKEWPARAM(0x3EA,BN_CLICKED),0);//0x3EA->stop

			::SendMessage(m_GameServerHwnds[i],WM_COMMAND,
				MAKEWPARAM(0x2,BN_CLICKED),0);//0x2->exit
		}
	}
	CWnd *w=GetLogonServer();
	if(w)
	{
		::SendMessage(w->GetSafeHwnd(),WM_COMMAND,
			MAKEWPARAM(0x3E9,BN_CLICKED),0);
		::SendMessage(w->GetSafeHwnd(),IDCANCEL,0,0);
	}

}

void CServerSystray::ShowMenu(CWnd *hwnd)
{
CMenu MyMenu;
	CPoint pt;
	GetCursorPos(&pt);

	if (MyMenu.CreatePopupMenu()==FALSE) return;

	MyMenu.AppendMenu(0,0,TEXT("取消(&C)"));
	MyMenu.AppendMenu(0,IDM_SHOW_CENTER_SERVER,"中心服务器设置");
	MyMenu.AppendMenu(0,IDM_SHOW_LOGON_SERVER,"登陆服务器设置");
	MyMenu.AppendMenu(MF_SEPARATOR);

	for(ULONG i=0;i<m_GameServerHwnds.size();i++)
	{
		if(::IsWindow(m_GameServerHwnds[i]))
		{
			char szTitle[128];
			memset(szTitle,0,sizeof(szTitle));
			::GetWindowText(m_GameServerHwnds[i],szTitle,127);
			MyMenu.AppendMenu(0,IDM_SHOW_GAMESERVER_IDSTART + i,szTitle);
		}
		else
		{
			m_GameServerHwnds.erase(m_GameServerHwnds.begin()+i);
			i--;
		}
	}

	MyMenu.AppendMenu(MF_SEPARATOR);
	MyMenu.AppendMenu(0,IDM_SHOW_ALL_SERVER,"全部显示");
	MyMenu.AppendMenu(0,IDM_HIDE_ALL_SERVER,"全部隐藏");
	MyMenu.AppendMenu(0,IDM_EXIT_SERVER,"退出(&X)");

	MyMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,pt.x,pt.y-10,hwnd);
}


//////////////////////////////////////////////////////////////////////////
