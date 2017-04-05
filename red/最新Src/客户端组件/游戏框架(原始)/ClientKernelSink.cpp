#include "StdAfx.h"
#include "GameFrameDlg.h"
#include "ClientKernelSink.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CClientKernelSink::CClientKernelSink(CGameFrameDlg & GameFrameDlg) : m_GameFrameDlg(GameFrameDlg)
{
}

//析构函数
CClientKernelSink::~CClientKernelSink()
{
}

//接口查询
void * __cdecl CClientKernelSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IClientKernelSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IClientKernelSink,Guid,dwQueryVer);
	return NULL;
}

//获取窗口
HWND __cdecl CClientKernelSink::GetFrameWnd()
{
	return m_GameFrameDlg.m_hWnd;
}

//重置状态
void __cdecl CClientKernelSink::ResetGameFrame()
{
	//重置界面
	m_GameFrameDlg.m_pGameFrameView->ResetData();
	m_GameFrameDlg.m_pGameFrameView->ResetGameView();
	m_GameFrameDlg.ResetGameFrame();

	return;
}

//关闭窗口
void __cdecl CClientKernelSink::CloseGameFrame(bool bForceClose)
{
	if (m_GameFrameDlg.m_hWnd!=NULL) m_GameFrameDlg.PostMessage(WM_CLOSE,0,0);
	return;
}

//时间消息
bool __cdecl CClientKernelSink::OnEventTimer(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	//变量界面
	WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
	m_GameFrameDlg.m_pGameFrameView->SetUserTimer(wViewChairID,nElapse);

	//事件通知
	return m_GameFrameDlg.OnTimerMessage(wChairID,nElapse,nTimerID);
}

//网络消息
bool __cdecl CClientKernelSink::OnEventSocket(const CMD_Command & Command, const void * pBuffer, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GF_FRAME:	//框架消息
		{
			return m_GameFrameDlg.OnFrameMessage(Command.wSubCmdID,pBuffer,wDataSize);
		}
	case MDM_GF_GAME:	//游戏消息
		{
			return m_GameFrameDlg.OnGameMessage(Command.wSubCmdID,pBuffer,wDataSize);
		}
	}

	return false;
}

//进程消息
bool __cdecl CClientKernelSink::OnEventProcess(const IPC_Head & Head, const void * pBuffer, WORD wDataSize, HWND hWndSend)
{
	return false;
}

//游戏状态
bool __cdecl CClientKernelSink::OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	return m_GameFrameDlg.OnGameSceneMessage(cbGameStation,bLookonOther,pBuffer,wDataSize);
}

//聊天信息
bool __cdecl CClientKernelSink::OnUserChatMessage(tagUserData * pSendUserData, tagUserData * pRecvUserData, LPCTSTR pszChatMessage, COLORREF crColor)
{
	if(strncmp(VOICE_SIGN,pszChatMessage,lstrlen(VOICE_SIGN))==0)
	{
		CGameFrameDlg *dlg = (CGameFrameDlg*)AfxGetMainWnd();
		char *p = (char*)pszChatMessage + lstrlen(VOICE_SIGN);
		char *pszWavFile = strchr(p,':');
		if(!pszWavFile)return true;
		*pszWavFile=0;
		pszWavFile++;
		WORD wToWho = (WORD)atol(p);

		//自已发给所有人的语音
		if(pSendUserData->wChairID == dlg->GetMeChairID() && wToWho==0)
			return true;

		if(wToWho!=0)
		{
			//是跟别人讲。不是和我讲
			if(wToWho!= dlg->GetMeChairID())
				return true;
			//这是和我讲的
			dlg->PlayGameSoundFromFip(pszWavFile);
		}
		else
		{
			//这是和我们大家讲的
			dlg->PlayGameSoundFromFip(pszWavFile);
		}
		return true;
	}
	else
	{
		//显示消息
		if (pRecvUserData==NULL)
			m_GameFrameDlg.m_MessageProxyHelper->InsertUserChat(pSendUserData->szName,pszChatMessage,crColor,MS_NORMAL);
		else
			m_GameFrameDlg.m_MessageProxyHelper->InsertUserChat(pSendUserData->szName,pRecvUserData->szName,pszChatMessage,crColor,MS_NORMAL);

		ShowChat.ShowChat(pszChatMessage,pSendUserData);
		m_GameFrameDlg.m_pGameFrameView->UpdateGameView(NULL);
	}

	return true;
}

//用户进入
void __cdecl CClientKernelSink::OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	m_GameFrameDlg.m_wKindID=m_GameFrameDlg.m_ClientKernelHelper->GetServerAttribute()->wKindID;
	m_GameFrameDlg.m_wGameGenre=m_GameFrameDlg.m_ClientKernelHelper->GetServerAttribute()->wGameGenre;

	int nBtn=m_GameFrameDlg.m_DlgUserList->m_ListUsers.AddButton(GetModuleHandle(GAME_FRAME_DLL_NAME),
					USERLIST_BTN,IMG_LISTCTRL_CMD_ID,
					pUserData->szName,
					0,0,0x505050,0);
	m_GameFrameDlg.m_DlgUserList->m_ListUsers.UpdateButton();
	if (bLookonUser==false)
	{
		WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
		m_GameFrameDlg.m_pGameFrameView->SetUserInfo(wViewChairID,pUserData);
	}
	//m_GameFrameDlg.m_pGameFrameView->SetUserInfo(wViewChairID,pUserData);
	return;
}

//用户离开
void __cdecl CClientKernelSink::OnEventUserLeft(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	int nItem=m_GameFrameDlg.m_DlgUserList->m_ListUsers.GetButtonByTitle(pUserData->szName);
	if(nItem)m_GameFrameDlg.m_DlgUserList->m_ListUsers.DeleteButton(nItem);
	m_GameFrameDlg.m_DlgUserList->m_ListUsers.UpdateButton();
	if (bLookonUser==false)
	{
		WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
		m_GameFrameDlg.m_pGameFrameView->SetUserInfo(wViewChairID,NULL);
	}
	m_GameFrameDlg.m_MessageProxyHelper->InsertUserName(pUserData->szName);
	m_GameFrameDlg.m_MessageProxyHelper->InsertGeneralString(TEXT("离开了"),0,0,true);
	return;
}

//用户游戏币
void __cdecl CClientKernelSink::OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//设置界面
	//m_GameFrameDlg.m_pGameFrameControl->UpdateUserItem(pUserData);
	//if (bLookonUser==false) m_GameFrameDlg.m_pGameFrameView->UpdateGameView(NULL);

	return;
}

//用户状态
void __cdecl CClientKernelSink::OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//设置界面
	//m_GameFrameDlg.m_pGameFrameControl->UpdateUserItem(pUserData);
	//if (bLookonUser==false)
		m_GameFrameDlg.m_pGameFrameView->UpdateGameView(NULL);

	return;
}

//时间更新
void __cdecl CClientKernelSink::OnEventTimerKilled(WORD wChairID, UINT nTimerID)
{
	//设置界面
	WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
	m_GameFrameDlg.m_pGameFrameView->SetUserTimer(wViewChairID,0);

	return;
}

//旁观状态
void __cdecl CClientKernelSink::OnEventLookonChanged(bool bLookonUser, void * pBuffer, WORD wDataSize)
{
	return;
}

//////////////////////////////////////////////////////////////////////////
