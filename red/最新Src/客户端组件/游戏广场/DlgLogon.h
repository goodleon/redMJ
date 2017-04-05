#ifndef DLG_LOGON_HEAD_FILE
#define DLG_LOGON_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "..\..\共享组件\公共服务\HtmlBrowser.h"
#include "PictureEx.h"

//////////////////////////////////////////////////////////////////////////


//注册表项名
#define REG_OPTION_LOGON		TEXT("OptionLogon")
#define REG_APP_PATH			TEXT("Software\\"szSystemRegKey"\\GamePlaza")
#define REG_USER_INFO			TEXT("Software\\"szSystemRegKey"\\GamePlaza\\UserInfo")
#define REG_LOGON_SERVER		TEXT("Software\\"szSystemRegKey"\\GamePlaza\\LogonServer")

//////////////////////////////////////////////////////////////////////////


//用户注册
class CDlgRegister : public CSkinDialogEx
{
	friend class CDlgLogon;

	//登陆信息
public:
	WORD								m_wFaceID;
	TCHAR								m_szAccounts[NAME_LEN];			//游戏帐号
	CString								m_strPassWord;					//登录密码

	//控件变量
public:
	CImageList							m_ImageList;
	CComboBoxEx							m_FaceSelect;
	CSkinButton							m_btLogon;						//登陆按钮
	CSkinButton							m_btCancel;						//取消按钮
	CSkinHyperLink						m_LineMainPage;					//游戏主页
	CSkinHyperLink						m_LinePassWord;					//密码保护

	//函数定义
public:
	//构造函数
	CDlgRegister();
	//析构函数
	virtual ~CDlgRegister();

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual void OnOK();
	//取消消息
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//登陆对话框
class CDlgLogon : public CDialog
{
	friend class CRoomViewItem;
	friend class CPlazaViewItem;

	//登陆信息
protected:
	WORD								m_wFaceID;						//头像标识
	DWORD								m_dwUserID;						//用户 I D
	TCHAR								m_szAccounts[NAME_LEN];			//游戏帐号
	CString								m_strPassWord;					//登录密码

	CxImage								m_AdImage;
	CPictureEx							m_Picture;


	//连接信息
protected:
	bool								m_bRegister;					//注册标志
	CString								m_strLogonServer;				//服务器地址
	static tagProxyInfo					m_ProxyInfo;					//代理信息

	//位置变量
protected:
	int									m_nFullWidth;					//全部宽度
	int									m_nFullHeight;					//全部高度
	bool								m_bNetOption;					//网络设置
	CRect								m_rcNormalFrame;				//框架位置

	//控件变量
public:
	CSkinButton							m_btLogon;						//登陆按钮
	CSkinButton							m_btCancel;						//取消按钮
	CSkinButton							m_btClear;						//删除按钮
	CSkinButton							m_btNetOption;					//网络按钮
	CSkinButton							m_btFindPass;					//找回密码
	CSkinButton							m_btWebSite;					//官方网站
	CSkinButton							m_btReg;						//游戏注册
	CSkinButton							m_btReg2;						//游戏注册
	CSkinButton							m_btFindPass2;					//找回密码

	CTextRenderWnd						m_TR;
public:
	CxImage								m_Logon_T;
	CxImage								m_Logon_L;
	CxImage								m_Logon_R;
	CxImage								m_Logon_M;
	CxImage								m_Logon_B;
	CxImage								m_Logon;

	//函数定义
public:
	//构造函数
	CDlgLogon();
	//析构函数
	virtual ~CDlgLogon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush m_brBkgnd;

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual void OnOK();
	//取消消息
	virtual void OnCancel();
	//功能函数
public:
	//获取服务器
	LPCTSTR GetLogonServer() { return m_strLogonServer; }
	//发送登录包
	bool SendLogonPacket(IClientSocket * pIClientSocke);
	//登陆成功处理
	bool OnLogonSuccess();

	//内部函数
private:
	//加载服务器
	void LoadLogonServer();
	//读取帐号
	void LoadAccountsInfo();
	//效验输入
	bool CheckLogonInput(bool bShowError);

    void Setup();
	//辅助函数
private:
	//显示用户登陆信息
	void SelectAccounts(LPCSTR pszAccounts,bool bChangeUser);
	//查找字符
	int ComboBoxFindString(CComboBox * pComboBox, LPCTSTR pszFindString);

	//消息函数
public:
	//注册帐号
	afx_msg void OnRegisterAccounts();
	//删除登陆用户信息
	afx_msg void OnDeleteAccounts();
	//选择改变
	afx_msg void OnSelchangeAccounts();
	//网络设置
	afx_msg void OnBnClickedNetOption();

	//找回密码
	afx_msg void OnBnClickedFindPass();;
	//官方网站
	afx_msg void OnBnClickedWebSite();
	//游戏注册
	afx_msg void OnBnClickedReg();



	//退出
	afx_msg void OnExit();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

//////////////////////////////////////////////////////////////////////////

#endif