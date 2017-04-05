#ifndef DLG_TOOLBARDLG_HEAD_FILE
#define DLG_TOOLBARDLG_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

#include "GameFrame.h"
#include "GlobalOption.h"
#include "..\..\共享组件\界面控件\SkinControls.h"


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

class CDialogIX : public CDialog
{
protected:
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};


//工具栏
class CDlgToolBar : public CDialogIX
{
public:
	//构造函数
	CDlgToolBar();
	//析构函数
	virtual ~CDlgToolBar();

private:
	CSkinImage					m_MainBg;//背景图片
	
	CRgnButton					*m_btChatSend; //发送按钮
	CRgnButton					*m_btChatDict; //常用语句按钮
	CRgnButton					*m_btShowChat; //显/隐 消息框
	CRgnButton					*m_btShowUserList; //显/隐用户框
	CEdit						*m_ChatEdit;	//聊天输入框

	DWORD						m_dwLastChat;
public:
	IClientKernel						*m_pICK;//内核接口


public:
	afx_msg void OnBnClickedShowChat();//显/隐 消息框
	afx_msg void OnBnClickedShowUserList();//显/隐用户框

	//重载函数
protected:
	//初始化函数
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////
//用户列表框
class CDlgUserList : public CDialogIX
{
public:
	//构造函数
	CDlgUserList();
	//析构函数
	virtual ~CDlgUserList();

	CImageListCtrl					m_ListUsers;				//用户列表

	CRgnButton						m_btQuit; //隐藏


	//用于Timer
private:
	DWORD m_dwTimes;

	//重载函数
protected:
	//初始化函数
	virtual BOOL OnInitDialog();
	virtual void OnOK(){return;}
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedHide();//隐框
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};
//////////////////////////////////////////////////////////////////////////

//消息窗口
class CDlgChatMsg : public CDialogIX
{
public:
	//构造函数
	CDlgChatMsg();
	//析构函数
	virtual ~CDlgChatMsg();
	void Show();


	CSkinImage							m_Msg_T;
	CSkinImage							m_Msg_B;
	CSkinImage							m_Msg_L;
	CSkinImage							m_Msg_R;

	CSkinRichEdit						m_ChatMsg;;				//聊天组件

	CRgnButton							m_btQuit; //隐藏

	//用于Timer
private:
	DWORD m_dwTimes;

	//重载函数
protected:
	//初始化函数
	virtual BOOL OnInitDialog();
	virtual void OnOK(){return;}
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedHide();//隐框

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};
//////////////////////////////////////////////////////////////////////////

/*
	CRect rc;
	GetWindowRect(rc);

	m_ToolBar->SetWindowPos(NULL,0,0,397,25,SWP_NOMOVE);
	m_ToolBar->SetWindowPos(NULL,rc.left + 7,rc.bottom - 34,0,0,SWP_NOSIZE);

	m_ChatMsgDlg->SetWindowPos(NULL,0,0,299,159,SWP_NOMOVE);
	m_ChatMsgDlg->SetWindowPos(NULL,rc.left+7,rc.bottom - 195,0,0,SWP_NOSIZE);

	m_DlgUserList->SetWindowPos(NULL,0,0,m_DlgUserList->m_ListUsers.GetWidth(),159,SWP_NOMOVE);
	m_DlgUserList->SetWindowPos(NULL,rc.left+307,rc.bottom - 195,0,0,SWP_NOSIZE);

*/

#endif