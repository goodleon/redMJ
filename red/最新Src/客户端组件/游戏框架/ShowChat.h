#pragma once

#include "ximage/ximage.h"

#define MAX_PLAYER      6


#define SCP_T            1
#define SCP_TL           2
#define SCP_L            3
#define SCP_BL           4
#define SCP_B            5
#define SCP_BR           6
#define SCP_R            7
#define SCP_TR           8



#define GFDlg ((CGameFrameDlg*)AfxGetMainWnd())

class CShowChat;
extern CShowChat ShowChat;
LPPOINT GAME_FRAME_CLASS GetChatPt();

// CShowChat

class CShowChat
{
public:
	//CxImage			m_ChatImg_T;	//聊天背景
	//CxImage			m_ChatImg_B;	//聊天背景
	//CxImage			m_ChatImg_L;	//聊天背景
	//CxImage			m_ChatImg_R;	//聊天背景
	//CxImage			m_ChatImg_BL;	//聊天背景
	//CxImage			m_ChatImg_BR;	//聊天背景
	CSkinImage			m_ChatImg_T;	//聊天背景
	CSkinImage			m_ChatImg_B;	//聊天背景
	CSkinImage			m_ChatImg_L;	//聊天背景
	CSkinImage			m_ChatImg_R;	//聊天背景
	CSkinImage			m_ChatImg_BL;	//聊天背景
	CSkinImage			m_ChatImg_BR;	//聊天背景

	CString			m_szMsg[MAX_PLAYER];		//聊天内容
	int				m_lStartTick[MAX_PLAYER];	//开始的显示时间
	int				m_lTime[MAX_PLAYER];		//显示时间

	POINT			PosPt[8];//8个方位的坐标

public:
	CShowChat();
	~CShowChat();
	//使用方法
	//CxImage *GetChairPos(WORD wViewChair,int &x,int &y,int &Word_x,int &Word_y);
	CSkinImage *GetChairPos(WORD wViewChair,int &x,int &y,int &Word_x,int &Word_y);

	bool ShowChat(LPCTSTR szString,tagUserData *pUser,int lTime=3000);


	void OnDraw(CDC *pDC);//要放在相应的OnPaint事件中
};


