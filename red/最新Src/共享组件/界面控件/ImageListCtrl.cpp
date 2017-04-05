// ImageListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ImageListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CImageListCtrl message handlers

IMPLEMENT_DYNAMIC(CImageListCtrl, CStatic)

BEGIN_MESSAGE_MAP(CImageListCtrl, CStatic)
	//{{AFX_MSG_MAP(CImageListCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

static ULONG g_ItemIndex=1000;

/////////////////////////////////////////////////////////////////////////////
// CImageListCtrl

CImageListCtrl::CImageListCtrl()
{
	memset(&m_ScrollTop_Rc,0,sizeof(m_ScrollTop_Rc));
	memset(&m_ScrollBottom_Rc,0,sizeof(m_ScrollBottom_Rc));
	memset(&m_ScrollMiddle_Rc,0,sizeof(m_ScrollMiddle_Rc));
	m_MinThumbPos=0;
	m_MaxThumbPos=0;
	SetThumbPos(0,false);
	m_nOldMousePos=0;
	m_BtnDC  = NULL;
	m_BtnBitmap=NULL;
	m_nItem_tmp=0;
	m_nLastItem=0;
	m_nLastItemRT = 0;
	m_nThumbHeight = 0;
	m_hFont=CreateFont(-12,NULL,0,0,1,0,0,0,GB2312_CHARSET,0,0,0,DEFAULT_PITCH,"宋体");

}

CImageListCtrl::~CImageListCtrl()
{
	ClearButton();

	if(m_BtnBitmap)
	{
		DeleteObject(SelectObject(m_BtnDC,m_BtnBitmap));
		m_BtnBitmap=NULL;
	}

	if(m_BtnDC)
	{
		::DeleteDC(m_BtnDC);
		m_BtnDC=NULL;
	}

	if(m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont=NULL;
	}

}

//在增加和删除后要马上调用
void CImageListCtrl::UpdateButton() 
{
	if(m_BtnBitmap)
	{
		DeleteObject(SelectObject(m_BtnDC,m_BtnBitmap));
		m_BtnBitmap=NULL;
	}
	if(m_BtnDC)
	{
		::DeleteDC(m_BtnDC);
		m_BtnDC=NULL;
	}

	ULONG hdcWidth = (m_Body_Rc.right-m_Body_Rc.left)*2;
	ULONG hdcHeight = (int)m_xButtons.size()*m_nButtonHeight+100;
	if(hdcHeight<1000)hdcHeight=1000;

	HDC hdc=::GetWindowDC(this->m_hWnd);
	m_BtnDC  =  ::CreateCompatibleDC(hdc);//创建设备上下文(HDC)
	m_BtnBitmap=  (HBITMAP)::SelectObject(m_BtnDC,::CreateCompatibleBitmap(hdc,
										hdcWidth,
										hdcHeight));

	::ReleaseDC(this->m_hWnd,hdc);


	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	ULONG nBtnIndex_Mouse = ButtonIndex_FromPoint(pt);

	HFONT hOldFont=(HFONT)SelectObject(m_BtnDC,m_hFont);
	SetBkMode(m_BtnDC,TRANSPARENT);
	for(ULONG i=0;i<m_xButtons.size();i++)
	{
		CImageHandle ImgHandle(&(m_xButtons[i]->img));
		if(i==0)
		{
			FillRect(m_BtnDC,&CRect(
						0,
						0,
						hdcWidth,
						hdcHeight
						),CBrush((COLORREF)m_xButtons[i]->img.GetPixel(0,0)));
		}
		ULONG Y = (m_nButtonHeight) * i;

		m_xButtons[i]->img.Draw(m_BtnDC,0,Y);

		if(nBtnIndex_Mouse == i && PtInRect(&m_Body_Rc,pt))
		{
			//MouseOver事件
			/*
			for(int j=0;j<m_xButtons[i]->img.GetWidth();j++)
			{
				for(int k=0;k<m_xButtons[i]->img.GetHeight();k++)
				{
					if(j<4 || j>m_xButtons[i]->img.GetWidth()-4 ||
						k<4 || k>m_xButtons[i]->img.GetHeight()-4)
					{
						COLORREF c=::GetPixel(m_BtnDC,j,Y + k);
						BYTE r = (ColorMapR(c) > 127) ? ColorMapR(c)-50 : ColorMapR(c) + 50;
						BYTE g = (ColorMapG(c) > 127) ? ColorMapG(c)-50 : ColorMapG(c) + 50;
						BYTE b = (ColorMapB(c) > 127) ? ColorMapB(c)-50 : ColorMapB(c) + 50;
						::SetPixel(m_BtnDC,j,Y + k,RGB(r,g,b));
					}
				}
			}
			*/
			CPen pen(PS_SOLID,1,RGB(255,255,255));
			HPEN penOld=(HPEN)::SelectObject(m_BtnDC,pen);

			int W = m_xButtons[i]->img.GetWidth() -1;
			int H = m_xButtons[i]->img.GetHeight() -1;
			::MoveToEx(m_BtnDC,0,Y,NULL);
			::LineTo(m_BtnDC,W,Y);
			::LineTo(m_BtnDC,W,Y + H);
			::LineTo(m_BtnDC,0,Y + H);
			::LineTo(m_BtnDC,0,Y);

			SelectObject(m_BtnDC,penOld);
			//m_xButtons[i]->img.Draw(m_BtnDC,
			//			CRect(0,Y,m_xButtons[i]->img.GetWidth()/2,Y + m_xButtons[i]->img.GetHeight()),
			//			CRect(m_xButtons[i]->img.GetWidth()/2,0,m_xButtons[i]->img.GetWidth(),m_xButtons[i]->img.GetHeight())
			//			);
		}
		if(m_xButtons[i]->Title[0]!=0)
		{
			SetTextColor(m_BtnDC,m_xButtons[i]->m_Color);
			TextOut(m_BtnDC,m_xButtons[i]->TitleX,Y+m_xButtons[i]->TitleY,m_xButtons[i]->Title,(int)strlen(m_xButtons[i]->Title));
		}
	}
	m_hFont=(HFONT)SelectObject(m_BtnDC,hOldFont);

	InvalidateRect(&m_Body_Rc,false);
}

void CImageListCtrl::DeleteButton(ULONG nItem)
{
	for(ULONG i=0;i<m_xButtons.size();i++)
	{
		if(m_xButtons[i]->dwItem == nItem)
		{
			SafeDelete(m_xButtons[i]);
			m_xButtons.erase(m_xButtons.begin() + i);
			return;
		}
	}
}

void CImageListCtrl::ClearButton()
{
	for(ULONG i=0;i<m_xButtons.size();i++)
	{
		SafeDelete(m_xButtons[i]);
	}
	m_xButtons.clear();
}

int CImageListCtrl::GetWidth()
{
RECT rc;
	GetClientRect(&rc);
	return rc.right - rc.left;
}

int CImageListCtrl::GetHeight()
{
RECT rc;
	GetClientRect(&rc);
	return rc.bottom - rc.top;
}

ULONG CImageListCtrl::ButtonIndex_FromPoint(POINT &point)
{
	if(m_nButtonHeight<1)return 0;
	int Ypos=point.y - m_ScrollTop_Rc.top;
	Ypos += GetScrollPixel();
	return Ypos / (m_nButtonHeight);
}

void CImageListCtrl::DrawText(ULONG nItem,int x,int y,LPCSTR szStr,COLORREF nColor)
{
	tagImageListButton *pBtn=GetButtonInfo(nItem);
	if(!pBtn)return;

	CImageHandle Btn_Handle(&(pBtn->img));
	HDC hdc=pBtn->img.GetDC();

	HFONT hOldFont=(HFONT)SelectObject(hdc,m_hFont);
	SetBkMode(hdc,TRANSPARENT);
	SetTextColor(hdc,nColor);
	::TextOut(hdc,x,y,szStr,(int)strlen(szStr));
	m_hFont=(HFONT)SelectObject(hdc,hOldFont);

	pBtn->img.ReleaseDC();
}

ULONG CImageListCtrl::AddButton(HINSTANCE hInst,UINT nResourceId,DWORD dwCommandId,LPCSTR pszTitle,int nTitleX,int nTitleY,COLORREF nTitleColor,DWORD dwUserData)
{
tagImageListButton *Btn=new tagImageListButton();

	Btn->img.SetLoadInfo(nResourceId,hInst,true);
	CImageHandle Btn_Handle(&(Btn->img));

	m_nButtonHeight = Btn->img.GetHeight();
	Btn->dwItem = (g_ItemIndex++);
	Btn->dwCmdId = dwCommandId;
	Btn->UserData=dwUserData;
	Btn->TitleX=nTitleX;
	Btn->TitleY=nTitleY;
	Btn->m_Color=nTitleColor;


	memset(Btn->Title,0,sizeof(Btn->Title));
	if(pszTitle)
		strncpy(Btn->Title,pszTitle,sizeof(Btn->Title)-1);

	if(g_ItemIndex>99999999)g_ItemIndex=1;

	m_xButtons.push_back(Btn);
	return Btn->dwItem;
}

BOOL CImageListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return true;
	//return CStatic::OnEraseBkgnd(pDC);
}

void CImageListCtrl::SetThumbPos(long nPos,bool bRedraw)
{
long nTmpPos=m_ThumbPos;
	m_ThumbPos=nPos;
	if(m_ThumbPos > m_MaxThumbPos)m_ThumbPos=m_MaxThumbPos;
	if(m_ThumbPos < m_MinThumbPos)m_ThumbPos=m_MinThumbPos;
	if(bRedraw && nTmpPos!=m_ThumbPos && IsWindow(GetSafeHwnd()))
		InvalidateRect(NULL,true);
}

long CImageListCtrl::GetThumbPos()
{
	return m_ThumbPos;
}

ULONG CImageListCtrl::GetLastButton()
{
	return m_nLastItem;
}

ULONG CImageListCtrl::GetLastButtonRT()
{
	return m_nLastItemRT;
}

ULONG CImageListCtrl::GetButtonByTitle(LPCSTR szTitle)
{
	for(ULONG i=0;i<m_xButtons.size();i++)
	{
		if(strcmp(m_xButtons[i]->Title,szTitle)==0)return m_xButtons[i]->dwItem;
	}
	return NULL;
}

long CImageListCtrl::GetButtonIndex(ULONG nItem)
{
	for(ULONG i=0;i<m_xButtons.size();i++)
	{
		if(m_xButtons[i]->dwItem == nItem)return i;
	}
	return -1;
}

tagImageListButton *CImageListCtrl::GetButtonInfo(ULONG nItem)
{
	for(ULONG i=0;i<m_xButtons.size();i++)
	{
		if(m_xButtons[i]->dwItem == nItem)return m_xButtons[i];
	}
	return NULL;
}

//计算滚动的高度
long CImageListCtrl::GetScrollPixel()
{
	if(m_xButtons.size()<1)return 0;
	int n_Total=m_ScrollMiddle_Rc.bottom - m_ScrollMiddle_Rc.top - m_nThumbHeight;
	int n_CurPos=m_ThumbPos - m_ScrollMiddle_Rc.top;
	int n_TotalScrollPixel=(((int)m_xButtons.size()) * m_nButtonHeight) - (m_Body_Rc.bottom - m_Body_Rc.top);

	if(n_TotalScrollPixel<0)n_TotalScrollPixel=0;
	if(n_CurPos<0)n_CurPos=0;
	return n_TotalScrollPixel * n_CurPos/n_Total;
}

void CImageListCtrl::ScrollUp()
{
	int n_Total=m_ScrollMiddle_Rc.bottom - m_ScrollMiddle_Rc.top - m_nThumbHeight;
	int n_TotalScrollPixel=(((int)m_xButtons.size()) * m_nButtonHeight) - (m_ScrollMiddle_Rc.bottom - m_ScrollMiddle_Rc.top);

	int Y=m_nButtonHeight*n_Total/n_TotalScrollPixel;
	if(Y<1)Y=1;
	SetThumbPos(m_ThumbPos - Y,true);
}

void CImageListCtrl::ScrollDown()
{
	int n_Total=m_ScrollMiddle_Rc.bottom - m_ScrollMiddle_Rc.top - m_nThumbHeight;
	int n_TotalScrollPixel=(((int)m_xButtons.size()) * m_nButtonHeight) - (m_ScrollMiddle_Rc.bottom - m_ScrollMiddle_Rc.top);

	int Y=m_nButtonHeight*n_Total/n_TotalScrollPixel;
	if(Y<1)Y=1;
	SetThumbPos(m_ThumbPos + Y,true);
}


void CImageListCtrl::SetHeight(ULONG nHeight)
{
	CImageHandle m_xTop_Handle(&m_xTop);
	CImageHandle m_xLeft_Handle(&m_xLeft);
	CImageHandle m_xRight_Handle(&m_xRight);
	CImageHandle m_xBottom_Handle(&m_xBottom);
	CImageHandle m_xScrollT_Handle(&m_xScrollT);
	CImageHandle m_xScrollM_Handle(&m_xScrollM);
	CImageHandle m_xScrollB_Handle(&m_xScrollB);
	CImageHandle m_xThumb_Handle(&m_xThumb);

	SetWindowPos(NULL,0,0,m_xTop.GetWidth(),nHeight,SWP_NOMOVE);


	m_ScrollTop_Rc.right = m_xTop.GetWidth()-m_xRight.GetWidth();
	m_ScrollTop_Rc.left = m_ScrollTop_Rc.right - m_xScrollT.GetWidth();
	m_ScrollTop_Rc.top = m_xTop.GetHeight();
	m_ScrollTop_Rc.bottom = m_ScrollTop_Rc.top + m_xScrollT.GetHeight();

	m_ScrollBottom_Rc.left = m_ScrollTop_Rc.left;
	m_ScrollBottom_Rc.right = m_ScrollTop_Rc.right;
	m_ScrollBottom_Rc.bottom = nHeight - m_xBottom.GetHeight();
	m_ScrollBottom_Rc.top = m_ScrollBottom_Rc.bottom - m_xScrollB.GetHeight();

	m_ScrollMiddle_Rc.left = m_ScrollTop_Rc.left;
	m_ScrollMiddle_Rc.right = m_ScrollTop_Rc.right;
	m_ScrollMiddle_Rc.top = m_ScrollTop_Rc.bottom;
	m_ScrollMiddle_Rc.bottom = m_ScrollBottom_Rc.top;

	m_Body_Rc.left = m_xLeft.GetWidth();
	m_Body_Rc.right = m_xTop.GetWidth() - m_xRight.GetWidth() - m_xScrollT.GetWidth();
	m_Body_Rc.top = m_xTop.GetHeight();
	m_Body_Rc.bottom = nHeight - m_xBottom.GetHeight();

	m_MinThumbPos = m_ScrollMiddle_Rc.top;
	m_MaxThumbPos = m_ScrollMiddle_Rc.bottom - m_xThumb.GetHeight();
	m_nThumbHeight = m_xThumb.GetHeight();
	SetThumbPos(m_ThumbPos,false);

}

void CImageListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(IsPtInThumb(point))
	{
		m_nOldMousePos = m_ThumbPos - point.y;
		return;
	}
	else if(PtInRect(&m_Body_Rc,point))
	{//是否表表框之内
		m_nItem_tmp=ButtonIndex_FromPoint(point);
		if(m_nItem_tmp>=m_xButtons.size())
		{
			m_nItem_tmp=0;
		}
		else
		{
			m_nItem_tmp=m_xButtons[m_nItem_tmp]->dwItem;
		}
		m_nLastItem = 0;
	}
}

void CImageListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(IsPtInThumb(point))
	{
		m_nOldMousePos = m_ThumbPos - point.y;
		return;
	}
	else if(PtInRect(&m_Body_Rc,point))
	{//是否表表框之内
		m_nItem_tmpRT=ButtonIndex_FromPoint(point);
		if(m_nItem_tmpRT>=m_xButtons.size())
		{
			m_nItem_tmpRT=0;
		}
		else
		{
			m_nItem_tmpRT=m_xButtons[m_nItem_tmpRT]->dwItem;
		}
		m_nLastItemRT = 0;
	}
}


void CImageListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//是否是按键
	m_nOldMousePos=0;
	if(PtInRect(&m_Body_Rc,point))
	{//是否表表框之内
		ULONG nIndex=ButtonIndex_FromPoint(point);

		m_nLastItem=0;
		if(nIndex<m_xButtons.size())
		{
			ULONG nItem=m_xButtons[nIndex]->dwItem;
			if(nItem == m_nItem_tmp)
			{
				m_nLastItem   = nItem;
				m_nLastItemRT = 0;
				::SendMessage(GetParent()->GetSafeHwnd(),WM_COMMAND,
					MAKEWPARAM(m_xButtons[nIndex]->dwCmdId,BN_CLICKED),IMG_L_CLICK_BUTTON);
			}
		}
		m_nItem_tmp=0;
		return;
	}
	EventMouse(nFlags,point);
}

void CImageListCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_nOldMousePos=0;
	if(PtInRect(&m_Body_Rc,point))
	{//是否表表框之内
		ULONG nIndex=ButtonIndex_FromPoint(point);


		m_nLastItemRT=0;
		if(nIndex<m_xButtons.size())
		{
			ULONG nItem=m_xButtons[nIndex]->dwItem;
			if(nItem == m_nItem_tmpRT)
			{
				m_nLastItem   = 0;
				m_nLastItemRT = nItem;
				::SendMessage(GetParent()->GetSafeHwnd(),WM_COMMAND,
					MAKEWPARAM(m_xButtons[nIndex]->dwCmdId,BN_CLICKED),IMG_R_CLICK_BUTTON);
			}
		}
		m_nItem_tmpRT=0;
		return;
	}
	EventMouse(nFlags,point);
}

bool CImageListCtrl::IsPtInThumb(const POINT &pt)
{
	return PtInRect(CRect(
			m_ScrollMiddle_Rc.left,
			m_ThumbPos,
			m_ScrollMiddle_Rc.right,
			m_ThumbPos + m_nThumbHeight),
			pt)!=false;
}


void CImageListCtrl::EventMouse(UINT nFlags,const POINT &pt)
{
	if(m_nOldMousePos)
	{//是否是拖动Thumb
		SetThumbPos(pt.y+m_nOldMousePos,true);
	}
	else if(PtInRect(&m_ScrollTop_Rc,pt))
	{//是否是上箭头
		ScrollUp();
	}
	else if(PtInRect(&m_ScrollBottom_Rc,pt))
	{//是否是下箭头
		ScrollDown();
	}
}

void CImageListCtrl::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);


	CImageHandle m_xTop_Handle(&m_xTop);
	CImageHandle m_xLeft_Handle(&m_xLeft);
	CImageHandle m_xRight_Handle(&m_xRight);
	CImageHandle m_xBottom_Handle(&m_xBottom);
	CImageHandle m_xScrollT_Handle(&m_xScrollT);
	CImageHandle m_xScrollM_Handle(&m_xScrollM);
	CImageHandle m_xScrollB_Handle(&m_xScrollB);
	CImageHandle m_xThumb_Handle(&m_xThumb);


	m_xTop.AlphaDrawImage((CDC*)&dc,0,0,m_xTop.GetWidth(),m_xTop.GetHeight(),0,0,RGB(255,0,255));

	m_xLeft.Draw(dc,
					0,
					m_xTop.GetHeight(),
					m_xLeft.GetWidth(),
					rc.Height()-m_xTop.GetHeight()-m_xBottom.GetHeight());

	m_xRight.Draw(dc,
					rc.Width()-m_xRight.GetWidth(),
					m_xTop.GetHeight(),
					m_xRight.GetWidth(),
					rc.Height()-m_xTop.GetHeight()-m_xBottom.GetHeight());

	m_xBottom.AlphaDrawImage((CDC*)&dc,0,rc.Height()-m_xBottom.GetHeight(),m_xBottom.GetWidth(),m_xBottom.GetHeight(),0,0,RGB(255,0,255));

	m_xScrollT.Draw(dc,m_ScrollTop_Rc);
	m_xScrollB.Draw(dc,m_ScrollBottom_Rc);
	m_xScrollM.Draw(dc,m_ScrollMiddle_Rc);
	m_xThumb.Draw(dc, m_ScrollTop_Rc.left,m_ThumbPos);

	if(!m_BtnDC)return;

	BitBlt(dc,
				m_Body_Rc.left,
				m_Body_Rc.top,
				m_Body_Rc.right-m_Body_Rc.left,
				m_Body_Rc.bottom - m_Body_Rc.top,
			m_BtnDC,0,GetScrollPixel(),SRCCOPY);
}


//光标消息
BOOL CImageListCtrl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
CPoint MousePoint;
UINT nFlags;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);
	nFlags=(GetKeyState(VK_LBUTTON)!=0);

	if(!nFlags)m_nOldMousePos=0;
	if(m_nOldMousePos)
	{
		EventMouse(nFlags,MousePoint);
	}

	if(PtInRect(&m_Body_Rc,MousePoint))
	{
		BOOL bRet=true;
		ULONG nBtnIndex_Mouse = ButtonIndex_FromPoint(MousePoint);
		if(nBtnIndex_Mouse < m_xButtons.size())
		{
			::SetCursor(::LoadCursor(NULL,/*IDC_HAND*/MAKEINTRESOURCE(32649)));
			bRet = false;
		}
		UpdateButton();
		if(bRet == false)return false;
	}
	return CStatic::OnSetCursor(pWnd,nHitTest,message);
}

