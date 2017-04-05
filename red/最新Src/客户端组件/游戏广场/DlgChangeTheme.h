#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "..\..\共享组件\公共服务\HtmlBrowser.h"

class CDlgChangeTheme :	public CSkinDialogEx
{
public:
	CSkinButton							m_btButton1;				//更换主题


	CDlgChangeTheme(void);
	~CDlgChangeTheme(void);

	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);

	afx_msg void OnBnClickedButton1();

	DECLARE_MESSAGE_MAP()
};
