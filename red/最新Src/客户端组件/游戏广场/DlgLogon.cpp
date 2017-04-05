#include "Stdafx.h"
#include "Resource.h"
#include "DlgLogon.h"
#include "GlobalUnits.h"
#include "DlgControlBar.h"
#include ".\dlglogon.h"

#define FACE_COUNT			50									//头像数目

#define LOGON_BGCOLOR      ((COLORREF)0xffffff)

//颜色定义
#define SELECT_COLOR		RGB(37,56,220)						//选择颜色

#define AD_LEFT				194
#define AD_TOP				39

//代理信息
tagProxyInfo				CDlgLogon::m_ProxyInfo;					//代理信息


//////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgRegister, CSkinDialogEx)
END_MESSAGE_MAP()
//构造函数
CDlgRegister::CDlgRegister() : CSkinDialogEx(IDD_REGISTER)
{
	//登录信息
	m_wFaceID=0;
	m_szAccounts[0]=0;

	return;
}

//析构函数
CDlgRegister::~CDlgRegister()
{
}
//控件绑定
void CDlgRegister::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btLogon);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_USER_FACE, m_FaceSelect);
	DDX_Control(pDX, IDC_MAIN_PAGE, m_LineMainPage);
	DDX_Control(pDX, IDC_PASSWORD_PROTECT, m_LinePassWord);
}

//初始化函数
BOOL CDlgRegister::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("帐号注册"));

	//限制输入
	((CComboBox *)(GetDlgItem(IDC_ACCOUNTS)))->LimitText(NAME_LEN-1);
	((CEdit *)(GetDlgItem(IDC_PASSWORD)))->LimitText(PASS_LEN-1);
	((CEdit *)(GetDlgItem(IDC_PASSWORD2)))->LimitText(PASS_LEN-1);
	m_LineMainPage.SetHyperLinkUrl(TEXT("http://"ServerDomain));
	m_LinePassWord.SetHyperLinkUrl(TEXT("http://"ServerDomain));

	CBitmap Image;
	//Image.LoadBitmap(IDB_FACE_NORMAL);
	m_ImageList.Create(32,32,ILC_COLOR16|ILC_MASK,0,0);
	m_ImageList.Add(&Image,RGB(255,0,255));
	m_FaceSelect.SetImageList(&m_ImageList);

	COMBOBOXEXITEM Item;
	ZeroMemory(&Item,sizeof(Item));
	Item.mask=CBEIF_IMAGE|CBEIF_SELECTEDIMAGE;
	for (int i=0;i<50;i++) 
	{
		Item.iImage=i;
		Item.iSelectedImage=i;
		m_FaceSelect.InsertItem(&Item);
	}

	m_wFaceID=rand()%50;
	m_FaceSelect.SetCurSel(m_wFaceID);

	return TRUE;
}

//确定函数
void CDlgRegister::OnOK()
{
	CString strBuffer;

	GetDlgItemText(IDC_ACCOUNTS,strBuffer);
	strBuffer.TrimLeft();
	strBuffer.TrimRight();
	if (strBuffer.IsEmpty())
	{
		AfxMessageBox(TEXT("帐号名字不能为空，请重新输入注册帐号！"),MB_ICONQUESTION);
		GetDlgItem(IDC_ACCOUNTS)->SetFocus();
		return;
	}
	lstrcpyn(m_szAccounts,strBuffer,CountArray(m_szAccounts));

	GetDlgItemText(IDC_PASSWORD,strBuffer);
	if (strBuffer.IsEmpty())
	{
		AfxMessageBox(TEXT("游戏密码不能为空，请重新输入游戏密码！"),MB_ICONQUESTION);
		GetDlgItem(IDC_PASSWORD)->SetFocus();
		return;
	}

	GetDlgItemText(IDC_PASSWORD2,m_strPassWord);
	if (strBuffer!=m_strPassWord)
	{
		AfxMessageBox(TEXT("游戏密码与确认密码不一致，请重新输入确认密码！"),MB_ICONQUESTION);
		GetDlgItem(IDC_PASSWORD2)->SetFocus();
		return;
	}

	m_wFaceID=(FACE_COUNT-m_FaceSelect.GetCurSel()-1);

	__super::OnOK();
}

//取消消息
void CDlgRegister::OnCancel()
{
	__super::OnCancel();
}


//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDlgLogon, CDialog)
	ON_CBN_SELCHANGE(IDC_ACCOUNTS, OnSelchangeAccounts)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_NET_OPTION, OnBnClickedNetOption)
	ON_BN_CLICKED(IDC_DELETE_RECORD, OnDeleteAccounts)
	ON_BN_CLICKED(IDB_LOGON_FINDPASS,  OnBnClickedFindPass)

	ON_BN_CLICKED(IDC_FORGET,  OnBnClickedFindPass)
	ON_BN_CLICKED(IDC_REGISTER,  OnBnClickedReg)

	ON_BN_CLICKED(IDB_LOGON_WEBSITE,  OnBnClickedWebSite)
	ON_BN_CLICKED(IDB_LOGON_REG,  OnBnClickedReg)
	
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

HBRUSH CDlgLogon::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(pWnd->GetDlgCtrlID() == IDC_REMEMBER_PASSWORD)
	{
		//pDC->SetTextColor(RGB(0,255,0));
		//pDC->SetBkColor(RGB(251,255,255));
		m_brBkgnd.CreateSolidBrush(RGB(183,223,227));
		pDC->SetBkColor(RGB(183,223,227));
		//pDC->SetBkMode(TRANSPARENT);		
		return (HBRUSH)m_brBkgnd;
		//return hbr;
	}
	//pDC->SetTextColor(RGB(0,0,255));

	pDC->SetBkMode(TRANSPARENT);
	return hbr;
}


//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgLogon::CDlgLogon() : CDialog(IDD_LOGON)
{
	//登录信息
	m_wFaceID=0;
	m_dwUserID=0;
	m_szAccounts[0]=0;

	//代理信息
	m_ProxyInfo.wProxyPort=0;
	m_ProxyInfo.cbProxyType=PROXY_NONE;


	//位置信息
	m_nFullWidth=0;
	m_nFullHeight=0;
	m_bNetOption=false;
	m_bRegister = false;

	return;
}

//析构函数
CDlgLogon::~CDlgLogon()
{
}

//控件绑定
void CDlgLogon::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_NET_OPTION, m_btNetOption);//网络设置
		DDX_Control(pDX, IDCANCEL, m_btCancel);//取消
		DDX_Control(pDX, IDOK, m_btLogon);//登陆
		
		DDX_Control(pDX, IDC_REGISTER, m_btReg2);//登陆
		DDX_Control(pDX, IDC_FORGET, m_btFindPass2);//登陆

		DDX_Control(pDX, IDC_DELETE_RECORD, m_btClear);//登陆

		DDX_Control(pDX, IDC_STATIC, m_Picture);
}

//初始化函数
BOOL CDlgLogon::OnInitDialog()
{
	__super::OnInitDialog();

	//写AppPath到注册表
	CRegKey RegAppPath;
	RegAppPath.Open(HKEY_CURRENT_USER,REG_APP_PATH,KEY_WRITE);
	char fpath[MAX_PATH];
	::GetModuleFileName(NULL,fpath,MAX_PATH);
	*strrchr(fpath,'\\')=0;
	RegAppPath.SetStringValue("Path",fpath);


	//SetWindowRgnFromRes(this->GetSafeHwnd(),AfxGetInstanceHandle(),TEXT("RGN_LOGON"),TEXT("BIN"));

	LayerWindow(this->GetSafeHwnd(),230);

	//if(!m_AdImage.Load("./AD.png",CXIMAGE_FORMAT_PNG))
	//{
	//	m_AdImage.LoadResource(FindResource(NULL,TEXT("IDB_LOGON_AD"),RT_BITMAP),CXIMAGE_FORMAT_BMP);
	//}

	if (!m_Picture.Load("./ad.gif"))
	{
		m_Picture.Load(MAKEINTRESOURCE(IDR_AD),_T("GIF"));
	}

	m_Picture.Draw();		

	m_Logon.LoadResource(FindResource(NULL,TEXT("IDB_LOGON"),RT_BITMAP),CXIMAGE_FORMAT_BMP);
	//m_Logon_T.LoadResource(FindResource(NULL,TEXT("IDB_LOGON_T"),RT_BITMAP),CXIMAGE_FORMAT_BMP);
	//m_Logon_L.LoadResource(FindResource(NULL,TEXT("IDB_LOGON_L"),RT_BITMAP),CXIMAGE_FORMAT_BMP);
	//m_Logon_R.LoadResource(FindResource(NULL,TEXT("IDB_LOGON_R"),RT_BITMAP),CXIMAGE_FORMAT_BMP);
	//m_Logon_M.LoadResource(FindResource(NULL,TEXT("IDB_LOGON_M"),RT_BITMAP),CXIMAGE_FORMAT_BMP);
	//m_Logon_B.LoadResource(FindResource(NULL,TEXT("IDB_LOGON_B"),RT_BITMAP),CXIMAGE_FORMAT_BMP);

	//m_btFindPass.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(140,26,0,0),this,IDB_LOGON_FINDPASS);
	//m_btWebSite.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(200,26,0,0),this,IDB_LOGON_WEBSITE);
	//m_btReg.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(260,26,0,0),this,IDB_LOGON_REG);

	//设置图片
	//m_btFindPass.SetButtonImage(IDB_LOGON_FINDPASS,AfxGetInstanceHandle(),false);
	//m_btWebSite.SetButtonImage(IDB_LOGON_WEBSITE,AfxGetInstanceHandle(),false);
	//m_btReg.SetButtonImage(IDB_LOGON_REG,AfxGetInstanceHandle(),false);

	//设置标题
	SetWindowText(TEXT("游戏登录"));
    COLORREF BGColor=RGB(255,224,125);
	COLORREF NormalTextColor=RGB(143,107,29);
	COLORREF HoverTextColor=RGB(0,0,255);
	COLORREF VisitedTextColor=RGB(255,0,0);



	//限制输入
	((CComboBox *)(GetDlgItem(IDC_ACCOUNTS)))->LimitText(NAME_LEN-1);
	((CEdit *)(GetDlgItem(IDC_PASSWORD)))->LimitText(PASS_LEN-1);
	((CEdit *)(GetDlgItem(IDC_PROXY_PORT)))->LimitText(5);

	//代理配置
	m_ProxyInfo.wProxyPort=AfxGetApp()->GetProfileInt(REG_OPTION_LOGON,TEXT("ProxyPort"),8080);
	m_ProxyInfo.cbProxyType=AfxGetApp()->GetProfileInt(REG_OPTION_LOGON,TEXT("ProxyType"),PROXY_NONE);
	m_ProxyInfo.strProxyName=AfxGetApp()->GetProfileString(REG_OPTION_LOGON,TEXT("ProxyName"),TEXT(""));
	m_ProxyInfo.strProxyPass=AfxGetApp()->GetProfileString(REG_OPTION_LOGON,TEXT("ProxyPass"),TEXT(""));
	m_ProxyInfo.strProxyServer=AfxGetApp()->GetProfileString(REG_OPTION_LOGON,TEXT("ProxyServer"),TEXT(""));

	//设置控件
	CComboBox * pProxyCom=(CComboBox *)GetDlgItem(IDC_PROXY_TYPE);
	pProxyCom->AddString(TEXT("不使用代理"));
	int iItem=pProxyCom->AddString(TEXT("SOCK 4 代理"));
	pProxyCom->SetItemData(iItem,PROXY_SOCKS4);
	if (m_ProxyInfo.cbProxyType==PROXY_SOCKS4) pProxyCom->SetCurSel(iItem);
	iItem=pProxyCom->AddString(TEXT("SOCK 5 代理"));
	pProxyCom->SetItemData(iItem,PROXY_SOCKS5);
	if (m_ProxyInfo.cbProxyType==PROXY_SOCKS5) pProxyCom->SetCurSel(iItem);
	if (pProxyCom->GetCurSel()==CB_ERR) pProxyCom->SetCurSel(0);

	//设置代理
	SetDlgItemText(IDC_PROXY_SERVER,m_ProxyInfo.strProxyServer);
	SetDlgItemText(IDC_PROXY_USER,m_ProxyInfo.strProxyName);
	SetDlgItemText(IDC_PROXY_PASS,m_ProxyInfo.strProxyPass);
	SetDlgItemInt(IDC_PROXY_PORT,m_ProxyInfo.wProxyPort);

	//移动位置
	GetWindowRect(&m_rcNormalFrame);

	m_nFullWidth=m_rcNormalFrame.Width();
	m_nFullHeight=m_rcNormalFrame.Height();
	m_rcNormalFrame.bottom=m_rcNormalFrame.top+348;

	MoveWindow(m_rcNormalFrame.right,m_rcNormalFrame.top,510,310,1);
	//MoveWindow(&m_rcNormalFrame);
	CenterWindow();

	GetWindowRect(&m_rcNormalFrame);
	m_nFullWidth=m_rcNormalFrame.Width();
	m_nFullHeight=409;

	//记录密码
	if (g_GlobalOption.m_enAcountsRule==enAcountsRule_AccountsAndPass)
		((CButton *)GetDlgItem(IDC_REMEMBER_PASSWORD))->SetCheck(BST_CHECKED);

	//加载信息
	LoadLogonServer();
	LoadAccountsInfo();

	//代理配置
	if ((m_ProxyInfo.cbProxyType!=PROXY_NONE)&&(m_bNetOption==false))
	{
		OnBnClickedNetOption();
	}

	//设置焦点
	if (GetDlgItem(IDC_ACCOUNTS)->GetWindowTextLength()==0)
	{
		GetDlgItem(IDC_ACCOUNTS)->SetFocus();
		((CComboBox *)GetDlgItem(IDC_ACCOUNTS))->SetEditSel(0,-1);
	}
	else 
	{
		GetDlgItem(IDC_PASSWORD)->SetFocus();
		((CEdit *)GetDlgItem(IDC_PASSWORD))->SetSel(0,-1);
	}

	return FALSE;
}

//消息解释
BOOL CDlgLogon::PreTranslateMessage(MSG * pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}




//确定函数
void CDlgLogon::OnOK()
{
	//效验输入
	if (CheckLogonInput(true)==false) return;

	//登陆广场
	//ShowWindow(SW_HIDE);
	IPlazaViewItem * pIPlazaViewItem=g_pControlBar->GetPlazaViewItem();
	ASSERT(pIPlazaViewItem!=NULL);
	pIPlazaViewItem->SendConnectMessage();

	return;
}

//取消消息
void CDlgLogon::OnCancel()
{
	DestroyWindow();
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
	return;
}

//取消消息
void CDlgLogon::OnExit()
{
	AfxMessageBox(TEXT("！！！！！！"),MB_ICONQUESTION);
    DestroyWindow();
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
	return;
}

//发送登录包
bool CDlgLogon::SendLogonPacket(IClientSocket * pIClientSocke)
{
	//变量定义
	TCHAR szPassword[33];
	BYTE cbBuffer[SOCKET_PACKAGE];
	CMD5Encrypt::EncryptData(m_strPassWord,szPassword);

	if (m_bRegister==false)
	{
		//构造数据
		CMD_GP_LogonByAccounts * pLogonByAccounts=(CMD_GP_LogonByAccounts *)cbBuffer;
		memset(pLogonByAccounts,0,sizeof(CMD_GP_LogonByAccounts));
		pLogonByAccounts->dwPlazaVersion=g_GlobalUnits.GetPlazaVersion();
		CopyMemory(pLogonByAccounts->szPassWord,szPassword,sizeof(pLogonByAccounts->szPassWord));
		lstrcpyn(pLogonByAccounts->szAccounts,m_szAccounts,CountArray(pLogonByAccounts->szAccounts));
		lstrcpyn(pLogonByAccounts->szRealityPass,m_strPassWord,CountArray(pLogonByAccounts->szRealityPass));

		//机器序列号
		tagClientSerial ClientSerial;
		g_GlobalUnits.GetClientSerial(ClientSerial);

		//发送数据包
		CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_LogonByAccounts),sizeof(cbBuffer)-sizeof(CMD_GP_LogonByAccounts));
		Packet.AddPacket(&ClientSerial,sizeof(ClientSerial),DTP_COMPUTER_ID);
		pIClientSocke->SendData(MDM_GP_LOGON,SUB_GP_LOGON_ACCOUNTS,cbBuffer,sizeof(CMD_GP_LogonByAccounts)+Packet.GetDataSize());
	}
	else
	{
		//构造数据
		CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)cbBuffer;
		memset(pRegisterAccounts,0,sizeof(CMD_GP_RegisterAccounts));
		pRegisterAccounts->wFaceID=m_wFaceID;
		pRegisterAccounts->dwPlazaVersion=g_GlobalUnits.GetPlazaVersion();
		CopyMemory(pRegisterAccounts->szPassWord,szPassword,sizeof(pRegisterAccounts->szPassWord));
		lstrcpyn(pRegisterAccounts->szAccounts,m_szAccounts,CountArray(pRegisterAccounts->szAccounts));

		//机器序列号
		tagClientSerial ClientSerial;
		g_GlobalUnits.GetClientSerial(ClientSerial);

		//发送数据包
		CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_RegisterAccounts),sizeof(cbBuffer)-sizeof(CMD_GP_RegisterAccounts));
		Packet.AddPacket(&ClientSerial,sizeof(ClientSerial),DTP_COMPUTER_ID);
		pIClientSocke->SendData(MDM_GP_LOGON,SUB_GP_REGISTER_ACCOUNTS,cbBuffer,sizeof(CMD_GP_RegisterAccounts)+Packet.GetDataSize());
		m_bRegister=false;
	}
	return true;
}

//登陆成功处理
bool CDlgLogon::OnLogonSuccess()
{
	//登录配置
	BOOL bRemPassword=(((CButton *)GetDlgItem(IDC_REMEMBER_PASSWORD))->GetCheck()==BST_CHECKED);
	AfxGetApp()->WriteProfileString(REG_OPTION_LOGON,TEXT("LogonServer"),m_strLogonServer);

	//配置信息
	if (bRemPassword) g_GlobalOption.m_enAcountsRule=enAcountsRule_AccountsAndPass;

	//用户信息
	//
	tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	AfxGetApp()->WriteProfileString(REG_OPTION_LOGON,TEXT("LastAccounts"),UserData.szAccounts);


	//写入用户记录
	TCHAR szBuffer[256];
	if (g_GlobalOption.m_enAcountsRule!=enAcountsRule_None)
	{
		CRegKey RegUserInfo;
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%s\\%s"),REG_USER_INFO,UserData.szAccounts);
		if (RegUserInfo.Create(HKEY_CURRENT_USER,szBuffer)==ERROR_SUCCESS)
		{
			TCHAR szPassBuffer[256]=TEXT("");
			if (bRemPassword) CXOREncrypt::EncryptData(m_strPassWord,szPassBuffer,CountArray(szPassBuffer));
			RegUserInfo.SetKeyValue(TEXT(""),UserData.szAccounts,TEXT("UserAccount"));
			RegUserInfo.SetKeyValue(TEXT(""),UserData.szPassWord,TEXT("UserPassToken"));
			RegUserInfo.SetKeyValue(TEXT(""),szPassBuffer,TEXT("UserPassword"));
		}
	}
	else
	{
		CRegKey RegUserID;
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),UserData.dwUserID);
		if (RegUserID.Open(HKEY_CURRENT_USER,REG_USER_INFO)==ERROR_SUCCESS)	RegUserID.RecurseDeleteKey(szBuffer);
	}

	//关闭窗口
	DestroyWindow();

	return true;
}

//读取帐号
void CDlgLogon::LoadAccountsInfo()
{
	//变量定义
	CComboBox * pComBoxAccounts=(CComboBox *)GetDlgItem(IDC_ACCOUNTS);

	//读取信息
	CRegKey RegUserInfo;
	RegUserInfo.Open(HKEY_CURRENT_USER,REG_USER_INFO,KEY_READ);
	if (RegUserInfo!=NULL)
	{
		int iInsertItem=CB_ERR;
		LONG lErrorCode=ERROR_SUCCESS;
		DWORD dwType=REG_SZ,dwIndex=0,dwUserDBID=0;
		TCHAR szUserIDBuffer[32]=TEXT(""),szTempBuffer[256]=TEXT("");
		do
		{
			//设置变量
			szTempBuffer[0]=0;
			szUserIDBuffer[0]=0;

			//读取游戏 ID
			DWORD dwBufferSize=sizeof(szUserIDBuffer);
			if (RegUserInfo.EnumKey(dwIndex++,szUserIDBuffer,&dwBufferSize,NULL)!=ERROR_SUCCESS) break;
			if(strlen(szUserIDBuffer)<1)
				continue;
			//加载其他信息
			CRegKey RegUserAccount;
			_snprintf(szTempBuffer,sizeof(szTempBuffer),TEXT("%s\\%s"),REG_USER_INFO,szUserIDBuffer);
			RegUserAccount.Open(HKEY_CURRENT_USER,szTempBuffer,KEY_READ);
			if (RegUserAccount!=NULL)
			{
				//游戏帐号
				dwBufferSize=sizeof(szTempBuffer);
				lErrorCode=RegUserAccount.QueryValue(TEXT("UserAccount"),&dwType,szTempBuffer,&dwBufferSize);
				if ((lErrorCode==ERROR_SUCCESS)&&(szTempBuffer[0]!=0)&&(ComboBoxFindString(pComBoxAccounts,szTempBuffer)==LB_ERR))
				{
					iInsertItem=pComBoxAccounts->AddString(szTempBuffer);
					pComBoxAccounts->SetItemData(iInsertItem,dwUserDBID);
				}
			}
		} while (true);
	}

	//读取最近登陆用户
	CString strLastUserID=AfxGetApp()->GetProfileString(REG_OPTION_LOGON,TEXT("LastAccounts"),NULL);
	if (strLastUserID.IsEmpty()==FALSE)
	{
		SelectAccounts(strLastUserID,true);
	}

	return;
}

void CDlgLogon::SelectAccounts(LPCSTR pszAccounts,bool bChangeUser)
{
	if(bChangeUser)
	{
		CComboBox * pComBoxAccounts=(CComboBox *)GetDlgItem(IDC_ACCOUNTS);
		pComBoxAccounts->SelectString(-1,pszAccounts);
	}

	CEdit * pPassWord=(CEdit *)(GetDlgItem(IDC_PASSWORD));

	TCHAR szTempBuffer[256]=TEXT("");
	CRegKey RegPassword;
	_snprintf(szTempBuffer,sizeof(szTempBuffer),TEXT("%s\\%s"),REG_USER_INFO,pszAccounts);
	RegPassword.Open(HKEY_CURRENT_USER,szTempBuffer,KEY_READ);

	//游戏帐号
	ULONG dwBufferSize=sizeof(szTempBuffer);
	DWORD dwType;
	dwBufferSize=RegPassword.QueryValue(TEXT("UserPassword"),&dwType,szTempBuffer,&dwBufferSize);
	if ((dwBufferSize==ERROR_SUCCESS)&&(szTempBuffer[0]!=0))
	{
		m_strPassWord.Empty();
		LPTSTR pszScrPass=m_strPassWord.GetBuffer(PASS_LEN+1);
		CXOREncrypt::CrevasseData(szTempBuffer,pszScrPass,PASS_LEN+1);
		m_strPassWord.ReleaseBuffer();
		SetDlgItemText(IDC_PASSWORD,m_strPassWord);
		((CButton *)GetDlgItem(IDC_REMEMBER_PASSWORD))->SetCheck(BST_CHECKED);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_REMEMBER_PASSWORD))->SetCheck(BST_UNCHECKED);
		SetDlgItemText(IDC_PASSWORD,TEXT(""));
	}
}



//选择改变
void CDlgLogon::OnSelchangeAccounts()
{
CString szUser;
	CComboBox * pComBoxAccounts=(CComboBox *)GetDlgItem(IDC_ACCOUNTS);
	pComBoxAccounts->GetLBText(pComBoxAccounts->GetCurSel(),szUser);
	SelectAccounts(szUser,false);
	return;
}

//加载服务器
void CDlgLogon::LoadLogonServer()
{
	//读取最近登陆服务器
	CComboBox * pComBoxServer=(CComboBox *)GetDlgItem(IDC_SERVER);
	m_strLogonServer=AfxGetApp()->GetProfileString(REG_OPTION_LOGON,TEXT("LogonServer"),NULL);
	if (m_strLogonServer.IsEmpty()==FALSE)
	{
		pComBoxServer->AddString(m_strLogonServer);
		pComBoxServer->SetWindowText(m_strLogonServer);
	}

	//读取服务器列表
	CRegKey RegLogonServer;
	RegLogonServer.Open(HKEY_CURRENT_USER,REG_LOGON_SERVER,KEY_READ);
	if (RegLogonServer!=NULL)
	{
		TCHAR szLogonServer[128]=TEXT("");
		DWORD dwIndex=0,dwBufferSize=sizeof(szLogonServer);
		do
		{
			dwBufferSize=sizeof(szLogonServer);
			if (RegLogonServer.EnumKey(dwIndex++,szLogonServer,&dwBufferSize,NULL)!=ERROR_SUCCESS) break;
			if (szLogonServer[0]!=0)
			{
				if (m_strLogonServer.IsEmpty()) m_strLogonServer=szLogonServer;
				if (ComboBoxFindString(pComBoxServer,szLogonServer)==LB_ERR) pComBoxServer->AddString(szLogonServer);
			}
		} while (true);
	}

	//设置选择
	if ((pComBoxServer->GetWindowTextLength()==0)&&(pComBoxServer->GetCount()>0)) pComBoxServer->SetCurSel(0);
	if (pComBoxServer->GetCount()==0)
	{
		pComBoxServer->AddString(ServerDomain);
		pComBoxServer->SetCurSel(0);
	}

	return;
}

//查找数据
int CDlgLogon::ComboBoxFindString(CComboBox * pComboBox, LPCTSTR pszFindString)
{
	CString strBuffer;
	int iItemHandle=LB_ERR,iItemFind=LB_ERR;
	int nStringLen=lstrlen(pszFindString);
	do 
	{
		iItemFind=pComboBox->FindString((iItemHandle==LB_ERR)?0:(iItemHandle+1),pszFindString);
		if ((iItemFind==LB_ERR)||(iItemHandle==iItemFind)||(iItemFind<iItemHandle)) return LB_ERR;
		iItemHandle=iItemFind;
		pComboBox->GetLBText(iItemHandle,strBuffer);
		if (pComboBox->GetLBTextLen(iItemHandle)==nStringLen) return iItemHandle;
	} while (true);
	return LB_ERR;
}

//效验输入
bool CDlgLogon::CheckLogonInput(bool bShowError)
{
	//定义变量
	UINT uControlID=0;
	CString strBuffer;

	try
	{
		//登录服务器
		GetDlgItemText(IDC_SERVER,m_strLogonServer);
		m_strLogonServer.TrimLeft();
		m_strLogonServer.TrimRight();
		if (m_strLogonServer.IsEmpty())
		{
			uControlID=IDC_SERVER;
			throw TEXT("登录服务器不能为空，请重新选择或者输入登录服务器！");
		}

		//登录帐号

		GetDlgItemText(IDC_ACCOUNTS,strBuffer);
		strBuffer.TrimLeft();
		strBuffer.TrimRight();
		if (strBuffer.IsEmpty())
		{
			uControlID=IDC_ACCOUNTS;
			throw TEXT("用户登录帐号不能为空，请重新输入登录帐号！");
		}
		lstrcpyn(m_szAccounts,strBuffer,CountArray(m_szAccounts));


		//用户密码
		GetDlgItemText(IDC_PASSWORD,m_strPassWord);
		if (m_strPassWord.IsEmpty())
		{
			uControlID=IDC_PASSWORD;
			throw TEXT("登录密码不能为空，请重新输入登录密码！");
		}

		//代理信息
		m_ProxyInfo.wProxyPort=GetDlgItemInt(IDC_PROXY_PORT,NULL,FALSE);
		GetDlgItemText(IDC_PROXY_USER,m_ProxyInfo.strProxyName);
		GetDlgItemText(IDC_PROXY_PASS,m_ProxyInfo.strProxyPass);
		GetDlgItemText(IDC_PROXY_SERVER,m_ProxyInfo.strProxyServer);
		CComboBox * pProxyCom=(CComboBox *)GetDlgItem(IDC_PROXY_TYPE);
		int iItem=pProxyCom->GetCurSel();
		m_ProxyInfo.cbProxyType=(BYTE)pProxyCom->GetItemData(iItem);
		if (m_ProxyInfo.cbProxyType!=PROXY_NONE)
		{
			if (m_ProxyInfo.strProxyServer.IsEmpty())
			{
				uControlID=IDC_PROXY_SERVER;
				throw TEXT("代理服务器地址不能为空，请重新输入代理服务器地址！");
			}
		}

		return true;
	}
	catch (LPCTSTR pszError)
	{
		if (bShowError)
		{
			ShowWindow(SW_SHOW);
			BringWindowToTop();
			AfxMessageBox(pszError,MB_ICONQUESTION);
			if (uControlID!=0) GetDlgItem(uControlID)->SetFocus();
		}

	}
	return false;
}


//网络设置
void CDlgLogon::OnBnClickedNetOption()
{
	m_bNetOption=!m_bNetOption;
	GetDlgItem(IDC_PROXY_TYPE)->EnableWindow(m_bNetOption);
	GetDlgItem(IDC_PROXY_SERVER)->EnableWindow(m_bNetOption);
	GetDlgItem(IDC_PROXY_PORT)->EnableWindow(m_bNetOption);
	GetDlgItem(IDC_PROXY_USER)->EnableWindow(m_bNetOption);
	GetDlgItem(IDC_PROXY_PASS)->EnableWindow(m_bNetOption);
	int nHeight=m_bNetOption?m_nFullHeight:m_rcNormalFrame.Height();
	SetWindowPos(GetParent(),0,0,m_nFullWidth,nHeight,SWP_NOMOVE);

	return;
}

//找回密码
afx_msg void CDlgLogon::OnBnClickedFindPass()
{
	g_GlobalUnits.GoWeb("/Client/Forget.asp");
}

//官方网站
afx_msg void CDlgLogon::OnBnClickedWebSite()
{
	g_GlobalUnits.GoWeb("/");
}
//游戏注册
afx_msg void CDlgLogon::OnBnClickedReg()
{
	g_GlobalUnits.GoWeb("/Client/Reg.asp");
	//OnRegisterAccounts();
}

//注册帐号
void CDlgLogon::OnRegisterAccounts()
{
	//*

	CDlgRegister DlgRegister;
	ShowWindow(SW_HIDE);
	if (DlgRegister.DoModal()!=IDOK) 
	{
		ShowWindow(SW_SHOW);
		return;
	}

	//设置变量
	m_bRegister=true;
	m_wFaceID=DlgRegister.m_wFaceID;
	m_strPassWord=DlgRegister.m_strPassWord;
	lstrcpy(m_szAccounts,DlgRegister.m_szAccounts);

	//服务器
	GetDlgItemText(IDC_SERVER,m_strLogonServer);
	if (m_strLogonServer.IsEmpty()) m_strLogonServer=ServerDomain;

	//登陆广场
	ShowWindow(SW_HIDE);
	IPlazaViewItem * pIPlazaViewItem=g_pControlBar->GetPlazaViewItem();
	ASSERT(pIPlazaViewItem!=NULL);
	pIPlazaViewItem->SendConnectMessage();
	return;
    
	//*/
}

//删除登陆用户信息
void CDlgLogon::OnDeleteAccounts() 
{
	//获取窗口
	UINT uComboBoxID=0;

	CComboBox * pComboBox=(CComboBox *)GetDlgItem(IDC_ACCOUNTS);

	//删除信息
	CString strBuffer;
	pComboBox->GetWindowText(strBuffer);
	pComboBox->SetWindowText(TEXT(""));
	if (strBuffer.IsEmpty()) return;
	int iSelectItem=ComboBoxFindString(pComboBox,strBuffer);
	if (iSelectItem!=LB_ERR)
	{
		CRegKey RegAccounts;
		CString strAccounts;

		pComboBox->GetWindowText(strAccounts);

		RegAccounts.Open(HKEY_CURRENT_USER,REG_USER_INFO);
		if (RegAccounts!=NULL) RegAccounts.RecurseDeleteKey(strAccounts);

		//删除选择信息
		CComboBox * pOtherComboBox=NULL;

		pOtherComboBox=(CComboBox *)GetDlgItem(IDC_ACCOUNTS);
		for (int j=0;j<pOtherComboBox->GetCount();j++)
		{
			pOtherComboBox->DeleteString(j);
		}

	}

	return;
}

//////////////////////////////////////////////////////////////////////////

void CDlgLogon::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rc;
	GetClientRect(rc);

	dc.FillRect(rc,&CBrush((COLORREF)LOGON_BGCOLOR));

	m_Logon.Draw(dc,0,0);

	m_AdImage.Draw(dc,AD_LEFT,AD_TOP);
	//m_Logon_T.Draw(dc,0,0);
	//m_Logon_L.Draw(dc,0,48);
	//m_Logon_R.Draw(dc,322-6,48);
	////m_Logon_M.Draw(dc,6,260);
	//m_Logon_B.Draw(dc,0,rc.bottom-7);

	DWORD textAlignment = THA_LEFT | TVA_TOP;
	RECT margins = {0,0,0,0};
	dc.SetBkMode(TRANSPARENT);
	m_TR.m_color=0x942525;//0x111197
	m_TR.SetFontFace("宋体",12);
	//m_TR.EnhDrawText2( dc, "\\b0服务器：",35,174);
	//m_TR.EnhDrawText2( dc, "\\b0用户名：",35,198);
	//m_TR.EnhDrawText2( dc, "\\b0密码：",35,222);

	//m_TR.EnhDrawText2( dc, "\\b0记住密码",195,222);

	m_TR.EnhDrawText2( dc, "\\b0类型：",20,329);
	m_TR.EnhDrawText2( dc, "\\b0地址：",20,351);
	m_TR.EnhDrawText2( dc, "\\b0用户：",20,371);

	m_TR.EnhDrawText2( dc, "\\b0端口：",170,351);
	m_TR.EnhDrawText2( dc, "\\b0密码：",170,371);

	//m_TR.EnhDrawText( dc, "\\b0服务器：", (11), &CRect(20,120,(100),(100)), &margins, textAlignment );

}






BOOL CDlgLogon::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return false;
}

void CDlgLogon::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if(point.y<25)
	//{
		SendMessage(WM_NCLBUTTONDOWN, HTCAPTION,0);
	//}
	//CRect rc(AD_LEFT,AD_TOP,AD_LEFT + m_Picture.GetWidth(),AD_TOP+m_Picture.GetHeight());
	
	CRect rect;
	m_Picture.GetWindowRect(&rect);
	ScreenToClient(&rect);

	if(PtInRect(rect,point))
	{
		g_GlobalUnits.GoWeb("/Client/LoginAd.asp",false);
	}
	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CDlgLogon::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);
	
	CRect rect;
	m_Picture.GetWindowRect(&rect);
	ScreenToClient(&rect);


	if(PtInRect(rect,MousePoint))
	{
		SetCursor(LoadCursor(0,MAKEINTRESOURCE(32649)));
		return true;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
