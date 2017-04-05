#pragma once

#ifndef IMAGE_DIALOG_HEAD_FILE
#define IMAGE_DIALOG_HEAD_FILE

#include "SkinControls.h"

//////////////////////////////////////////////////////////////////////////

//对话框类
class SKIN_CONTROL_CLASS CImageDialog : public CDialog
{
	//函数定义
public:
	//构造函数
	CImageDialog(UINT nIDTemplate, CWnd * pParentWnd=NULL);
	//析构函数
	virtual ~CImageDialog();

	//重载函数
public:
	//初始化消息
	virtual BOOL OnInitDialog();

protected:
	//画背景
	void DrawSkinView(CDC *pDC);


	//消息映射
protected:
	//绘画消息
	afx_msg void OnPaint();
	//绘画背景
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//控件颜色
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CImageDialog)
};

//////////////////////////////////////////////////////////////////////////

#endif