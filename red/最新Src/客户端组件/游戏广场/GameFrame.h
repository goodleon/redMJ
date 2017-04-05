#ifndef GAME_FRAME_HEAD_FILE
#define GAME_FRAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "DlgControlBar.h"
#include "PlazaViewItem.h"

/////////////////////////////////////////////////////////////////////////////


//主框架类
class CGameFrame : public CFrameWnd
{
	//控件变量
public:
	CSkinButton							m_btMin;						//最小按钮
	CSkinButton							m_btClose;						//关闭按钮
	bool								m_bMaxed;						//最大化标志
	bool								m_bMaxedC;						//是否点过最大化标志
	CSkinButton							m_btMax;						//最大化按钮
	CToolTipCtrl						m_ToolTipCtrl;					//提示控件
	CPlazaViewItem						m_DlgGamePlaza;					//游戏广场
	CDlgControlBar						m_DlgControlBar;				//控制条类

	//CHtmlBrower							*m_pHtmlStatus;					//状态栏滚动

	//导航按钮
public:
	CSkinButton							m_btButton1;					//功能按钮
	CSkinButton							m_btButton2;					//功能按钮
	CSkinButton							m_btButton3;					//功能按钮
	CSkinButton							m_btButton4;					//功能按钮
	CSkinButton							m_btButton5;					//功能按钮
	CHtmlBrower							*m_pHtmlBrower1;				//浏览窗口

	//资源变量
protected:
	CSkinImage							m_ImageTL;						//左上标题
	CSkinImage							m_ImageTM;						//中上标题
	CSkinImage							m_ImageTR;						//右上标题

//	CSkinImage							m_StatusL;						//状态栏
//	CSkinImage							m_StatusM;						//状态栏
	//函数定义	
public:
	//构造函数
	CGameFrame();
	//析构函数
	virtual ~CGameFrame();

	//重载函数
public:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//消息函数
protected:
	//绘画消息
	afx_msg void OnPaint();
	//绘画背景
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//创建消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//大小消息
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR * lpMMI);
	//位置消息
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//最小按钮
	afx_msg void OnBnClickedMin();
	//最大化按钮
	afx_msg void OnBnClickedMax();
	//关闭按钮
	afx_msg void OnBnClickedClose();
	//改变消息
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//关闭消息
	afx_msg void OnClose();

	//导航按钮
protected:
	//功能按钮
	afx_msg void OnBnClickedButton1();
	//功能按钮
	afx_msg void OnBnClickedButton2();
	//功能按钮
	afx_msg void OnBnClickedButton3();
	//功能按钮
	afx_msg void OnBnClickedButton4();
	//功能按钮
	afx_msg void OnBnClickedButton5();

	//热键
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);

	//游戏进程发窗口句柄给大厅
	afx_msg LRESULT OnGameClient(WPARAM wParam, LPARAM lParam);

	//自定义消息
protected:
	//安装完成
	LRESULT OnMessageSetupFinish(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

/////////////////////////////////////////////////////////////////////////////

#endif
