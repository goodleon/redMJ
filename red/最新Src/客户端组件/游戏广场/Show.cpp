#include "Stdafx.h"
#include "GlobalUnits.h"
#include "Show.h"
#include "RoomViewItem.h"

#define CONTROL_SPACE						2							//控件间距
#define PIC_BT_WIDE							20							//按钮宽度
#define PIC_BT_HIGHT						20							//按钮高度
#define BT_BUTTON_WIDE						65							//按钮宽度
#define BT_BUTTON_HIGHT						21							//按钮高度

BEGIN_MESSAGE_MAP(CShow, CSkinDialogEx)
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_SHOW_CLOSE, OnBnClickedClose)
END_MESSAGE_MAP()

//构造函数
CShow::CShow(CWnd * pParentWnd) : CSkinDialogEx(IDD_SHOW)
{
	s_pParentWnd=NULL;
	m_dwTargetUserID=0L;
	m_pParentWnd=pParentWnd;
	m_pTargetUserItem=NULL;
}

//析构函数
CShow::~CShow()
{
}

//控件绑定函数
void CShow::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOW_CLOSE,m_BtClose);
}

//初始化函数
BOOL CShow::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	if (m_pTargetUserItem!=NULL)
	{
		TCHAR szTitle[256];
		_snprintf(szTitle,sizeof(szTitle),TEXT("[ %s ] － 个人形象SHOW"),m_pTargetUserItem->GetUserData()->szName,m_strRoomName);
		SetWindowText(szTitle);
		
	}
	return FALSE;
}




//退出按钮
void CShow::OnBnClickedClose()
{
	DestroyWindow();
	return;
}

//获取位置
void CShow::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	lpMMI->ptMinTrackSize.x=300;
	lpMMI->ptMinTrackSize.y=250;
	return;
}


