#include "StdAfx.h"
#include ".\dlgchangetheme.h"
#include "GamePlaza.h"

CDlgChangeTheme::CDlgChangeTheme(void): CSkinDialogEx(IDD_DIALOG2)
{
}

CDlgChangeTheme::~CDlgChangeTheme(void)
{
}
//////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDlgChangeTheme, CSkinDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////
//ÅäÖÃ°´Å¥
void CDlgChangeTheme::OnBnClickedButton1()
{
	if(theApp.m_intTheme == 1)
	{
		theApp.m_intTheme = 2;
		
	}
	else
	{
		theApp.m_intTheme = 1;
	}
	::ShowWindow(theApp.m_MainPlaza,SW_HIDE);
	::ShowWindow(theApp.m_MainPlaza,SW_SHOW);
	//::PostMessage(theApp.m_MainPlaza,WM_PAINT,0,0);
}

//¿Ø¼þ°ó¶¨
void CDlgChangeTheme::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btButton1);
}