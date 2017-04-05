//***********************************************
//File		: PlazaViewItem.h 
//Time		: 
//Author	: Anson
//Comment	: 游戏广场类
//***********************************************


#pragma once


#include "Stdafx.h"
#include "DlgLogon.h"
#include "ServerItemView.h"
#include "vector"

#include "TextRender.h"



struct tagGameButton
{
	CRgnButton Button;
	WORD wKindID;
	TCHAR szKindName[KIND_LEN];//游戏名字
};

//
//CPlazaViewItem(游戏广场类)
//
class CPlazaViewItem : public CDialog, public IPlazaViewItem, 
						public IClientSocketSink, 
						public ITreeCtrlSink
{
	//状态变量
protected:
	bool						m_bRectify;				//调整标志
	bool						m_bInitDialog;			//初始标志
	bool						m_bLogonPlaza;			//登录标志

	//控件变量
public:
	CDlgLogon					m_DlgLogon;				//登陆对话框
	//CHtmlBrower				*m_pHtmlBrower;			//浏览窗口
	CHtmlBrower					*m_pHtmlBrower1;		//浏览窗口
	CServerItemView				m_ServerItemView;		//服务器视图
	CClientSocketHelper			m_ClientSocket;			//网络连接

	TransButton					m_btGame1;
	TransButton					m_btGame2;
	TransButton					m_btGame3;

public:
	//游戏图标按钮集
	std::vector<tagGameButton*>		m_GameButtons;
	CImageListCtrl					m_RoomList;
	CImageListCtrl					m_ServerList;

	CSkinImage							m_StatusL;						//状态栏
	CSkinImage							m_StatusM;						//状态栏

	//游戏列表
protected:
	CSkinImage							m_ImageBGMain;					//主背景图片
	CSkinImage							m_ImageBGMain2;
	CSkinImage							m_ImageML;						//左边框
	CSkinImage							m_ImageMR;						//右边框


	CSkinImage						m_GameListT2;						//游戏列表
	CxImage							m_GameListT;						//游戏列表
	CxImage							m_GameListL;						//游戏列表
	CxImage							m_GameListR;						//游戏列表
	CxImage							m_GameListM;						//游戏列表
	CxImage							m_GameListB;						//游戏列表
	CSkinImage						m_GameListB2;						//游戏列表
	CxImage							m_GameListScroll;					//游戏列表


	CTextRenderWnd g_TextRender;											//文本链接对象

	//函数定义
public:
	CPlazaViewItem();
	virtual ~CPlazaViewItem();

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual void OnOK() { return; };
	//取消函数
	virtual void OnCancel() { return; };


	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CPlazaViewItem))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//视图接口
public:
	//获取句柄
	virtual HWND __cdecl GetWindowHandle() { return GetSafeHwnd(); }

	//树表接口
public:
	//左键单击
	virtual void __cdecl OnTreeLeftClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//右键单击
	virtual void __cdecl OnTreeRightClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//左键双击
	virtual void __cdecl OnTreeLeftDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//右键双击
	virtual void __cdecl OnTreeRightDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//选择改变
	virtual void __cdecl OnTreeSelectChanged(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//子项展开
	virtual void __cdecl OnTreeItemexpanded(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);

	//网络接口
public:
	//网络连接消息
	virtual bool __cdecl OnSocketConnect(int iErrorCode, LPCTSTR pszErrorDesc, IClientSocket * pIClientSocke);
	//网络读取消息
	virtual bool __cdecl OnSocketRead(CMD_Command Command, void * pBuffer, WORD wDataSize, IClientSocket * pIClientSocke);
	//网络关闭消息
	virtual bool __cdecl OnSocketClose(IClientSocket * pIClientSocke, bool bCloseByServer);

	//消息处理
protected:
	//登陆消息
	bool OnSocketMainLogon(CMD_Command Command, void * pBuffer, WORD wDataSize, IClientSocket * pIClientSocke);
	//列表消息
	bool OnSocketMainServerList(CMD_Command Command, void * pBuffer, WORD wDataSize, IClientSocket * pIClientSocke);
	//系统消息
	bool OnSocketMainSystem(CMD_Command Command, void * pBuffer, WORD wDataSize, IClientSocket * pIClientSocke);

	//广场接口
public:
	//登录消息
	virtual bool __cdecl SendLogonMessage();
	//连接消息
	virtual bool __cdecl SendConnectMessage();

	//功能函数
public:
	//显示网页
	//void WebBrowse(LPCTSTR lpszUrl, bool bForceBrowse);
	//显示消息
    int ShowMessageBox(LPCTSTR pszMessage, UINT nType);

	//内部函数
private:
	//更新标题
	void UpdateTitleText();
	//绘画列表
	void DrawListFrame(CDC * pDC);
	//调整控件
	void RectifyControl(int nWidth, int nHeight);

public:
	//bluker:显示服务列表
	void ShowServers(tagGameButton* pGameButton);
	void ClearGameButtons();
	//消息函数
protected:
	//绘画消息
	afx_msg void OnPaint();
	//绘画背景
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//登录广场
	afx_msg void OnCommandLogon();
	//连接服务器
	afx_msg void OnCommandConnect();
	//取消连接
	afx_msg void OnCommandCancelConnect();
	//位置消息
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//显示消息
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	//bluker:游戏种类图标按钮[点击事件]
	afx_msg void OnBnClicked_GameButtons();


	//房间列表框
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};



//-----------------------------------------------
//					the end
//-----------------------------------------------
