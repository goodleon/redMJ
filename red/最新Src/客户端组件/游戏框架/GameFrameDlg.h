#ifndef GAME_FRAME_DLG_HEAD_FILE
#define GAME_FRAME_DLG_HEAD_FILE

#pragma once

#include "GameFrame.h"
#include "GameFrameView.h"
#include "ToolBarDlg.h"

//类说明
class CGameFrameControl;
class CClientKernelSink;

//////////////////////////////////////////////////////////////////////////

#define VOICE_SIGN TEXT("/VoiceTo:")

//游戏主窗口
class GAME_FRAME_CLASS CGameFrameDlg : public CDialog
{
	//友元定义
	friend class CClientKernelSink;
	friend class CGameFrameControl;

	//内核变量
protected:
	CClientKernelSink					* m_pKernelSink;				//钩子接口
	CUserFaceResHelper					m_UserFaceResHelper;			//头像组件
	CClientKernelHelper					m_ClientKernelHelper;			//游戏逻辑
	CMessageProxyHelper					m_MessageProxyHelper;			//聊天代理

	//界面辅助
private:
	bool								m_bReset;						//是否重置
	bool								m_bRectify;						//调整标志
	bool								m_bInitDialog;					//初始标志

	//配置变量
private:
	bool								m_bAllowSound;					//允许声音


	//控件变量
protected:
	CGameFrameView						* m_pGameFrameView;				//视图指针
	//CGameFrameControl					* m_pGameFrameControl;			//控制视图

	CRgnButton							*m_btQuit;						//退出按钮
	CRgnButton							*m_btMax;						//最大化按钮
	CRgnButton							*m_btMin;						//最小化按钮
	bool								m_bMaxed;						//最大化标志

	CRgnButton							*m_btButton1;					//功能按钮
	CRgnButton							*m_btButton2;					//功能按钮
	CRgnButton							*m_btButton3;					//功能按钮
	CRgnButton							*m_btButton4;					//功能按钮
	CRgnButton							*m_btButton5;					//功能按钮
	CHtmlBrower							*m_pHtmlBrower1;				//浏览窗口

	CRgnButton							*m_btOption;					//系统设置按钮
	CRgnButton							*m_btHelp;						//帮助按钮
public:
	CDlgToolBar							*m_ToolBar;						//工具栏
	CDlgChatMsg							*m_ChatMsgDlg;					//消息框
	CDlgUserList						*m_DlgUserList;					//用户列表框
	CExpression							m_Expression;					//表情窗口
	CRgnButton							*m_btToolSet;					//游戏设置
	
	//功能按钮
	afx_msg void OnBnClickedButton1();
	//功能按钮
	afx_msg void OnBnClickedButton2();
	//功能按钮
	afx_msg void OnBnClickedButton3();
	//功能按钮
	afx_msg void OnBnClickedButton4();
	//功能按钮
	afx_msg void OnBnClickedButton5();

	//游戏属性
public:
	WORD								m_wKindID;//当前游戏种类
	WORD								m_wGameGenre;

	//函数定义
public:
	//构造函数
	CGameFrameDlg(CGameFrameView * pGameFrameView);
	//析构函数
	virtual ~CGameFrameDlg();

	//拆分条接口
public:
	//按钮消息
	//virtual bool __cdecl OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton);
	//拆分条消息
	//virtual bool __cdecl OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos);

	//常规继承
private:
	//初始函数
	virtual bool InitGameFrame()=NULL;
	//重置框架
	virtual void ResetGameFrame()=NULL;
	//游戏设置
	virtual void OnGameOptionSet()=NULL;
	//游戏帮助
	virtual void OnGameOptionHelp(){};
	//时间消息
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)=NULL;
	//旁观状态
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)=NULL;
	//网络消息
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)=NULL;
	//游戏场景
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize)=NULL;

	//特殊继承
private:
	//调整界面
	virtual void RectifyControl(int nWidth, int nHeight);
	//框架消息
	virtual bool OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);

	//功能函数
public:
	//发送函数
	bool SendData(WORD wSubCmdID);
	//发送函数
	bool SendData(WORD wSubCmdID, void * pData, WORD wDataSize);
	//发送准备
	bool SendUserReady(void * pBuffer, WORD wDataSize);
	//设置定时器
	bool SetGameTimer(WORD wChairID, UINT nTimerID, UINT nElapse);
	//删除定时器
	bool KillGameTimer(UINT nTimerID);
	//播放声音
	bool PlayGameSound(LPCTSTR pszSoundName);
	//播放声音
	bool PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName);
	//播放声音
	bool PlayGameSoundFromFip(LPCTSTR pszSoundName);
	//播放声音
	bool PlayGameSoundFromFip2(UINT uEventType,WORD wToWho);

	//指定声音包文件
	bool SetWavPackage(LPCSTR szWavFile);

	//指定声音脚本文件
	bool SetWavScript(LPCSTR szWavScriptFile);

	//找出大厅窗口名柄
	HWND FindPlazaWindow();

	//消息函数
public:
	//插入图片
	bool InsertImage(CBitmap * pBitmap);
	//插入名字
	bool InsertUserName(LPCTSTR pszUserName);
	//系统消息
	bool InsertSystemString(LPCTSTR pszString);
	//特色消息
	bool InsertShtickString(LPCTSTR pszString, COLORREF crColor, bool bNewLine);
	//普通消息
	bool InsertGeneralString(LPCTSTR pszString, COLORREF crColor, bool bNewLine);

	//辅助函数
public:
	//激活界面
	void ActiveGameFrame();
	//切换椅子
	WORD SwitchViewChairID(WORD wChairID);		//这是原有的转换函数
	WORD GetChairCount();						//得到椅子数
	WORD XChair(WORD wChairID);	//从服务器id到位置(用于CGoldFlowerPlayer)
	WORD XChairUserInfo(WORD wChairID);			//从服务器id到位置(tagUserData *GetUserInfo())
	bool SetReadyIfAutoStart(CButton *pBtnStart);//自动派桌 的 响应

	//声音控制
public:
	//是否启用
	bool IsEnableSound();
	//启用声音
	void EnableSound(bool bEnable);

	//信息函数
public:
	//旁观模式
	bool IsLookonMode();
	//允许旁观
	bool IsAllowLookon();
	//自己位置
	WORD GetMeChairID();
	//时间位置
	WORD GetTimeChairID();
	//获取用户
	const tagUserData * GetUserData(WORD wChairID);
	//内核接口
	void * GetClientKernel(const IID & Guid, DWORD dwQueryVer);

	//游戏状态
public:
	//游戏状态
	BYTE GetGameStatus();
	//游戏状态
	void SetGameStatus(BYTE bGameStatus);

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//确定消息
	virtual void OnOK();
	//取消消息
	virtual void OnCancel();

	//消息映射
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnWindowPosChanging(WINDOWPOS * lpwndpos);
	afx_msg void OnBnClickedQuit();//退出
	afx_msg void OnBnClickedMax();//最大化
	afx_msg void OnBnClickedMin();//最小化
	afx_msg void OnBnClickedOption();//系统设置
	afx_msg void OnBnClickedHelp();//游戏帮助
	//表情消息
	LRESULT OnHitExpression(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif