//***********************************************
//File		: PlazaViewItem.cpp 
//Time		: 
//Author	: Anson
//Comment	: 游戏广场类
//***********************************************


#include "Stdafx.h"
#include "../../公共文件/GlobalDef.h"
#include "GamePlaza.h"
#include "GlobalUnits.h"
#include "RoomViewItem.h"
#include "PlazaViewItem.h"
#include "DlgControlBar.h"

#include "BCMenu.h"
#include ".\plazaviewitem.h"

#define GAME_LIST_X     11
#define GAME_LIST_Y     11


#define		LIST_TOP			119				//列表头高
#define		LIST_LEFT			24				//列表左宽
#define		LIST_RIGHT			9				//列表右宽
#define		LIST_BOTTOM			4				//列表底高

#define		IDC_WEB_BEFORE		101
#define		IDC_WEB_BRUSH		102
#define		IDC_WEB_HOME		103
#define		IDC_WEB_NEXT		104
#define		IDC_WEB_STOP		105
#define		IDC_GAMEBUTTONS		8888

#define		IDC_BT_BUTTON_GAME1	7777			//功能按钮
#define		IDC_BT_BUTTON_GAME2	7778			//功能按钮
#define		IDC_BT_BUTTON_GAME3	7779			//功能按钮

BEGIN_MESSAGE_MAP(CPlazaViewItem, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_GAMEBUTTONS,	OnBnClicked_GameButtons)
	ON_COMMAND(IDM_LOGON_PLAZA,		OnCommandLogon)
	ON_COMMAND(IDM_CONNECT_SERVER,	OnCommandConnect)
	ON_COMMAND(IDM_CANCEL_CONNECT,	OnCommandCancelConnect)
END_MESSAGE_MAP()


UINT GetGameIco(const char *pszGameKind)
{
	//if(!strcmpi(pszGameKind,"斗地主"))
	//	return IDB_ICO_DDZ;
	//else if(!strcmpi(pszGameKind,"诈金花"))
	//	return IDB_ICO_ZJH;
	//else if(!strcmpi(pszGameKind,"长沙麻将"))
	//	return IDB_ICO_MJ;
	//else if(!strcmpi(pszGameKind,"梭哈"))
	//	return IDB_ICO_SH;
	//else if(!strcmpi(pszGameKind,"三打哈"))
	//	return IDB_ICO_SDH;
	//else if(!strcmpi(pszGameKind,"有奖游戏"))
	//	return IDB_ICO_YJYX;
	//else
	//{
		return IDB_ICO_DDZ;
	//}
}


CPlazaViewItem::CPlazaViewItem() : CDialog(IDD_GAME_PLAZA)
{
	//设置变量
	m_bRectify		= false;
	m_bInitDialog	= false;
	m_bLogonPlaza	= false;
	m_pHtmlBrower1	= NULL;
	//服务器列表
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBGMain.SetLoadInfo(IDB_BG_MAIN,hInstance);
	m_ImageBGMain2.SetLoadInfo(IDB_BG_MAIN2,hInstance);

	m_ImageML.SetLoadInfo(IDB_FRAME_ML,hInstance);
	m_ImageMR.SetLoadInfo(IDB_FRAME_MR,hInstance);

	m_GameListT2.SetLoadInfo(GAMELIST_T3,hInstance);     //背景
	m_GameListT.LoadResource(FindResource(NULL,"GAMELIST_T",RT_BITMAP),CXIMAGE_FORMAT_BMP);
	m_GameListL.LoadResource(FindResource(NULL,"GAMELIST_L",RT_BITMAP),CXIMAGE_FORMAT_BMP);
	m_GameListR.LoadResource(FindResource(NULL,"GAMELIST_R",RT_BITMAP),CXIMAGE_FORMAT_BMP);
	m_GameListM.LoadResource(FindResource(NULL,"GAMELIST_M",RT_BITMAP),CXIMAGE_FORMAT_BMP);
	m_GameListB2.SetLoadInfo(GAMELIST_B3,hInstance);     //背景
	m_GameListB.LoadResource(FindResource(NULL,"GAMELIST_B",RT_BITMAP),CXIMAGE_FORMAT_BMP);
	m_GameListScroll.LoadResource(FindResource(NULL,"GAMELIST_SCROLL",RT_BITMAP),CXIMAGE_FORMAT_BMP);

	m_StatusL.SetLoadInfo(IDB_STATUS_L,hInstance);
	m_StatusM.SetLoadInfo(IDB_STATUS_M,hInstance);
	return ;
}

CPlazaViewItem::~CPlazaViewItem()
{
	ClearGameButtons();
}

//控件绑定
void CPlazaViewItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_LIST, m_ServerItemView);
}

//初始化函数
BOOL CPlazaViewItem::OnInitDialog()
{
	__super::OnInitDialog();
//	m_ImageBGMain.LoadFromResource(IDB_BG_MAIN,hInstance);
	
	//设置接口
	g_pControlBar->SetPlazaViewItem(this);

	if(m_RoomList.Create(NULL, WS_CHILD|SS_LEFT|SS_NOTIFY,CRect(0,0,0,0), this))
	{
		HINSTANCE hInstance=AfxGetInstanceHandle();
		m_RoomList.m_xTop.SetLoadInfo(ROOMLIST_T,hInstance);
		m_RoomList.m_xLeft.SetLoadInfo(ROOMLIST_L1,hInstance);
		m_RoomList.m_xRight.SetLoadInfo(ROOMLIST_R1,hInstance);
		m_RoomList.m_xBottom.SetLoadInfo(ROOMLIST_B,hInstance);
		m_RoomList.m_xScrollT.SetLoadInfo(ROOMLIST_S_T,hInstance);
		m_RoomList.m_xScrollM.SetLoadInfo(ROOMLIST_S_M,hInstance);
		m_RoomList.m_xScrollB.SetLoadInfo(ROOMLIST_S_B,hInstance);
		m_RoomList.m_xThumb.SetLoadInfo(ROOMLIST_S_THUMB,hInstance);

		m_RoomList.SetHeight(191);
		//char *p = "诈金花房间1";
		//for(int i=100001;i<=100021;i++)
		//	m_RoomList.AddButton((HINSTANCE)FindResource(NULL,"ROOMLIST_BTN",RT_BITMAP),IMG_LISTCTRL_CMD_ID,(DWORD)p,20,5,0xffffff,i);
		m_RoomList.UpdateButton();
			SetWindowRgnFromRes(m_RoomList.m_hWnd,AfxGetInstanceHandle(),TEXT("RGN_ROOMLIST"),TEXT("BIN"));
	}
////////////////////////////////////////////////////////////////////////////

	if(m_ServerList.Create(NULL, WS_CHILD|SS_LEFT|SS_NOTIFY,CRect(0,0,0,0), this))
	{
		HINSTANCE hInstance=AfxGetInstanceHandle();
		m_ServerList.m_xTop.SetLoadInfo(GAMELIST_T2,hInstance);
		m_ServerList.m_xLeft.SetLoadInfo(ROOMLIST_L,hInstance);
		m_ServerList.m_xRight.SetLoadInfo(ROOMLIST_R,hInstance);
		m_ServerList.m_xBottom.SetLoadInfo(GAMELIST_B2,hInstance);
		m_ServerList.m_xScrollT.SetLoadInfo(ROOMLIST_S_T,hInstance);
		m_ServerList.m_xScrollM.SetLoadInfo(ROOMLIST_S_M,hInstance);
		m_ServerList.m_xScrollB.SetLoadInfo(ROOMLIST_S_B,hInstance);
		m_ServerList.m_xThumb.SetLoadInfo(ROOMLIST_S_THUMB,hInstance);

		m_ServerList.UpdateButton();
			//BUG,取消列表头顶颜色过滤,改用位图,否则档住了OnPaint()的渲染区域
			//SetWindowRgnFromRes(m_ServerList.m_hWnd,AfxGetInstanceHandle(),TEXT("RGN_SERVERLIST"),TEXT("BIN"));
	}

	m_btGame1.Create(TEXT("棋牌游戏"),WS_CHILD|WS_VISIBLE,CRect(-999,0,0,0),this,IDC_BT_BUTTON_GAME1);
	m_btGame1.SetButtonImage(TABCARD,AfxGetInstanceHandle(),false);
	m_btGame2.Create(TEXT("休闲游戏"),WS_CHILD|WS_VISIBLE,CRect(-999,0,0,0),this,IDC_BT_BUTTON_GAME2);
	m_btGame2.SetButtonImage(TABCARD,AfxGetInstanceHandle(),false);
	m_btGame3.Create(TEXT("大型网游"),WS_CHILD|WS_VISIBLE,CRect(-999,0,0,0),this,IDC_BT_BUTTON_GAME3);
	m_btGame3.SetButtonImage(TABCARD,AfxGetInstanceHandle(),false);
	
	m_btGame1.ShowWindow(SW_HIDE);
	m_btGame2.ShowWindow(SW_HIDE);
	m_btGame3.ShowWindow(SW_HIDE);
	//m_btGame3.ShowWindow(SW_HIDE);
	

	//m_ServerList.ShowWindow(SW_SHOW);

///////////////////////////////////////////////////////////////////////


	//创建控件
	m_pHtmlBrower1 = new CHtmlBrower;
	m_pHtmlBrower1->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(320,511,751,535),this,20,NULL);
	m_pHtmlBrower1->EnableBrowser(true);
	m_pHtmlBrower1->Navigate("http://"ServerDomain"/Client/StatusNotice.asp");

	//设置控件
	m_ServerItemView.InitServerItemView(this);
	m_ServerItemView.ShowWindow(SW_HIDE);
	g_GlobalUnits.m_ServerListManager.InitServerListManager(&m_ServerItemView);

	::EnableWindow(GetParent()->m_hWnd,false);
	//登陆广场
	SendLogonMessage();

	//设置变量
	m_bInitDialog=true;

	theApp.m_MainPlaza = this->GetSafeHwnd();

	return TRUE;
}

//接口查询
void * __cdecl CPlazaViewItem::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(IPlazaViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(IClientSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IViewItem,Guid,dwQueryVer);
	return NULL;
}


//左键单击
void __cdecl CPlazaViewItem::OnTreeLeftClick(CListItem *pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)
{
	//效验参数
	if (pListItem==NULL) 
	{
		return;
	}

	//消息处理
	switch (pListItem->GetItemGenre())
	{
	case ItemGenre_Inside:
		{
			//变量定义
			CListInside * pListInside=(CListInside *)pListItem;
			tagGameInside * pGameInside=pListInside->GetItemInfo();

			//WebBrowse(TEXT("http://"ServerDomain),false);

			return;
		}
	case ItemGenre_Kind:		//游戏类型
		{
			//变量定义
			CListKind * pListKind=(CListKind *)pListItem;
			tagGameKind * pGameKind=pListKind->GetItemInfo();

			//连接规则
			//TCHAR szRuleUrl[256]=TEXT("");
			//_snprintf(szRuleUrl,sizeof(szRuleUrl),TEXT("http://"ServerDomain"/cn/game/GameRule.asp?KindID=%ld"),pGameKind->wKindID);
			//WebBrowse(szRuleUrl,true);

			return;
		}
	}

	return;
}

//右键单击
void __cdecl CPlazaViewItem::OnTreeRightClick(CListItem *pListItem, 
											  HTREEITEM hTreeItem, 
											  CTreeCtrl *pTreeCtrl)
{
	return;
}

//左键双击
void __cdecl CPlazaViewItem::OnTreeLeftDBClick(CListItem *pListItem, 
											   HTREEITEM hTreeItem, 
											   CTreeCtrl *pTreeCtrl)
{
	//效验参数
	if(pListItem==NULL) 
	{
		return ;
	}

	//消息处理
	switch (pListItem->GetItemGenre())
	{
	//游戏类型
	case ItemGenre_Kind:
		{
			CListKind *pListKind	= (CListKind *)pListItem;
			tagGameKind *pGameKind	= pListKind->GetItemInfo();

			//安装判断
			if (pListKind->m_bInstall==false)
			{
				//TCHAR szBuffer[512]=TEXT("");
				//_snprintf(szBuffer,sizeof(szBuffer),TEXT("【%s】还没有安装，现在是否下载？"),pGameKind->szKindName);
				//int nResult = ShowMessageBox(szBuffer,
				//						MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
				//if(nResult == IDYES)	
				{
					g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,
													pGameKind->wKindID,
													true);
				}
			}

			return;
		}

	//进程类型
	case ItemGenre_Process:	
		{
			CListProcess *pListProcess	= (CListProcess *)pListItem;
			tagGameProcess *pGameProcess= pListProcess->GetItemInfo();

			//构造命令行
			CString strCommonLine;
			tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
			strCommonLine.Format(TEXT("%s /UserToken:%ld#%s#%s /KindID:%ld /WndPlaza:%ld"),
							pGameProcess->szProcessName,
							UserData.dwUserID,
							UserData.szAccounts,
							UserData.szPassWord,
							pGameProcess->wKindID,
							m_hWnd);

			//启动游戏客户端
			STARTUPINFO			StartInfo;
			
			memset(&StartInfo,0,sizeof(StartInfo));
			memset(&g_GameProcessInfo,0,sizeof(g_GameProcessInfo));

			StartInfo.cb=sizeof(StartInfo);
			StartInfo.wShowWindow=SW_SHOWMAXIMIZED;
			BOOL bSuccess=CreateProcess(NULL,strCommonLine.GetBuffer(),NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&g_GameProcessInfo);
			strCommonLine.ReleaseBuffer();
			if(bSuccess == FALSE)
			{
				//下载询问
				//TCHAR szBuffer[512]=TEXT("");
				//_snprintf(szBuffer,sizeof(szBuffer),TEXT("【%s】还没有安装，现在是否下载？"),pGameProcess->szKindName);
				//int nResult=ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
				//if (nResult==IDYES)
					g_GlobalAttemper.DownLoadClient(pGameProcess->szKindName,pGameProcess->wKindID,true);
			}
			else
			{
				CloseHandle(g_GameProcessInfo.hThread);
				CloseHandle(g_GameProcessInfo.hProcess);
			}
			return;
		}

	//游戏房间
	case ItemGenre_Server:	
		{
			CListServer * pListServer=(CListServer *)pListItem;
			//bluker:进房间操作。。。只要有这个就OK了
			g_pControlBar->CreateRoomViewItem(pListServer);
			return;
		}
	}

	return;
}

//右键双击
void __cdecl CPlazaViewItem::OnTreeRightDBClick(CListItem *pListItem, 
												HTREEITEM hTreeItem, 
												CTreeCtrl *pTreeCtrl)
{
	return;
}

//选择改变
void __cdecl CPlazaViewItem::OnTreeSelectChanged(CListItem *pListItem, 
												 HTREEITEM hTreeItem, 
												 CTreeCtrl *pTreeCtrl)
{
	return;
}

//子项展开
void __cdecl CPlazaViewItem::OnTreeItemexpanded(CListItem *pListItem, 
												HTREEITEM hTreeItem, 
												CTreeCtrl * pTreeCtrl)
{
	return;
}

//网络连接消息
bool __cdecl CPlazaViewItem::OnSocketConnect(int iErrorCode, 
											LPCTSTR pszErrorDesc, 
											IClientSocket *pIClientSocke)
{
	//错误处理
	if (iErrorCode!=0)
	{
		g_GlobalAttemper.DestroyStatusWnd(this);
		ShowMessageBox(pszErrorDesc,MB_ICONINFORMATION);
		SendLogonMessage();
		return true;
	}

	//发送登录包
	if (m_bLogonPlaza==false)
	{
		m_DlgLogon.SendLogonPacket(pIClientSocke);
		g_GlobalAttemper.ShowStatusMessage(TEXT("正在验证用户登录信息..."),this);
	}

	return true;
}

//网络读取消息
bool __cdecl CPlazaViewItem::OnSocketRead(CMD_Command Command, 
										  void *pBuffer, 
										  WORD wDataSize, 
										  IClientSocket *pIClientSocke)
{
	switch (Command.wMainCmdID)
	{
	//登陆消息
	case MDM_GP_LOGON:				
		{
			return OnSocketMainLogon(Command,
								pBuffer,
								wDataSize,
								pIClientSocke);
		}

	//列表消息
	case MDM_GP_SERVER_LIST:		
		{
			return OnSocketMainServerList(Command,
								pBuffer,
								wDataSize,
								pIClientSocke);
		}

	//系统消息
	case MDM_GP_SYSTEM:				
		{
			return OnSocketMainSystem(Command,
								pBuffer,
								wDataSize,
								pIClientSocke);
		}
	}
	return true;
}

//网络关闭消息
bool __cdecl CPlazaViewItem::OnSocketClose(IClientSocket *pIClientSocke, 
										   bool bCloseByServer)
{
	if (m_bLogonPlaza==false)
	{
		g_GlobalAttemper.DestroyStatusWnd(this);
		if(pIClientSocke->GetRecvPacketCount()==0)
		{
			ShowMessageBox(TEXT("服务器繁忙，请选择其他服务器登陆！"),MB_ICONINFORMATION);
		}
		SendLogonMessage();
	}
	return true;
}

//登陆消息
bool CPlazaViewItem::OnSocketMainLogon(CMD_Command Command, 
									   void *pBuffer, 
									   WORD wDataSize, 
									   IClientSocket *pIClientSocke)
{
	ASSERT(Command.wMainCmdID==MDM_GP_LOGON);
	switch (Command.wSubCmdID)
	{
	//登陆成功
	case SUB_GP_LOGON_SUCCESS:		
		{
			//效验参数
			ASSERT(wDataSize>=sizeof(CMD_GP_LogonSuccess));
			if (wDataSize<sizeof(CMD_GP_LogonSuccess)) 
			{
				return false;
			}

			//保存信息
			tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
			CMD_GP_LogonSuccess * pLogonSuccess=(CMD_GP_LogonSuccess *)pBuffer;
			UserData.wFaceID=pLogonSuccess->wFaceID;
			UserData.cbGender=pLogonSuccess->cbGender;
			UserData.cbMember=pLogonSuccess->cbMember;
			UserData.dwUserID=pLogonSuccess->dwUserID;
			UserData.dwGroupID=pLogonSuccess->dwGroupID;
			UserData.dwExperience=pLogonSuccess->dwExperience;
			UserData.dwUserRight=pLogonSuccess->dwUserRight;
			UserData.dwMasterRight=pLogonSuccess->dwMasterRight;

			//扩展信息
			void * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pLogonSuccess+1,wDataSize-sizeof(CMD_GP_LogonSuccess));
			while (true)
			{
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_USER_ACCOUNTS:		//用户帐户
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize>0);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szAccounts));
						if (DataDescribe.wDataSize<=sizeof(UserData.szAccounts))
						{
							CopyMemory(UserData.szAccounts,pDataBuffer,DataDescribe.wDataSize);
							UserData.szAccounts[CountArray(UserData.szAccounts)-1]=0;
						}
						break;
					}
				case DTP_USER_PASS:			//用户密码
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize>0);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szPassWord));
						if (DataDescribe.wDataSize<=sizeof(UserData.szPassWord))
						{
							CopyMemory(UserData.szPassWord,pDataBuffer,DataDescribe.wDataSize);
							UserData.szPassWord[CountArray(UserData.szPassWord)-1]=0;
						}
						break;
					}
				case DTP_USER_GROUP_NAME:	//社团名字
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize>0);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szGroupName));
						if (DataDescribe.wDataSize<=sizeof(UserData.szGroupName))
						{
							CopyMemory(UserData.szGroupName,pDataBuffer,DataDescribe.wDataSize);
							UserData.szGroupName[CountArray(UserData.szGroupName)-1]=0;
						}
						break;
					}
				case DTP_STATION_PAGE:		//游戏主站
					{
						ASSERT(pDataBuffer!=NULL);
						if (pDataBuffer!=NULL) 
						{
							g_GlobalUnits.SetStationPage((LPCTSTR)pDataBuffer);
							////m_pHtmlBrower->Navigate(g_GlobalUnits.GetStationPage());
						}
						break;
					}
				case DTP_SESSIONID:		//游戏主站
					{
						ASSERT(pDataBuffer!=NULL);
						if (pDataBuffer!=NULL) 
						{
							memcpy(UserData.szSessionId,pDataBuffer,sizeof(UserData.szSessionId));
						}
						break;
					}
				default: { ASSERT(FALSE); }
				}
			}

			//设置提示
			g_GlobalAttemper.ShowStatusMessage(TEXT("正在读取服务器列表信息..."),this);

			return true;
		}

	//登陆失败
	case SUB_GP_LOGON_ERROR:		
		{
			//效验参数
			CMD_GP_LogonError *pLogonError = (CMD_GP_LogonError *)pBuffer;
			ASSERT(wDataSize>=(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe)));
			if (wDataSize<(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe))) return false;

			//关闭连接
			g_GlobalAttemper.DestroyStatusWnd(this);
			pIClientSocke->CloseSocket(false);

			//显示消息
			WORD wDescribeSize=wDataSize-(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe));
			if (wDescribeSize>0)
			{
				pLogonError->szErrorDescribe[wDescribeSize-1]=0;
				ShowMessageBox(pLogonError->szErrorDescribe,MB_ICONINFORMATION);
			}

			//发送登陆
			SendLogonMessage();

			return true;
		}

	//登陆完成
	case SUB_GP_LOGON_FINISH:
		{
			//关闭提示
			g_GlobalAttemper.DestroyStatusWnd(this);

			//展开类型
			INT_PTR nIndex=0;
			CListType * pListType=NULL;
			do
			{
				pListType=g_GlobalUnits.m_ServerListManager.EnumTypeItem(nIndex++);
				if (pListType==NULL) break;
				//bluker:返回:棋类,扑克类,麻将类,休闲类
				//tagGameType *gtype=pListType->GetItemInfo();
				//MessageBox(gtype->szTypeName);
				g_GlobalUnits.m_ServerListManager.ExpandListItem(pListType);
			} while (true);

			//展开列表
			nIndex=0;
			CListInside * pListInside=NULL;
			do
			{
				pListInside=g_GlobalUnits.m_ServerListManager.EnumInsideItem(nIndex++);
				if (pListInside==NULL) break;
				//bluker:返回:"逍遥岛休闲游戏中心 [ 0 人]"
				//tagGameInside *gInside=pListInside->GetItemInfo();
				//MessageBox(gInside->szDisplayName);
				g_GlobalUnits.m_ServerListManager.ExpandListItem(pListInside);
			} while (true);
			//记录信息
			m_bLogonPlaza=true;
			::EnableWindow(GetParent()->m_hWnd,true);
			::LockWindowUpdate(GetParent()->m_hWnd);

			//bluker:枚举可用的游戏
			nIndex=0;
			CListKind * pListKind=NULL;
			//ClearGameButtons();在改在注销中清空了

			////////////////////////////////////////////////////
			do
			{
				pListKind=g_GlobalUnits.m_ServerListManager.EnumKindItem(nIndex++);
				if (!pListKind) break;
				tagGameKind *pGameKind=pListKind->GetItemInfo();
				if(!pGameKind)break;

				if(g_GlobalUnits.m_ServerListManager.IsExistKindServerRunning(pGameKind->wKindID))
				{
					//bluker:动态加游戏按钮(已安装的游戏)
					tagGameButton *tagButton=new tagGameButton();
					char GameTitle[32];
					sprintf(GameTitle,"[%d]",pGameKind->dwOnLineCount);
					//if(!tagButton->Button.Create(/*GameTitle*/"正 常",WS_CHILD|WS_VISIBLE,CRect(-999,0,0,0),this,IDC_GAMEBUTTONS))
					//{
					//	SafeDelete(tagButton);
					//	break;
					//}
					//tagButton->Button.SetTitlePos(158,11);

					//if(!tagButton->Button.LoadRgnImage(AfxGetInstanceHandle(),GetGameIco(pGameKind->szKindName),RGB(255,0,255),0))
					///////if(!tagButton->Button.SetButtonImage(GetGameIco(pGameKind->szKindName),AfxGetInstanceHandle(),false))
					//{
					//	SafeDelete(tagButton);
					//	break;
					//}
					//else
					{
						tagButton->wKindID=pGameKind->wKindID;
						lstrcpyn(tagButton->szKindName,pGameKind->szKindName,CountArray(tagButton->szKindName));


						int nBtn = m_ServerList.AddButton(AfxGetInstanceHandle(),GetGameIco(pGameKind->szKindName),IDC_GAMEBUTTONS,"正常",//tagButton->szKindName,
				123,10,0x000000,tagButton->wKindID);

						tagImageListButton *Btn=m_ServerList.GetButtonInfo(nBtn);
						if(Btn)
						{
							CSkinImage tmp;
							tmp.SetLoadInfo(GetGameIco(pGameKind->szKindName),AfxGetInstanceHandle());
							CImageHandle ImgHandle(&Btn->img);
							CImageHandle ImgHandlet(&tmp);

							HDC hdc=Btn->img.GetDC();
							//擦掉Money
							tmp.Draw(hdc,CRect(37,0,164,30),CRect(37,0,164,30));

							//写新的Money
							char Money[32];
							sprintf(Money,"%s",pGameKind->szKindName);
							m_ServerList.DrawText(nBtn,77 - strlen(Money)*4+1,10+1,Money,0xffffff);
							m_ServerList.DrawText(nBtn,77 - strlen(Money)*4,10,Money,0x000000);

							//换LOGO
//							g_GlobalUnits.m_UserFaceRes->DrawSmallFace(CDC::FromHandle(hdc),5,6,User->wFaceID);
							Btn->img.ReleaseDC();
							m_ServerList.UpdateButton();
						}

						//CString strServer;
						//strServer.Format("%s    正常",pGameKind->szKindName);
						//m_ServerList.AddButton(AfxGetInstanceHandle(),GetGameIco(pGameKind->szKindName),IDC_GAMEBUTTONS,strServer,//tagButton->szKindName,
				//80,10,0x000000,tagButton->wKindID);
						
						//tagButton = m_ServerList.m_xButtons.back()->UserData
						m_GameButtons.push_back(tagButton);
						//m_GameButtons.push_back(tagButton);

						//游戏显示数量
						//if(m_GameButtons.size()==999)break;
					}
				}
			} while (true);

////////////////////////////////////////////////////
			CRect rc;
			GetClientRect(&rc);
			RectifyControl(rc.Width(),rc.Height());
			//m_ServerItemView.ShowWindow(SW_NORMAL);
			::LockWindowUpdate(NULL);
			::ShowWindow(GetParent()->m_hWnd,SW_NORMAL);
			::SetActiveWindow(GetParent()->m_hWnd); // hoho,这一句可是关键哦！！
			::SetForegroundWindow(GetParent()->m_hWnd);

			m_DlgLogon.OnLogonSuccess();
			//m_pHtmlBrower->EnableBrowser(true);


			m_ServerList.UpdateButton();
			m_ServerList.ShowWindow(SW_SHOW);

			g_GlobalOption.PlayMedia("Welcome.mp3");
			return true;
		}
	}

	return true;
}

//游戏列表消息
bool CPlazaViewItem::OnSocketMainServerList(CMD_Command Command, 
											void * pBuffer, 
											WORD wDataSize, 
											IClientSocket * pIClientSocke)
{
	ASSERT(Command.wMainCmdID==MDM_GP_SERVER_LIST);
	
	switch (Command.wSubCmdID)
	{
	//类型信息
	case SUB_GP_LIST_TYPE:			
		{
			//效验参数
			ASSERT(wDataSize%sizeof(tagGameType)==0);
			if (wDataSize%sizeof(tagGameType)!=0) return false;

			//处理消息
			tagGameType * pGameType=(tagGameType *)pBuffer;
			WORD wItemCount=wDataSize/sizeof(tagGameType);
			g_GlobalUnits.m_ServerListManager.InsertTypeItem(pGameType,wItemCount);

			return true;
		}

	//种类消息
	case SUB_GP_LIST_KIND:			
		{
			//效验参数
			ASSERT(wDataSize%sizeof(tagGameKind)==0);
			if (wDataSize%sizeof(tagGameKind)!=0) return false;

			//处理消息
			tagGameKind * pGameKind=(tagGameKind *)pBuffer;
			WORD wItemCount=wDataSize/sizeof(tagGameKind);
			g_GlobalUnits.m_ServerListManager.InsertKindItem(pGameKind,wItemCount);

			return true;
		}

	//进程信息
	case SUB_GP_LIST_PROCESS:		
		{
			//效验参数
			ASSERT(wDataSize%sizeof(tagGameProcess)==0);
			if (wDataSize%sizeof(tagGameProcess)!=0) return false;

			//处理消息
			tagGameProcess * pGameProcess=(tagGameProcess *)pBuffer;
			WORD wItemCount=wDataSize/sizeof(tagGameProcess);
			g_GlobalUnits.m_ServerListManager.InsertProcessItem(pGameProcess,wItemCount);

			return true;
		}

	//站点消息
	case SUB_GP_LIST_STATION:		
		{
			//效验参数
			ASSERT(wDataSize%sizeof(tagGameStation)==0);
			if (wDataSize%sizeof(tagGameStation)!=0) return false;

			//处理消息
			tagGameStation * pGameStation=(tagGameStation *)pBuffer;
			WORD wItemCount=wDataSize/sizeof(tagGameStation);
			g_GlobalUnits.m_ServerListManager.InsertStationItem(pGameStation,wItemCount);

			return true;
		}

	//服务器房间
	case SUB_GP_LIST_SERVER:		
		{
			//效验参数
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//处理消息
			tagGameServer * pGameServer=(tagGameServer *)pBuffer;
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			g_GlobalUnits.m_ServerListManager.InsertServerItem(pGameServer,wItemCount);

			return true;
		}

	//列表发送完成
	case SUB_GP_LIST_FINISH:		
		{
			//更新人数
			INT_PTR nIndex=0;
			DWORD dwAllOnLineCount=0L;
			CListKind * pListKind=NULL;
			do
			{
				pListKind=g_GlobalUnits.m_ServerListManager.EnumKindItem(nIndex++);
				if (pListKind==NULL) break;
				//bluker:返回:每个游戏房间的名称,id等信息
				//tagGameKind *gKind=pListKind->GetItemInfo();

				//------------------------------------------------(+)更新标题
				for(UINT i=0;i<m_GameButtons.size();i++)
				{
					if(pListKind->GetItemInfo()->wKindID==m_GameButtons[i]->wKindID)
					{
						char GameTitle[32];
						sprintf(GameTitle,"[%d]",pListKind->GetItemInfo()->dwOnLineCount);
						m_GameButtons[i]->Button.SetWindowText(/*GameTitle*/"正 常");
						break;
					}
				}
				//------------------------------------------------(-)更新标题

				dwAllOnLineCount+=pListKind->GetItemInfo()->dwOnLineCount;
			} while (true);
			g_GlobalUnits.m_ServerListManager.UpdateGameOnLineCount(dwAllOnLineCount);

			return true;
		}

	//列表配置
	case SUB_GP_LIST_CONFIG:		
		{
			//效验参数
			ASSERT(wDataSize%sizeof(CMD_GP_ListConfig)==0);
			if (wDataSize%sizeof(CMD_GP_ListConfig)!=0) return false;

			//处理消息
			CMD_GP_ListConfig * pListConfig=(CMD_GP_ListConfig *)pBuffer;
			m_ServerItemView.ShowOnLineCount(pListConfig->bShowOnLineCount?true:false);

			return true;
		}
	}

	return true;
}

//系统消息
bool CPlazaViewItem::OnSocketMainSystem(CMD_Command Command, 
										void * pBuffer, 
										WORD wDataSize, 
										IClientSocket * pIClientSocke)
{
	ASSERT(Command.wMainCmdID==MDM_GP_SYSTEM);
	
	switch (Command.wSubCmdID)
	{
	//版本信息
	case SUB_GP_VERSION:			
		{
			//效验参数
			ASSERT(wDataSize>=sizeof(CMD_GP_Version));
			if (wDataSize<sizeof(CMD_GP_Version)) return false;

			//消息处理
			CMD_GP_Version * pVersion=(CMD_GP_Version *)pBuffer;
			if (pVersion->bAllowConnect)
			{
				TCHAR szMessage[]=TEXT("游戏大厅版本已经升级，现在的版本还可以继续使用，现在是否马上下载新版本？");
				int iResult=::MessageBox(m_hWnd,szMessage,TEXT("游戏广场"),MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
				if (iResult==IDYES)
				{
					g_GlobalAttemper.DestroyStatusWnd(this);
					m_ClientSocket->CloseSocket(false);
					tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
					memset(&GlobalUserData,0,sizeof(GlobalUserData));
					g_GlobalAttemper.DownLoadClient(TEXT("游戏广场"),0,true);
				}
			}
			else
			{
				g_GlobalAttemper.DestroyStatusWnd(this);
				m_ClientSocket->CloseSocket(false);
				//TCHAR szMessage[]=TEXT("游戏大厅版本已经升级，现在的版本不可以继续使用，现在是否马上下载新版本？");
				//int iResult=::MessageBox(m_hWnd,szMessage,TEXT("游戏广场"),MB_ICONSTOP|MB_YESNO|MB_DEFBUTTON1);
				//if(iResult != IDYES)
				//{
				//	tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
				//	memset(&GlobalUserData,0,sizeof(GlobalUserData));
				//	AfxGetMainWnd()->PostMessage(WM_CLOSE);
				//}
				//else 
				{
					g_GlobalAttemper.DownLoadClient(TEXT("游戏广场"),0,true);
				}
			}

			return true;
		}
	}
	return true;
}

//登录消息
bool __cdecl CPlazaViewItem::SendLogonMessage()
{
	m_bLogonPlaza=false;
	PostMessage(WM_COMMAND,IDM_LOGON_PLAZA,0);
	return true;
}

//连接消息
bool __cdecl CPlazaViewItem::SendConnectMessage()
{
	PostMessage(WM_COMMAND,IDM_CONNECT_SERVER,0);
	return true;
}

/*
//显示网页
void CPlazaViewItem::WebBrowse(LPCTSTR lpszUrl, bool bForceBrowse)
{
	if (m_pHtmlBrower->GetSafeHwnd())
	{
		m_pHtmlBrower->Navigate(lpszUrl);
		if (bForceBrowse==true) 
		{
			m_pHtmlBrower->EnableBrowser(true);
		}
	}
	return;
}
*/

//显示消息
int CPlazaViewItem::ShowMessageBox(LPCTSTR pszMessage, UINT nType)
{
	int nResult = MessageBox(pszMessage,TEXT("游戏大厅"),nType);
	return nResult;
}

//位置消息
void CPlazaViewItem::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	//调整控件
	RectifyControl(cx,cy);
	//把新的样式赋给窗体
	return;
}

//显示消息
void CPlazaViewItem::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//设置标题
	if (bShow) 
	{
		UpdateTitleText();
	}

	return;
}

//绘画消息
void CPlazaViewItem::OnPaint()
{
	CPaintDC dc(this);
	//绘画界面

	DrawListFrame(&dc);

	CRect rcClient;
	GetClientRect(&rcClient);
	CImageHandle StatusLHandle(&m_StatusL);
	CImageHandle StatusMHandle(&m_StatusM);
	//m_StatusL.BitBlt(dc,275,rcClient.bottom-83);
	//m_StatusM.StretchBlt(dc,
	//		CRect(573,rcClient.bottom-83,rcClient.right,rcClient.bottom),
	//		CRect(0,0,m_StatusM.GetWidth(),m_StatusM.GetHeight())
	//		);
	m_StatusL.BitBlt(dc,267,rcClient.bottom-78);
	m_StatusM.BitBlt(dc,565,rcClient.bottom-78);
	//m_StatusM.StretchBlt(dc,
	//		CRect(573,rcClient.bottom-78,rcClient.right,rcClient.bottom),
	//		CRect(0,0,m_StatusM.GetWidth(),m_StatusM.GetHeight())
	//	);

	return;
}

//绘画背景
BOOL CPlazaViewItem::OnEraseBkgnd(CDC * pDC)
{
	//Invalidate(FALSE);
	//UpdateWindow();
	return TRUE;
}

//登录广场
void CPlazaViewItem::OnCommandLogon()
{
	//创建登陆框
	if (m_DlgLogon.m_hWnd==NULL) 
	{
		m_DlgLogon.Create(IDD_LOGON,this);
	}
/*
HRSRC   hrsrc=FindResource(0,(LPCSTR)IDD_LOGON,RT_DIALOG);   
HGLOBAL   hglobal=LoadResource(0,hrsrc);   
CDialog   dialog;   
dialog.InitModalIndirect(hglobal,this);
dialog.DoModal();
*/

	//显示登陆框
	m_DlgLogon.ShowWindow(SW_SHOW);
	m_DlgLogon.SetActiveWindow();
	m_DlgLogon.SetForegroundWindow();

	return;
}

//连接服务器
void CPlazaViewItem::OnCommandConnect()
{
	//创建组件
	if (m_ClientSocket.GetInterface()==NULL)
	{
		try
		{
			IUnknownEx * pIUnknownEx=(IUnknownEx *)QueryInterface(IID_IUnknownEx,VER_IUnknownEx);
			if (m_ClientSocket.CreateInstance()==false) 
			{
				throw TEXT("网络组件创建失败");
			}
			if (m_ClientSocket->SetSocketSink(pIUnknownEx)==false) 
			{
				throw TEXT("网络组件回调接口设置失败");
			}
		}
		catch (...)
		{
			ShowMessageBox(TEXT("网络组件创建失败，请重新下载游戏大厅！"),MB_ICONSTOP);
			PostMessage(WM_COMMAND,IDM_LOGON_PLAZA,0);
			return;
		}
	}

	//地址解释
	CRegKey RegServer;
	LPCTSTR pszServerIP="219.148.111.182"; //m_DlgLogon.GetLogonServer();
	TCHAR szRegServer[256]=TEXT(""),szServerAddr[64]=TEXT("");
	_snprintf(szRegServer,sizeof(szRegServer),TEXT("%s\\%s"),REG_LOGON_SERVER,pszServerIP);
	
	if (RegServer.Open(HKEY_CURRENT_USER,szRegServer,KEY_READ)==ERROR_SUCCESS)
	{
		TCHAR szReadData[1024]=TEXT("");
		DWORD dwReadData=0L,dwDataType=0L,dwDataSize=sizeof(szReadData);
		LONG lErrorCode=RegServer.QueryValue(TEXT("ServerAddr"),&dwDataType,szReadData,&dwDataSize);
		if (lErrorCode==ERROR_SUCCESS) 
		{
			CXOREncrypt::CrevasseData(szReadData,szServerAddr,sizeof(szServerAddr));
			pszServerIP=szServerAddr;
		}
	}

	//连接服务器
	try
	{
		//设置代理
		m_ClientSocket->SetProxyServer(m_DlgLogon.m_ProxyInfo);

		//连接服务器
		m_ClientSocket->CloseSocket(false);
		if (m_ClientSocket->Connect(pszServerIP,PORT_LOGON_SERVER)==false)
		{
			throw TEXT("服务器连接错误，可能是你的系统还没有成功连接上网络！");
		}
	}
	catch (LPCTSTR pszError)
	{
		ShowMessageBox(pszError,MB_ICONINFORMATION);
		SendLogonMessage();
		return;
	}

	//显示提示框
	CString strMessage=TEXT("正在连接服务器，请稍候...");
	g_GlobalAttemper.ShowStatusMessage(strMessage,this);
	
	return;
}

//取消连接
void CPlazaViewItem::OnCommandCancelConnect()
{
	g_GlobalAttemper.DestroyStatusWnd(this);
	m_ClientSocket->CloseSocket(false);
	SendLogonMessage();
	return;
}

//更新标题
void CPlazaViewItem::UpdateTitleText()
{
	#ifndef ADMIN_PLAZA
		AfxGetMainWnd()->SetWindowText(szProductName);
	#else
		AfxGetMainWnd()->SetWindowText(TEXT(szProductName"Admin"));
	#endif
	return;
}


//绘画列表
void CPlazaViewItem::DrawListFrame(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//加载资源
	HDC hDC=pDC->m_hDC;

	CImageHandle ImageImageBGMainHandle(&m_ImageBGMain);
	CImageHandle ImageImageBGMainHandle2(&m_ImageBGMain2);
	CImageHandle ImageMLHandle(&m_ImageML);
	CImageHandle ImageMRHandle(&m_ImageMR);
	//绘画
	//*
	if(rcClient.Width()<850){
		if(theApp.m_intTheme == 1)
		{
			m_ImageBGMain.BitBlt(hDC,0,0);
		}
		else
		{
			m_ImageBGMain2.BitBlt(hDC,0,0);
		}
	}
	else
	{
		SetStretchBltMode(hDC,STRETCH_HALFTONE);
		m_ImageBGMain.StretchBlt(hDC,CRect(rcClient.left+0,rcClient.top,rcClient.right-10,rcClient.bottom),CRect(0,0,m_ImageBGMain.GetWidth(),m_ImageBGMain.GetHeight()));
	}

	tagGlobalUserData UserData=g_GlobalUnits.GetGolbalUserData();
	if (UserData.dwUserID!=0)
	{
		if(!m_ServerList.IsWindowVisible())
			m_ServerList.ShowWindow(SW_SHOW);
		//CDC BufferDC;
		//CBitmap BufferBmp;
		//BufferBmp.LoadBitmap(ROOMLIST_T);

		//pDC->TransparentBlt(100,200,GAME_LIST_X,GAME_LIST_Y,&BufferDC,0,0,GAME_LIST_X+10,GAME_LIST_Y+20,(0xff00ff));
		//RGBQUAD m_rgbTable;
		//COLORREF m_Color(RGB(255,0,255));
		//m_rgbTable.rgbBlue = GetBValue(m_Color);
		//m_rgbTable.rgbGreen = GetGValue(m_Color);
		//m_rgbTable.rgbRed = GetRValue(m_Color);
	
		//m_GameListT.Draw2(hDC,-999,-999);
		CImageHandle HandleGameListT(&m_GameListT2);
		m_GameListT2.AlphaDrawImage(pDC,GAME_LIST_X,GAME_LIST_Y,RGB(255,0,255));
		//int cx=m_GameListT.GetWidth()+GAME_LIST_X;
		//int cy=m_GameListT.GetHeight()+GAME_LIST_Y;
		//m_GameListT.Stretch(hDC,CRect(GAME_LIST_X,GAME_LIST_Y,cx,cy));
		//m_GameListT.SetTransIndex(0);
		//m_GameListT.SetTransColor(m_rgbTable);
		//m_GameListL.Draw2(hDC,GAME_LIST_X,GAME_LIST_Y+49);
		//m_GameListR.Draw2(hDC,GAME_LIST_X+252,GAME_LIST_Y+49);
		//m_GameListM.Draw2(hDC,GAME_LIST_X+6,GAME_LIST_Y+49);
		CRect cr;
		GetClientRect(cr);

		CImageHandle HandleGameListB(&m_GameListB2);
		m_GameListB2.AlphaDrawImage(pDC,GAME_LIST_X,cr.bottom-88,RGB(255,0,255));

		
		m_btGame1.ShowWindow(SW_SHOW);
		m_btGame2.ShowWindow(SW_SHOW);
		m_btGame3.ShowWindow(SW_SHOW);

		m_btGame2.EnableWindow(false);
		m_btGame3.EnableWindow(false);
		//m_GameListB.Draw(hDC,GAME_LIST_X,cr.bottom-88);
		//m_GameListB.SetTransColor(m_rgbTable);
		//m_GameListB.AlphaStrip();
	//TransparentBlt(
		//m_GameListScroll.Draw2(hDC,GAME_LIST_X+252-13,GAME_LIST_Y+49);
		////绘画游戏列表框
		//m_GameListT.TransparentBlt(hDC,0,(0xff00ff),GAME_LIST_X,GAME_LIST_Y);
		//m_GameListL.TransparentBlt(hDC,0,(0xff00ff),GAME_LIST_X,GAME_LIST_Y+49);
		//m_GameListR.TransparentBlt(hDC,0,(0xff00ff),GAME_LIST_X+252,GAME_LIST_Y+49);
		//m_GameListM.TransparentBlt(hDC,40,(0xff00ff),GAME_LIST_X+6,GAME_LIST_Y+49);
		//m_GameListB.TransparentBlt(hDC,0,(0xff00ff),GAME_LIST_X,GAME_LIST_Y+352);
		//m_GameListScroll.TransparentBlt(hDC,0,(0xff00ff),GAME_LIST_X+252-17,GAME_LIST_Y+49);
	}


	/*char text[] =	"\\b0怎样获得小红人游戏币？\\par"
					"现开通以下三种充值方式：\\par\\par"
					"　1.您可以通过在网上商店购买到虚拟卡进行充值。[\\a1立即购买\\a0](9.5折)\\par"
					"　2.您可以用短信：移动发送111到1234567或联通发送111到7654321进行短信充值。\\par"
					"　3.您可以用固定电话：拨找1601234567进行充值。\\par";
	DWORD textAlignment = THA_LEFT | TVA_TOP;
	RECT margins = {2,2,2,2};
	pDC->SetBkMode(TRANSPARENT);
	g_TextRender.m_color=0x942525;//0x111197
	g_TextRender.SetFontFace("宋体",12);
	g_TextRender.EnhDrawText( pDC->m_hDC, text, strlen(text), &CRect(20,rcClient.bottom-109,250,rcClient.bottom-5), &margins, textAlignment );

*/

	/*m_ImageMsgBarT.AlphaDrawImage(CDC::FromHandle(hDC),
			8,rcClient.bottom-129,m_ImageMsgBarT.GetWidth(),m_ImageMsgBarT.GetHeight(),0,0,(COLORREF)0);
	m_ImageMsgBarB.AlphaDrawImage(CDC::FromHandle(hDC),
			8,rcClient.bottom-5,m_ImageMsgBarB.GetWidth(),m_ImageMsgBarB.GetHeight(),0,0,(COLORREF)0);
	m_ImageMsgBarL.BitBlt(hDC,8,rcClient.bottom-109);
	m_ImageMsgBarR.BitBlt(hDC,471,rcClient.bottom-109);*/



	//绘画左右的竖线
	m_ImageML.StretchBlt(hDC,
			CRect(0,0,m_ImageML.GetWidth(),rcClient.bottom),
			CRect(0,0,m_ImageML.GetWidth(),m_ImageML.GetHeight())
			);
	m_ImageMR.StretchBlt(hDC,
		CRect(rcClient.right-m_ImageMR.GetWidth(),0,rcClient.right,rcClient.bottom),
		CRect(0,0,m_ImageMR.GetWidth(),m_ImageMR.GetHeight())
			);

	//bluker:不显示用户名和头像
	
	//绘画信息
	//tagGlobalUserData UserData=g_GlobalUnits.GetGolbalUserData();
	UserData=g_GlobalUnits.GetGolbalUserData();

	if (UserData.dwUserID!=0)
	{
		CRect cr;
		GetClientRect(cr);
		//设置 DC
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(CSkinAttribute::m_DefaultFont);

		//用户名字
		TCHAR szBuffer[128];
		pDC->SetTextColor(RGB(255,255,255));
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s"),UserData.szAccounts);
		pDC->TextOut(50,cr.bottom-78,szBuffer,lstrlen(szBuffer));

		//用户号码
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("社团：%s"),UserData.szGroupName);
		//_snprintf(szBuffer,CountArray(szBuffer),TEXT("社团：未开放"),UserData.szGroupName);
		pDC->TextOut(50,cr.bottom-58,szBuffer,lstrlen(szBuffer));

		LPCTSTR pszMember[]={TEXT("普通会员"),TEXT("中级会员"),TEXT("高级会员")};
		BYTE cbMember=UserData.cbMember;
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s"),pszMember[cbMember]);
		pDC->TextOut(50,cr.bottom-38,szBuffer,lstrlen(szBuffer));
		//CRect re;

		//GetWindowRect(re);
		//CString aa;
		//aa.Format("%d,%d",re.right,re.bottom);
		//MessageBox(aa);
		//用户头像
		g_GlobalUnits.m_UserFaceRes->DrawBigFace(pDC,160,cr.bottom-82,UserData.wFaceID);
	}//

	return;
}

//调整控件
void CPlazaViewItem::RectifyControl(int nWidth, int nHeight)
{
	//状态判断
	if (m_bInitDialog==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;
	//移动控件
	HDWP hDwp=BeginDeferWindowPos(32);
	DeferWindowPos(hDwp,m_ServerItemView,NULL,22,40,260,460,uFlags);

	//DeferWindowPos(hDwp,m_pHtmlBrower->GetSafeHwnd(),NULL,117,nHeight-109,354,104,uFlags);

	tagGlobalUserData UserData=g_GlobalUnits.GetGolbalUserData();
	
	for(UINT i=0;i<m_GameButtons.size();i++)
	{
		if (UserData.dwUserID!=0)
		{
			m_GameButtons[i]->Button.ShowWindow(SW_NORMAL);
			DeferWindowPos(hDwp,m_GameButtons[i]->Button.m_hWnd,NULL,GAME_LIST_X+8,GAME_LIST_Y+52 + i * 35,0,0,uFlags|SWP_NOSIZE);
		}
		else
			m_GameButtons[i]->Button.ShowWindow(SW_HIDE);
			
	}

	m_ServerList.SetHeight(nHeight-165);//y从11减了77
	m_ServerList.SetWindowPos(NULL,11,77,0,0,SWP_NOSIZE);//y从6加了71
	m_ServerList.UpdateButton();

	DeferWindowPos(hDwp,m_btGame1,NULL,34,57,65,16,uFlags);
	DeferWindowPos(hDwp,m_btGame2,NULL,28+65,57,65,16,uFlags);
	DeferWindowPos(hDwp,m_btGame3,NULL,22+65*2,57,65,16,uFlags);

	EndDeferWindowPos(hDwp);
	Invalidate(FALSE);
	return;
}


//bluker:显示服务列表
void CPlazaViewItem::ShowServers(tagGameButton* pGameButton)
{
int nIndex=0;
CListServer * pListServer=NULL;

		m_RoomList.ClearButton();
	//枚举所有可用服务器
	while(true)
	{
		pListServer=g_GlobalUnits.m_ServerListManager.EnumServerItem(nIndex++);
		if(!pListServer)break;
		tagGameServer *pGameServer=pListServer->GetItemInfo();
		if(pGameButton->wKindID == pGameServer->wKindID)
		{
	//		AfxMessageBox("列出当前选择种类的游戏");
			//列出当前选择种类的游戏
			//pGameServer->wServerID,pGameButton->szKindName,pGameServer->szServerName;
			CString strServer;
			//strServer.Format("%s    %d",pGameServer->szServerName,pGameServer->dwOnLineCount);
			strServer.Format("%s",pGameServer->szServerName);
			int nBtn = m_RoomList.AddButton(AfxGetInstanceHandle(),ROOMLIST_BTN,IMG_LISTCTRL_CMD_ID,strServer,
				20,5,0xffffff,(pGameServer->wServerID <<16) | pGameServer->wKindID);

			tagImageListButton *Btn=m_RoomList.GetButtonInfo(nBtn);
			if(Btn)
			{
				CSkinImage tmp;
				tmp.SetLoadInfo(ROOMLIST_BTN,AfxGetInstanceHandle());
				CImageHandle ImgHandle(&Btn->img);
				CImageHandle ImgHandlet(&tmp);

				HDC hdc=Btn->img.GetDC();
				//擦掉Money
				tmp.Draw(hdc,CRect(110,0,164,30),CRect(110,0,164,30));

				//写新的Money
				char Money[32];
				sprintf(Money,"%d",pGameServer->dwOnLineCount);
				m_RoomList.DrawText(nBtn,130 - strlen(Money)*4+1,5+1,Money,0xffffff);
				m_RoomList.DrawText(nBtn,130 - strlen(Money)*4,5,Money,0x000000);

				Btn->img.ReleaseDC();
				m_RoomList.UpdateButton();
			}

		}
	}


CRect rc;
CRect rcView;
POINT pt;
	pGameButton->Button.GetWindowRect(rc);
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	//GetWindowRect(rcView);
	GetClientRect(&rcView);
	//if(pt.y > rcView.bottom - m_RoomList.GetHeight())
	//	pt.y = rcView.bottom - m_RoomList.GetHeight();
	if(rcView.bottom - pt.y  < m_RoomList.GetHeight())
		pt.y = rcView.bottom - m_RoomList.GetHeight();

	//CString aa;
	//aa.Format("%d,%d,%d",pt.y,rcView.bottom,m_RoomList.GetHeight());
	//MessageBox(aa);
	//aa.Format("%d,%d,%d",pt.y,rcView.bottom,m_RoomList.GetHeight());
	//MessageBox(aa);
//	ScreenToClient(rc);
	//m_RoomList.SetWindowPos(NULL,GAME_LIST_X + m_GameListT.GetWidth()+1,pt.y,0,0,SWP_NOSIZE);

	//m_ServerList.GetClientRect(&rcView);
	m_RoomList.SetWindowPos(NULL,m_ServerList.m_xTop.GetWidth() + m_GameListT.GetWidth()+20,pt.y,0,0,SWP_NOSIZE);

	m_RoomList.ShowWindow(SW_NORMAL);
	m_RoomList.UpdateButton();

}
void CPlazaViewItem::ClearGameButtons()
{
	for(UINT i=0;i<m_GameButtons.size();i++)
	{
		delete m_GameButtons[i];
	}
	m_GameButtons.clear();
}

//bluker:游戏种类图标按钮[点击事件]
afx_msg void CPlazaViewItem::OnBnClicked_GameButtons()
{/*
	CWnd *hwnd=this->GetFocus();
	for(UINT i=0;i<m_GameButtons.size();i++)
	{
		//if(hwnd->m_hWnd==m_GameButtons[i]->Button.m_hWnd)
		//if(hwnd == m_ServerList.GetWindow(i))
		ULONG nButton;
		tagImageListButton *BtnInfo;
		nButton=m_ServerList.GetLastButton();
		if(nButton)
		{
			BtnInfo=m_ServerList.GetButtonInfo(nButton);
			if(!BtnInfo)return;
			if(BtnInfo->UserData>0)
			{
			AfxMessageBox("asd2");
			ShowServers(m_GameButtons[i]);
			return;
			}
		}
	}*/
	ULONG nButton;
		tagImageListButton *BtnInfo;
		nButton=m_ServerList.GetLastButton();
		if(nButton)
		{
			BtnInfo=m_ServerList.GetButtonInfo(nButton);
			if(!BtnInfo)return;
			for(int i = 0; i < (int)m_GameButtons.size();i++)
			{
				if(BtnInfo->UserData == m_GameButtons[i]->wKindID)
				{
					ShowServers(m_GameButtons[i]);
				}
			}
		}
}

BOOL CPlazaViewItem::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(wParam == IMG_LISTCTRL_CMD_ID)
	{
		ULONG nButton;
		tagImageListButton *BtnInfo;

		nButton=m_RoomList.GetLastButton();
		if(nButton)
		{
			BtnInfo=m_RoomList.GetButtonInfo(nButton);
			if(!BtnInfo)return true;
			if(BtnInfo->UserData>0)
			{
				WORD wKindID = BtnInfo->UserData & 0xffff;
				WORD wServerID = BtnInfo->UserData>>16;

				if(wKindID==PRIZE_GAME_ID)
				{
					//构造命令行
					CString strCommonLine;
					CListServer * pListServer=g_GlobalUnits.m_ServerListManager.SearchServerItem(wKindID,wServerID);
					if(!pListServer)return true;

					tagGameServer *pGameServer = pListServer->GetItemInfo();
					tagGameKind * pGameKind = pListServer->GetListKind()->GetItemInfo();

					strCommonLine.Format(TEXT("%s /WndPlaza:%d /GameName:\"%s\" /sid:%s"),
						pGameKind->szProcessName,
						::AfxGetMainWnd()->m_hWnd,
						pGameServer->szServerName,
						g_GlobalUnits.GetGolbalUserData().szSessionId);

					CWinFileInfo WinFileInfo;
					if (WinFileInfo.OpenWinFile(pGameKind->szProcessName)==false)
					{
						g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
						return NULL;
					}
					DWORD dwFileVerMS=0L,dwFileVerLS=0L;
					WinFileInfo.GetFileVersion(dwFileVerMS,dwFileVerLS);//获取版本

					//版本分析
					BYTE cbFileVer1=(BYTE)(HIWORD(dwFileVerMS));
					BYTE cbFileVer2=(BYTE)(LOWORD(dwFileVerMS));
					BYTE cbFileVer3=(BYTE)(HIWORD(dwFileVerLS));
					BYTE cbFileVer4=(BYTE)(LOWORD(dwFileVerLS));
					BYTE cbListVer1=(BYTE)(LOWORD(LOWORD(pGameKind->dwMaxVersion)));
					BYTE cbListVer2=(BYTE)(HIBYTE(LOWORD(pGameKind->dwMaxVersion)));
					BYTE cbListVer3=(BYTE)(LOBYTE(HIWORD(pGameKind->dwMaxVersion)));
					BYTE cbListVer4=(BYTE)(HIBYTE(HIWORD(pGameKind->dwMaxVersion)));
					//判断版本
					if ((cbFileVer1!=cbListVer1)||(cbFileVer2!=cbListVer2)||(cbFileVer3!=cbListVer3)||(cbFileVer4!=cbListVer4))
					{
						g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
						return false;
					}

					//启动游戏客户端
					STARTUPINFO StartInfo;
					memset(&StartInfo,0,sizeof(StartInfo));
					StartInfo.cb=sizeof(StartInfo);
					StartInfo.wShowWindow=SW_SHOWMAXIMIZED;
					BOOL bSuccess=CreateProcess(NULL,strCommonLine.GetBuffer(),NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&g_GameProcessInfo);
					strCommonLine.ReleaseBuffer();
					if (bSuccess==FALSE)
					{
						g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
						memset(&g_GameProcessInfo,0,sizeof(g_GameProcessInfo));
					}
					else
					{
						AfxGetMainWnd()->ShowWindow(SW_HIDE);
					}
					return true;
				}
				else
				{
					g_pControlBar->CreateRoomViewItem(wKindID,wServerID);
					m_RoomList.ShowWindow(SW_HIDE);

					g_szGameTitle = TEXT(szProductName);
					CListKind * pListKind = g_GlobalUnits.m_ServerListManager.SearchKindItem(wKindID);
					tagGameKind *pGameKind=pListKind->GetItemInfo();
					g_szGameTitle += pGameKind->szKindName;
					g_szGameTitle += TEXT(" ");

					CListServer *pListServer=g_GlobalUnits.m_ServerListManager.SearchServerItem(wKindID,wServerID);
					tagGameServer *pGameServer=pListServer->GetItemInfo();
					g_szGameTitle += pGameServer->szServerName;
				}
			}
		}
		/*
		nButton=m_RoomList.GetLastButtonRT();
		if(nButton)
		{
			MessageBox("OK");
			return;
		}//*/
		return true;
	}
	return __super::OnCommand(wParam,lParam);
}



//-----------------------------------------------
//					the end
//-----------------------------------------------




