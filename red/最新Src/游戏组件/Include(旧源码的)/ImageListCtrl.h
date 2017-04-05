#if !defined(AFX_IMAGELISTCTRL_H__B382B86C_A9B6_4F61_A03D_53C27C76DF9E__INCLUDED_)
#define AFX_IMAGELISTCTRL_H__B382B86C_A9B6_4F61_A03D_53C27C76DF9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageListCtrl.h : header file
//

#include <vector>


#include "SkinControls.h"
#include "SkinImage.h"

#define IMG_LISTCTRL_CMD_ID (31234)
#define IMG_L_CLICK_BUTTON	1
#define IMG_R_CLICK_BUTTON	2

#ifndef ColorMapR
#define ColorMapR(color) (*((BYTE*)&color))
#define ColorMapG(color) (*((BYTE*)&color+1))
#define ColorMapB(color) (*((BYTE*)&color+2))
#endif
/////////////////////////////////////////////////////////////////////////////
// ImageListCtrl window

struct tagImageListButton
{
	CSkinImage img;
	DWORD dwCmdId;
	ULONG dwItem;
	DWORD UserData;
	int TitleX,TitleY;
	COLORREF m_Color;
	TCHAR Title[128];
};


class SKIN_CONTROL_CLASS CImageListCtrl : public CStatic
{
// Construction
public:
	CImageListCtrl();
	virtual ~CImageListCtrl();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ImageListCtrl)
	//}}AFX_VIRTUAL

// Implementation
private:
	RECT m_ScrollTop_Rc;//滚动条上箭头的位置
	RECT m_ScrollBottom_Rc;//滚动条下箭头的位置
	RECT m_ScrollMiddle_Rc;//滚动条中间部份的位置
	RECT m_Body_Rc;//列表的位置

	long m_MinThumbPos,m_MaxThumbPos;
	long m_ThumbPos;//Thumb当前的位置
	long m_nOldMousePos;//0表示鼠标没按下,非零表示鼠标到 Thumb->top 的象素
	long m_nButtonHeight;//每个按钮的高度
	long m_nThumbHeight;//滚动按钮的高度

	ULONG m_nItem_tmp,m_nLastItem;//上次左键点击的按钮
	ULONG m_nItem_tmpRT,m_nLastItemRT;//上次右键点击的按钮

	//画按钮的句柄
private:
	HDC  m_BtnDC; 
	HBITMAP  m_BtnBitmap;

public:
	CSkinImage m_xTop;
	CSkinImage m_xLeft;
	CSkinImage m_xRight;
	CSkinImage m_xBottom;
	CSkinImage m_xScrollT;
	CSkinImage m_xScrollM;
	CSkinImage m_xScrollB;
	CSkinImage m_xThumb;

	HFONT m_hFont;

	std::vector<tagImageListButton*> m_xButtons;

	void SetHeight(ULONG nHeight);
	void SetThumbPos(long nPos,bool bRedraw);
	long GetThumbPos();//Thumb的位置
	long GetScrollPixel();//计算滚动的高度
	ULONG AddButton(HINSTANCE hInst,UINT nResourceId,DWORD dwCommandId=IMG_LISTCTRL_CMD_ID,LPCSTR pszTitle=NULL,int nTitleX=0,int nTitleY=0,COLORREF nTitleColor=0,DWORD dwUserData=0);//返回新按钮

	ULONG GetLastButton();
	ULONG GetLastButtonRT();
	ULONG GetButtonByTitle(LPCSTR szTitle);

	long GetButtonIndex(ULONG nItem);
	tagImageListButton *GetButtonInfo(ULONG nItem);

	void DeleteButton(ULONG nItem);
	void ClearButton();

	void UpdateButton();//在增加和删除后要马上调用
public:
	void ScrollUp();
	void ScrollDown();
	int GetWidth();
	int GetHeight();

	//在按钮上加文字
	void DrawText(ULONG nItem,int x,int y,LPCSTR szStr,COLORREF nColor=0);
private:
	bool IsPtInThumb(const POINT &pt);
	void EventMouse(UINT nFlags,const POINT &pt);
	ULONG ButtonIndex_FromPoint(POINT &point);

	// Generated message map functions
protected:
	//{{AFX_MSG(ImageListCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CImageListCtrl)
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGELISTCTRL_H__B382B86C_A9B6_4F61_A03D_53C27C76DF9E__INCLUDED_)
/*

.h->CImageListCtrl *m_RoomList;
.h->afx_msg void OnBnRoomList_Clicked()

BEGIN_MESSAGE_MAP(CMyListBoxDlg, CDialog)
	ON_BN_CLICKED(IMG_LISTCTRL_CMD_ID,OnBnRoomList_Clicked)
END_MESSAGE_MAP()

BOOL OnInitDialog()
{
	if(m_RoomList)delete m_RoomList;
	m_RoomList = new CImageListCtrl();
	if(m_RoomList->Create(NULL, WS_CHILD|SS_LEFT|SS_NOTIFY,CRect(0,0,0,0), this))
	{
		m_RoomList->m_xTop->LoadResource(FindResource(NULL,"ROOMLIST_T",RT_BITMAP),CXIMAGE_FORMAT_BMP);
		m_RoomList->m_xLeft->LoadResource(FindResource(NULL,"ROOMLIST_L",RT_BITMAP),CXIMAGE_FORMAT_BMP);
		m_RoomList->m_xRight->LoadResource(FindResource(NULL,"ROOMLIST_R",RT_BITMAP),CXIMAGE_FORMAT_BMP);
		m_RoomList->m_xBottom->LoadResource(FindResource(NULL,"ROOMLIST_B",RT_BITMAP),CXIMAGE_FORMAT_BMP);
		m_RoomList->m_xScrollT->LoadResource(FindResource(NULL,"ROOMLIST_S_T",RT_BITMAP),CXIMAGE_FORMAT_BMP);
		m_RoomList->m_xScrollM->LoadResource(FindResource(NULL,"ROOMLIST_S_M",RT_BITMAP),CXIMAGE_FORMAT_BMP);
		m_RoomList->m_xScrollB->LoadResource(FindResource(NULL,"ROOMLIST_S_B",RT_BITMAP),CXIMAGE_FORMAT_BMP);
		m_RoomList->m_xThumb->LoadResource(FindResource(NULL,"ROOMLIST_S_THUMB",RT_BITMAP),CXIMAGE_FORMAT_BMP);
		m_RoomList->SetHeight(350);
		
		//for(int i=100001;i<=100021;i++)
		//	m_RoomList->AddButton(FindResource(NULL,"ROOMLIST_BTN",RT_BITMAP),IMG_LISTCTRL_CMD_ID,"诈金花房间1",20,5,0xffffff,i);
		m_RoomList->UpdateButton();
	}
}



//bluker:游戏种类图标按钮[点击事件]
afx_msg void CMyListBoxDlg::OnBnRoomList_Clicked()
{
ULONG nButton;
tagImageListButton *BtnInfo;
	nButton=m_RoomList->GetLastButton();
	if(!nButton)return;
	BtnInfo=m_RoomList->GetButtonInfo(nButton);
	if(!BtnInfo)return;

	if(BtnInfo->UserData>0)
	{
		g_pControlBar->CreateRoomViewItem(BtnInfo->UserData & 0xffff,BtnInfo->UserData>>16);
		m_RoomList->ShowWindow(SW_HIDE);
	}
}



*/