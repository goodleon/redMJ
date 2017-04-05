#ifndef INIR_SERVERSYSTRAY_HEAD_FILE
#define INIR_SERVERSYSTRAY_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include <vector>


#define MY_WM_SYSTRAYICON	(WM_USER+4)		// 系统托盘消息
#define MY_WM_GAMESERVER_LOAD	(WM_USER+5)		// 系统托盘消息


#define IDM_SHOW_CENTER_SERVER  10000			// 显示中心服务器
#define IDM_SHOW_LOGON_SERVER   10001			// 显示登陆服务器
#define IDM_SHOW_ALL_SERVER	    10002			// 全部显示
#define IDM_HIDE_ALL_SERVER	    10003			// 全部隐藏
#define IDM_EXIT_SERVER	        10004			// 全部隐藏

#define IDM_SHOW_GAMESERVER_IDSTART	10100		// 游戏服务器点击事件开始的ID

#define MAX_GAMESERVER_COUNT		100		// 每台服务器就多可运行的游戏服务器数

//////////////////////////////////////////////////////////////////////////

//配置参数
class CServerSystray
{
	//网络设置
public:
	CServerSystray();
	~CServerSystray();
	
	CWnd *GetCenterServer();
	CWnd *GetLogonServer();

	void ShowCenterServer();
	void ShowLogonServer();
	void ShowGameServer(WPARAM nMenuId);

	void HideAll();
	void ShowAll();
	void StopAndCloseAllService();

	void AddIcon_To_Systray(HWND hwnd);
	void DeleteIcon_From_Systray(HWND hwnd);

	void AddGameServerWindow(HWND hwnd);

	void ShowMenu(CWnd *hwnd);

private:
	void ShowWindow(HWND hwnd);

	TCHAR m_ExePath[MAX_PATH];
	std::vector<HWND>					m_GameServerHwnds;
};

//////////////////////////////////////////////////////////////////////////

#endif//INIR_SERVERSYSTRAY_HEAD_FILE
