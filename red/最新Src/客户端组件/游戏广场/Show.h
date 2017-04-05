#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "Afxcmn.h"

//发送短信息类
class CShow : public CSkinDialogEx
{
	//变量定义
public:
	CWnd								* s_pParentWnd;						//处理窗口
	
	//信息变量
	long int							m_dwTargetUserID;					//对话用户
	__int64								m_dwTargerAccID;					//ACC 号码
	CString								m_strTargerName;					//用户名字
	CString								m_strRoomName;						//房间名字
	IUserItem							* m_pTargetUserItem;				//目标用户
	

	//控件变量
public:
	
	CSkinButton							m_BtClose;							//退出按钮
	CMessageProxyHelper					m_MessageProxyHelper;				//消息代理

	DECLARE_MESSAGE_MAP()
	//函数定义
public:
	//构造函数
	CShow(CWnd * pParentWnd);
	//析构函数
	virtual ~CShow();
	//初始化函数
	virtual BOOL OnInitDialog();
	//重载函数
protected:
	//控件绑定函数
	virtual void DoDataExchange(CDataExchange * pDX); 
	
	//消息解释函数
	//virtual BOOL PreTranslateMessage(MSG * pMsg);
	//功能函数 

public:
	
	//退出按钮
	afx_msg void OnBnClickedClose();
	//获取位置
	afx_msg void OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	
};
