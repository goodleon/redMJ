#include "StdAfx.h"
#include "Resource.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//按钮标识

//游戏按钮
#define IDC_START						100								//开始按钮
#define IDC_OUT_CARD					101								//出牌按钮
#define IDC_OUT_PROMPT					102								//提示按钮
#define IDC_SEND_CONCEAL				103								//留底按钮

//功能按钮
#define IDC_CONCEAL_CARD				200								//查看底牌
#define IDC_REQUEST_LEAVE				201								//请求离开
#define IDC_LAST_TURN_CARD				202								//上轮扑克
#define IDC_TRUSTEE_CONTROL				203								//托管控制
#define IDC_RERANDCARD					204								//重新洗牌
#define IDC_OPENCONCEALCARD				205								//炒翻底牌
#define IDC_GIVEUP						206								//放弃炒底

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_OUT_PROMPT, OnOutPrompt)
	ON_BN_CLICKED(IDC_SEND_CONCEAL, OnSendConceal)
	ON_BN_CLICKED(IDC_CONCEAL_CARD, OnConcealCard)
	ON_BN_CLICKED(IDC_REQUEST_LEAVE, OnRuquestLeave)
	ON_BN_CLICKED(IDC_LAST_TURN_CARD, OnLastTurnCard)
	ON_BN_CLICKED(IDC_TRUSTEE_CONTROL, OnStusteeControl)
	ON_BN_CLICKED(IDC_RERANDCARD,OnReRandCard)
	ON_BN_CLICKED(IDC_OPENCONCEALCARD,OnOpenConcealCard)
	ON_BN_CLICKED(IDC_GIVEUP,OnGiveUp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView() : CGameFrameView()
{
	//显示变量
	m_bDeasilOrder=true;
	m_bWaitConceal=false;
	m_bLastTurnCard=false;
	//游戏变量
	m_wCardScore=0xFFFF;
	m_wBankerUser=INVALID_CHAIR;

	//叫牌信息
	m_cbCallCount=0;
	m_cbCallColor=COLOR_ERROR;
	m_cbMainValue=VALUE_ERROR;
	m_cbValueOrder[0]=VALUE_ERROR;
	m_cbValueOrder[1]=VALUE_ERROR;
	m_wCallCardUser=INVALID_CHAIR;
	m_wPutConcealUser = INVALID_CHAIR;

	//加载资源
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBanker.SetLoadInfo(IDB_BANKER,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageBackGround.SetLoadInfo(IDB_BACK_GROUND,hInstance);
	m_ImageColorFlag.SetLoadInfo(IDB_COLOR_FLAG,hInstance);
	m_ImageLastTurn.SetLoadInfo(IDB_LAST_TURN_TIP,hInstance);
	m_ImageWaitConceal.SetLoadInfo(IDB_WAIT_CONCEAL,hInstance);
	m_ImageStationBack.SetLoadInfo(IDB_STATION_BACK,hInstance);
	m_ImageStationTitle.SetLoadInfo(IDB_STATION_TITLE,hInstance);
	m_ImageHeadBg.SetLoadInfo(IDB_HEAD_BG,hInstance);
	m_ImageLeve1.SetLoadInfo(IDB_LEVEL1,hInstance);
	m_ImageLeve2.SetLoadInfo(IDB_LEVEL2,hInstance);
	m_ImageLeve3.SetLoadInfo(IDB_LEVEL3,hInstance);
	m_ImageLeve4.SetLoadInfo(IDB_LEVEL4,hInstance);
	m_ImageLeve5.SetLoadInfo(IDB_LEVEL5,hInstance);
	m_ImageLeve6.SetLoadInfo(IDB_LEVEL6,hInstance);


	//加载资源
	CImageHandle ImageHandleBanker(&m_ImageBanker);
	CImageHandle ImageHandleStationBack(&m_ImageStationBack);
	CImageHandle ImageHandleBackGround(&m_ImageBackGround);

	//获取大小
	m_SizeBanker.cx=m_ImageBanker.GetWidth();
	m_SizeBanker.cy=m_ImageBanker.GetHeight();
	m_SizeStation.cx=m_ImageStationBack.GetWidth();
	m_SizeStation.cy=m_ImageStationBack.GetHeight();

	return;
}

//析构函数
CGameClientView::~CGameClientView()
{
}

//建立消息
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//创建控件
	CRect rcCreate(0,0,0,0);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,10);
	m_CallCardWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,11);
	m_ConcealCardView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,12);

	//创建扑克
	for (WORD i=0;i<4;i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].SetCardSpace(DEF_CARD_H_SPACE,DEF_CARD_V_SPACE,0);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,20+i);
	}
	m_CardScore.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,50);
	m_HandCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,60);

	//设置扑克
	m_CardScore.SetDisplayFlag(true);
	m_CardScore.SetCardSpace(DEF_CARD_H_SPACE,0,0);
	m_HandCardControl.SetSinkWindow(AfxGetMainWnd());

	//创建按钮
	m_btStart.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_OUT_CARD);
	m_btOutPrompt.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_OUT_PROMPT);
	m_btSendConceal.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_SEND_CONCEAL);

	//功能按钮
	m_btConcealCard.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_CONCEAL_CARD);
	m_btRuquestLeave.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_REQUEST_LEAVE);
	m_btLastTurnCard.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_LAST_TURN_CARD);
	m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_TRUSTEE_CONTROL);
	m_btReRandCard.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_RERANDCARD);
	m_btOpenConcealCard.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_OPENCONCEALCARD);
	m_btGiveUp.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_GIVEUP);

	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btOutPrompt.SetButtonImage(IDB_OUT_PROMPT,hInstance,false);
	m_btSendConceal.SetButtonImage(IDB_SEND_CONCEAL,hInstance,false);

	//功能按钮
	m_btConcealCard.SetButtonImage(IDB_CONCEAL_CARD,hInstance,false);
	m_btRuquestLeave.SetButtonImage(IDB_REQUEST_LEAVE,hInstance,false);
	m_btLastTurnCard.SetButtonImage(IDB_LAST_TURN_CARD,hInstance,false);
	m_btStusteeControl.SetButtonImage(IDB_START_TRUSTEE,hInstance,false);
	m_btReRandCard.SetButtonImage(IDB_RESOLUTION,hInstance,false);
	m_btOpenConcealCard.SetButtonImage(IDB_OPEN_BOTTOM,hInstance,false);
	m_btGiveUp.SetButtonImage(IDB_GIVE_UP,hInstance,false);

	//建立提示
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btConcealCard,TEXT("查看底牌"));
	m_ToolTipCtrl.AddTool(&m_btRuquestLeave,TEXT("查看底牌"));
	m_ToolTipCtrl.AddTool(&m_btLastTurnCard,TEXT("显示上一轮出牌"));
	m_ToolTipCtrl.AddTool(&m_btStusteeControl,TEXT("开始（停止）托管"));

	//读取配置
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;


	m_btReRandCard.ShowWindow(SW_HIDE);
	m_btOpenConcealCard.ShowWindow(SW_HIDE);
	m_btGiveUp.ShowWindow(SW_HIDE);
	m_btReRandCard.EnableWindow(FALSE);	
	m_btOpenConcealCard.EnableWindow(FALSE);
	m_btGiveUp.EnableWindow(FALSE);


	return 0;
}

//消息解释
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//重置界面
void CGameClientView::ResetGameView()
{
	//显示变量
	m_bWaitConceal=false;
	m_bLastTurnCard=false;

	//游戏变量
	m_wCardScore=0xFFFF;
	m_wBankerUser=INVALID_CHAIR;

	//叫牌信息
	m_cbCallCount=0;
	m_cbCallColor=COLOR_ERROR;
	m_cbMainValue=VALUE_ERROR;
	m_cbValueOrder[0]=VALUE_ERROR;
	m_cbValueOrder[1]=VALUE_ERROR;
	m_wCallCardUser=INVALID_CHAIR;
	m_wPutConcealUser = INVALID_CHAIR;

	//隐藏控件
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btOutPrompt.ShowWindow(SW_HIDE);
	m_btSendConceal.ShowWindow(SW_HIDE);


	//禁用按钮
	m_btConcealCard.EnableWindow(FALSE);
	m_btRuquestLeave.EnableWindow(FALSE);
	m_btLastTurnCard.EnableWindow(FALSE);
	m_btStusteeControl.EnableWindow(FALSE);
	m_btStusteeControl.SetButtonImage(IDB_START_TRUSTEE,AfxGetInstanceHandle(),false);	
	m_btReRandCard.ShowWindow(SW_HIDE);
	m_btOpenConcealCard.ShowWindow(SW_HIDE);
	m_btGiveUp.ShowWindow(SW_HIDE);
	m_btReRandCard.EnableWindow(FALSE);	
	m_btOpenConcealCard.EnableWindow(FALSE);
	m_btGiveUp.EnableWindow(FALSE);
	

	//设置扑克
	m_CardScore.SetCardData(NULL,0);
	m_HandCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayFlag(false);
	for (WORD i=0;i<CountArray(m_UserCardControl);i++) m_UserCardControl[i].SetCardData(NULL,0);

	//控件组件
	m_ScoreView.ShowWindow(SW_HIDE);
	m_CallCardWnd.ShowWindow(SW_HIDE);
	m_ConcealCardView.ShowWindow(SW_HIDE);

	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//变量定义
	int nViewHeight=nHeight-m_SizeStation.cy-50;

	//设置坐标
	if (m_bDeasilOrder==false)
	{
		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-145;
		m_ptFace[3].y=nHeight/2-m_nYFace-190;
		m_ptName[3].x=nWidth-m_nXBorder-m_nXFace-74;
		m_ptName[3].y=nHeight/2+m_nYFace-257;
		m_ptTimer[3].x=nWidth-183;
		m_ptTimer[3].y=171;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-150;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-30;
		m_PointBanker[3].x=nWidth-m_nXBorder-m_SizeBanker.cx-100;
		m_PointBanker[3].y=nHeight/2-118;

		m_ptFace[1].x=9;
		m_ptFace[1].y=nHeight/2-m_nYFace-190;
		m_ptName[1].x=85;
		m_ptName[1].y=nHeight/2+m_nYFace-257;		
		m_ptTimer[1].x=28;
		m_ptTimer[1].y=171;
		m_ptReady[1].x=m_nXBorder+5+145+m_nXTimer;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-30;
		m_PointBanker[1].x=m_nXBorder+50;
		m_PointBanker[1].y=nHeight/2-120;
	}
	else
	{
		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-145;
		m_ptFace[3].y=nHeight/2-m_nYFace-190;
		m_ptName[3].x=nWidth-m_nXBorder-m_nXFace-74;
		m_ptName[3].y=nHeight/2+m_nYFace-257;
		m_ptTimer[3].x=nWidth-183;
		m_ptTimer[3].y=171;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-150;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-30;
		m_PointBanker[3].x=nWidth-m_nXBorder-m_SizeBanker.cx-100;
		m_PointBanker[3].y=nHeight/2-118;

		m_ptFace[1].x=9;
		m_ptFace[1].y=nHeight/2-m_nYFace-190;
		m_ptName[1].x=85;
		m_ptName[1].y=nHeight/2+m_nYFace-257;		
		m_ptTimer[1].x=28;
		m_ptTimer[1].y=171;
		m_ptReady[1].x=m_nXBorder+5+145+m_nXTimer;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-30;
		m_PointBanker[1].x=m_nXBorder+50;
		m_PointBanker[1].y=nHeight/2-120;
	}

	m_ptFace[0].x=nWidth/2-m_nXFace/2-70;
	m_ptFace[0].y=m_nYBorder+40;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2-35;
	m_ptName[0].y=m_nYBorder+37;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[0].y=m_nYBorder+140;
	m_ptReady[0].x=nWidth/2-m_nXTimer-m_nXFace/2+38;
	m_ptReady[0].y=m_nYBorder+m_nYTimer/2+150;
	m_PointBanker[0].x=nWidth/2-m_nXFace/2-m_SizeBanker.cx-m_nXTimer*2-10;
	m_PointBanker[0].y=m_nYBorder+140;

	m_ptFace[2].x=9;
	m_ptFace[2].y=nHeight-92;
	m_ptName[2].x=85;
	m_ptName[2].y=nHeight-95;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer-5;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2+30;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-110;
	m_PointBanker[2].x=m_nXBorder+20;
	m_PointBanker[2].y=nHeight-m_nXBorder-m_SizeBanker.cy-100;

	POINT *pChatPt = GetChatPt();
	pChatPt[SCP_B].x =m_ptName[2].x-25;
	pChatPt[SCP_B].y =m_ptName[2].y-80;

	pChatPt[SCP_L].x =m_ptName[1].x-25;
	pChatPt[SCP_L].y =m_ptName[1].y+80;

	pChatPt[SCP_R].x =m_ptName[3].x-195;
	pChatPt[SCP_R].y =m_ptName[3].y+80;

	pChatPt[SCP_T].x =m_ptName[0].x-60;
	pChatPt[SCP_T].y =m_ptName[0].y+80;

	//移动按钮
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//开始按钮
	m_btStart.GetWindowRect(&rcButton);
	//DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nViewHeight-225,0,0,uFlags);
	DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2,nViewHeight-rcButton.Height()-45,0,0,uFlags);

	//留底按钮
	m_btSendConceal.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btSendConceal,NULL,(nWidth-rcButton.Width())/2,nViewHeight-225,0,0,uFlags);

	//出牌按钮
	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2+10,nViewHeight-225,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2-rcButton.Width()-10,nViewHeight-225,0,0,uFlags);

	//功能按钮
	m_btConcealCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nViewHeight-5-rcButton.Height()-20,0,0,uFlags);
	DeferWindowPos(hDwp,m_btRuquestLeave,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*2,nViewHeight-5-rcButton.Height()-20,0,0,uFlags);
	DeferWindowPos(hDwp,m_btConcealCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*3,nViewHeight-5-rcButton.Height()-20,0,0,uFlags);
	DeferWindowPos(hDwp,m_btLastTurnCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*4,nViewHeight-5-rcButton.Height()-20,0,0,uFlags);
	DeferWindowPos(hDwp,m_btReRandCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*4+20,nViewHeight-5-rcButton.Height()-90,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOpenConcealCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*4+20,nViewHeight-5-rcButton.Height()-70,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUp,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*4+20,nViewHeight-5-rcButton.Height()-50,0,0,uFlags);

	//移动控件
	EndDeferWindowPos(hDwp);

	//移动扑克
	m_CardScore.SetBenchmarkPos(m_nXBorder+200,nViewHeight+5,enXLeft,enYTop);
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+95,enXCenter,enYTop);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nViewHeight-170,enXCenter,enYBottom);
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nViewHeight-m_nYFace-m_nYBorder+30,enXCenter,enYBottom);

	//用户扑克
	if (m_bDeasilOrder==true)
	{
		m_UserCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-80,nViewHeight/2,enXRight,enYCenter);
		m_UserCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+80,nViewHeight/2,enXLeft,enYCenter);
	}
	else 
	{
		m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-80,nViewHeight/2,enXRight,enYCenter);
		m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+80,nViewHeight/2,enXLeft,enYCenter);
	}

	//叫牌窗口
	CRect rcCallCard;
	m_CallCardWnd.GetWindowRect(&rcCallCard);
	m_CallCardWnd.SetWindowPos(NULL,nWidth/2+20,nViewHeight-m_nYBorder-rcCallCard.Height()-162,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//积分视图
	CRect rcScore;
	m_ScoreView.GetWindowRect(&rcScore);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcScore.Width())/2,nViewHeight/2-160,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//底牌窗口
	CRect rcConceal;
	m_ConcealCardView.GetWindowRect(&rcConceal);
	m_ConcealCardView.SetWindowPos(NULL,(nWidth-rcConceal.Width())/2,nViewHeight/2-160,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

void CGameClientView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//if (GetSystemMetrics(SM_CXSCREEN)>800)
	//::SendMessage(GetParent()->GetSafeHwnd(),WM_NCLBUTTONDOWN, HTCAPTION,0);
	//CGameFrameView::OnLButtonDown(nFlags, point);
	if(point.y<48 && GetSystemMetrics(SM_CXSCREEN)>800)
	{
		::SendMessage(GetParent()->GetSafeHwnd(),WM_NCLBUTTONDOWN, HTCAPTION,0);
	}
	CGameFrameView::OnLButtonDown(nFlags, point);
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//绘画背景
	CImageHandle CHImageBackGround(&m_ImageBackGround);
	//DrawViewImage(pDC,m_ImageBackGround,enMode_Spread);
	SetStretchBltMode(pDC->m_hDC,STRETCH_HALFTONE);
	m_ImageBackGround.StretchBlt(pDC->m_hDC,CRect(0,0,nWidth,nHeight),CRect(0,0,m_ImageBackGround.GetWidth(),m_ImageBackGround.GetHeight()));

	//DrawViewImage(pDC,m_ImageBack,enMode_Centent);
	BYTE MOVEPOS=24;

	CImageHandle HandleHeadBg(&m_ImageHeadBg);

	//绘画用户
	TCHAR szBuffer[32];
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wUserTimer=GetUserTimer(i);
		const tagUserData * pUserData=GetUserInfo(i);

		//绘画用户
		if (pUserData!=NULL)
		{
			m_ImageHeadBg.AlphaDrawImage(pDC,m_ptFace[i].x-3,m_ptFace[i].y-15,RGB(255,0,255));
			
			//用户名字
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));
			pDC->SetTextAlign(((i==0)&&(m_bDeasilOrder==true))||((i==10)&&(m_bDeasilOrder==false))?TA_CENTER:TA_LEFT);
			//pDC->TextOut(m_ptName[i].x+77,m_ptName[i].y+1,pUserData->szName,lstrlen(pUserData->szName));
			
			LPCTSTR pszMember[]={TEXT("普通会员"),TEXT("中级会员"),TEXT("高级会员")};
			BYTE cbMember=pUserData->cbMember;
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s(%s)"),pUserData->szName,pszMember[cbMember]);

			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,szBuffer,lstrlen(szBuffer));

			//社团
			pDC->SetTextColor((wUserTimer>0)?RGB(255,255,255):RGB(255,255,255));
			pDC->SetTextAlign(((i==0)&&(m_bDeasilOrder==true))||((i==10)&&(m_bDeasilOrder==false))?TA_CENTER:TA_LEFT);
			//pDC->TextOut(m_ptName[i].x+77,m_ptName[i].y+1,pUserData->szName,lstrlen(pUserData->szName));
			
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("社团：%s"),pUserData->szGroupName);


			pDC->TextOut(m_ptName[i].x,m_ptName[i].y+40,szBuffer,lstrlen(szBuffer));
			


			//用户金币（投影效果）
			pDC->SetTextColor((wUserTimer>0)?RGB(0,0,0):RGB(0,0,0));
			pDC->SetTextAlign(((i==0)&&(m_bDeasilOrder==true))||((i==10)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
			CString s;
			s.Format("游戏币:%d",pUserData->lScore);

			if(i!=2)
			{
				pDC->TextOut(m_ptName[i].x,m_ptName[i].y+20,s,lstrlen(s));
			}
			else
			{
				pDC->TextOut(m_ptName[i].x,m_ptName[i].y+20,s,lstrlen(s));
			}

			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(255,255,0));
			pDC->SetTextAlign(((i==0)&&(m_bDeasilOrder==true))||((i==10)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
	
			if(i!=2)
			{
				pDC->TextOut(m_ptName[i].x,m_ptName[i].y+21,s,lstrlen(s));
			}
			else
			{
				pDC->TextOut(m_ptName[i].x,m_ptName[i].y+21,s,lstrlen(s));
			}


			//用户星星等级
			if (pUserData->lExperience<=500L) 
			{
				CImageHandle ImageHandleLevel(&m_ImageLeve1);
				if(i!=2)
				{
					m_ImageLeve1.AlphaDrawImage(pDC,m_ptName[i].x,m_ptName[i].y+67,RGB(255,0,255));
				}
				else
				{
					m_ImageLeve1.AlphaDrawImage(pDC,m_ptName[i].x,m_ptName[i].y+67,RGB(255,0,255));
				}
			}
			if ((pUserData->lExperience>500L)&&(pUserData->lExperience<=1500L)) 
			{

				CImageHandle ImageHandleLevel(&m_ImageLeve2);
				if(i!=2)
				{
					m_ImageLeve2.AlphaDrawImage(pDC,m_ptName[i].x,m_ptName[i].y+67,RGB(255,0,255));
				}
				else
				{
					m_ImageLeve2.AlphaDrawImage(pDC,m_ptName[i].x,m_ptName[i].y+67,RGB(255,0,255));
				}
			}
			if ((pUserData->lExperience>1500L)&&(pUserData->lExperience<=4000L)) 
			{
				CImageHandle ImageHandleLevel(&m_ImageLeve3);
				if(i!=2)
				{
					m_ImageLeve3.AlphaDrawImage(pDC,m_ptName[i].x,m_ptName[i].y+67,RGB(255,0,255));
				}
				else
				{
					m_ImageLeve3.AlphaDrawImage(pDC,m_ptName[i].x,m_ptName[i].y+67,RGB(255,0,255));
				}
			}
			if ((pUserData->lExperience>4000L)&&(pUserData->lExperience<=10000L))
			{
				CImageHandle ImageHandleLevel(&m_ImageLeve4);
				if(i!=2)
				{
					m_ImageLeve4.AlphaDrawImage(pDC,m_ptName[i].x,m_ptName[i].y+67,RGB(255,0,255));
				}
				else
				{
					m_ImageLeve4.AlphaDrawImage(pDC,m_ptName[i].x,m_ptName[i].y+67,RGB(255,0,255));
				}
			}
			if ((pUserData->lExperience>10000L)&&(pUserData->lExperience<=20000L)) 
			{
				CImageHandle ImageHandleLevel(&m_ImageLeve5);
				if(i!=2)
				{
					m_ImageLeve5.AlphaDrawImage(pDC,m_ptName[i].x,m_ptName[i].y+67,RGB(255,0,255));
				}
				else
				{
					m_ImageLeve5.AlphaDrawImage(pDC,m_ptName[i].x,m_ptName[i].y+67,RGB(255,0,255));
				}
			}
			if (pUserData->lExperience>20000L)
			{
				CImageHandle ImageHandleLevel(&m_ImageLeve6);
				if(i!=2)
				{
					m_ImageLeve6.AlphaDrawImage(pDC,m_ptName[i].x,m_ptName[i].y+67,RGB(255,0,255));
				}
				else
				{
					m_ImageLeve6.AlphaDrawImage(pDC,m_ptName[i].x,m_ptName[i].y+67,RGB(255,0,255));
				}
			}


			//其他信息
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			//DrawUserFace(pDC,pUserData->wFaceID,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
			DrawUserBigFace(pDC,pUserData->wFaceID,m_ptFace[i].x+7,m_ptFace[i].y-4,pUserData->cbUserStatus==US_OFFLINE);
		}
	}




	//状态界面
	//CImageHandle ImageHandleStationBack(&m_ImageStationBack);
	//CImageHandle ImageHandleStationTitle(&m_ImageStationTitle);
	//for (int nXPos=0;nXPos<nWidth;nXPos+=m_ImageStationBack.GetWidth())
	//{
	//	m_ImageStationBack.BitBlt(pDC->m_hDC,nXPos,nHeight-m_ImageStationBack.GetHeight()-m_nYBorder);
	//}
	//m_ImageStationTitle.BitBlt(pDC->m_hDC,m_nXBorder,nHeight-m_ImageStationTitle.GetHeight()-m_nYBorder);

	//主牌信息
	if (m_cbMainValue!=VALUE_ERROR)
	{
		//变量定义
		LPCTSTR szValue[]=
		{
			TEXT("A"),TEXT("2"),TEXT("3"),TEXT("4"),TEXT("5"),
			TEXT("6"),TEXT("7"),TEXT("8"),TEXT("9"),TEXT("10"),
			TEXT("J"),TEXT("Q"),TEXT("K")
		};

		//构造字符
		TCHAR szString[32]=TEXT("");
		_snprintf(szString,CountArray(szString),TEXT("现在打:%s"),szValue[m_cbMainValue-1]);

		//输出信息
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetTextColor(RGB(240,240,240));
		//DrawTextString(pDC,szString,RGB(240,240,240),RGB(50,50,50),m_nXBorder+65,nHeight-m_nYBorder-m_SizeStation.cy-3+MOVEPOS);
		DrawTextString(pDC,szString,RGB(240,240,240),RGB(50,50,50),190,502);

		//绘画花色
		if (m_cbCallColor!=COLOR_ERROR)
		{
			//加载资源
			CImageHandle HandleColorFlag(&m_ImageColorFlag);
			int nColorImageWidth=m_ImageColorFlag.GetWidth()/6;
			int nColorImageHeight=m_ImageColorFlag.GetHeight();

			//绘画标志
			for (BYTE i=0;i<m_cbCallCount;i++) 
			{
				//int nXPos=m_nXBorder+85+i*(nColorImageWidth+4);
				//int nYPos=nHeight-m_nYBorder-m_SizeStation.cy-5+MOVEPOS;
				int nXPos=250+i*(nColorImageWidth+4);
				int nYPos=502;
				m_ImageColorFlag.AlphaDrawImage(pDC,nXPos,nYPos,nColorImageWidth,nColorImageHeight,
					nColorImageWidth*(m_cbCallColor>>4),0,RGB(255,0,255));

			}
		}
		else
		{
			lstrcpyn(szString,TEXT("[ 花色未定 ]"),CountArray(szString));
			//DrawTextString(pDC,szString,RGB(240,240,240),RGB(50,50,50),m_nXBorder+85,nHeight-m_nYBorder-m_SizeStation.cy-3+MOVEPOS);
			DrawTextString(pDC,szString,RGB(240,240,240),RGB(50,50,50),250,502);
		}
	}

	//升级信息
/*	if ((m_cbValueOrder[0]!=VALUE_ERROR)&&(m_cbValueOrder[1]!=VALUE_ERROR))
	{
		TCHAR szString[32]=TEXT("");
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetTextColor(RGB(240,240,240));
		_snprintf(szString,CountArray(szString),TEXT("自己 %ld：对家 %ld"),m_cbValueOrder[0],m_cbValueOrder[1]);
		DrawTextString(pDC,szString,RGB(240,240,240),RGB(50,50,50),m_nXBorder+65,nHeight-m_nYBorder-m_SizeStation.cy+14);
	}*/


	

	//庄家信息
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		const tagUserData * pUserData=GetUserInfo(m_wBankerUser);
		if (pUserData!=NULL)
		{
			//庄家名字
			pDC->SetTextAlign(TA_LEFT|TA_TOP);
			TCHAR szString[32]=TEXT("");
			//DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_nXBorder+65,nHeight-m_nYBorder-m_SizeStation.cy+20+MOVEPOS);
			_snprintf(szString,CountArray(szString),TEXT("庄家名字:%s"),pUserData->szName);
			//DrawTextString(pDC,szString,RGB(240,240,240),RGB(50,50,50),420,502);
			
			//庄家标志
			CImageHandle ImageHandleBanker(&m_ImageBanker);
			m_ImageBanker.AlphaDrawImage(pDC,m_PointBanker[m_wBankerUser].x,m_PointBanker[m_wBankerUser].y,m_SizeBanker.cx,m_SizeBanker.cy,0,0,RGB(255,0,255));
		}
	}

	//叫牌用户
	if (m_wCallCardUser!=INVALID_CHAIR)
	{
		//获取用户
		const tagUserData * pUserData=GetUserInfo(m_wCallCardUser);
		if (pUserData!=NULL)
		{
			pDC->SetTextAlign(TA_LEFT|TA_TOP);
			TCHAR szString[32]=TEXT("");
			//DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_nXBorder+65,nHeight-m_nYBorder-m_SizeStation.cy+42+MOVEPOS);
			_snprintf(szString,CountArray(szString),TEXT("叫牌用户:%s"),pUserData->szName);
			DrawTextString(pDC,szString,RGB(240,240,240),RGB(50,50,50),390,502);

		}
	}
	
	//埋底用户
	if(m_wPutConcealUser!=INVALID_CHAIR)
	{
		//获取用户
		const tagUserData * pUserData=GetUserInfo(m_wPutConcealUser);
		if(pUserData!=NULL)
		{
			pDC->SetTextAlign(TA_LEFT|TA_TOP);
			//DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_nXBorder+65,nHeight-m_nYBorder-m_SizeStation.cy+64+MOVEPOS);
			//DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_nXBorder+65,502);
		}
	}

	//游戏得分
	if (m_wCardScore!=0xFFFF)
	{
		TCHAR szCardScore[16]=TEXT("");
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetTextColor(RGB(240,240,240));
		_snprintf(szCardScore,CountArray(szCardScore),TEXT("游戏得分:%ld"),m_wCardScore);
		//DrawTextString(pDC,szCardScore,RGB(240,240,240),RGB(50,50,50),m_nXBorder+65,nHeight-m_nYBorder-m_SizeStation.cy+86+MOVEPOS);'
		//DrawTextString(pDC,szCardScore,RGB(240,240,240),RGB(50,50,50),m_nXBorder+65,502);
		DrawTextString(pDC,szCardScore,RGB(240,240,240),RGB(50,50,50),320,502);
	}

	//底牌标志
	if (m_bWaitConceal==true)
	{
		//加载资源
		CImageHandle HandleWaitConceal(&m_ImageWaitConceal);

		//绘画标志
		int nImageWidth=m_ImageWaitConceal.GetWidth();
		int nImageHegith=m_ImageWaitConceal.GetHeight();
		m_ImageWaitConceal.AlphaDrawImage(pDC,(nWidth-nImageWidth)/2,(nHeight-m_SizeStation.cy-nImageHegith)/2-50,RGB(255,0,255));
	}

	//上轮标志
	if (m_bLastTurnCard==true)
	{
		//加载资源
		CImageHandle HandleLastTurn(&m_ImageLastTurn);

		//绘画标志
		int nImageWidth=m_ImageLastTurn.GetWidth();
		int nImageHegith=m_ImageLastTurn.GetHeight();
		m_ImageLastTurn.AlphaDrawImage(pDC,(nWidth-nImageWidth)/2,(nHeight-m_SizeStation.cy-nImageHegith)/2-65,RGB(255,0,255));
	}

	return;
}

//用户顺序
void CGameClientView::SetUserOrder(bool bDeasilOrder)
{
	//状态判断
	if (m_bDeasilOrder==bDeasilOrder) return;

	//设置变量
	m_bDeasilOrder=bDeasilOrder;
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);

	//设置界面
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyGameView(rcClient.Width(),rcClient.Height());

	//刷新界面
	UpdateGameView(NULL);

	return;
}

//设置得分
void CGameClientView::SetCardScore(WORD wCardScore)
{
	//设置用户
	if (wCardScore!=m_wCardScore)
	{
		//设置变量
		m_wCardScore=wCardScore;

		//更新界面
		UpdateGameView(NULL);
	}

	return;
}

//庄家用户
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	//设置用户
	if (wBankerUser!=m_wBankerUser)
	{
		//设置变量
		m_wBankerUser=wBankerUser;

		//更新界面
		UpdateGameView(NULL);
	}

	return;
}

//设置提示
void CGameClientView::SetWaitConceal(bool bWaitConceal)
{
	//设置变量
	if (bWaitConceal!=m_bWaitConceal)
	{
		//设置变量
		m_bWaitConceal=bWaitConceal;

		//更新界面
		UpdateGameView(NULL);
	}

	return;
}

//设置上轮
void CGameClientView::SetLastTurnCard(bool bLastTurnCard)
{
	//设置变量
	if (bLastTurnCard!=m_bLastTurnCard)
	{
		//设置变量
		m_bLastTurnCard=bLastTurnCard;

		//更新界面
		UpdateGameView(NULL);
	}

	return;
}

//设置主牌
void CGameClientView::SetValueOrder(BYTE cbMainValue, BYTE cbValueMySelf, BYTE cbValueOther)
{
	//设置变量
	m_cbMainValue=cbMainValue;
	m_cbValueOrder[0]=cbValueMySelf;
	m_cbValueOrder[1]=cbValueOther;

	//更新界面
	UpdateGameView(NULL);

	return;
}

//设置亮主
void CGameClientView::SetCallCardInfo(WORD wCallCardUser, BYTE cbCallColor, BYTE cbCallCount)
{
	//设置变量
	m_cbCallColor=cbCallColor;
	m_cbCallCount=cbCallCount;
	m_wCallCardUser=wCallCardUser;

	//更新界面
	UpdateGameView(NULL);

	return;
}

//埋底用户
void CGameClientView::SetPutConcealUser(WORD wPutConcealUser)
{
	//设置变量
	m_wPutConcealUser = wPutConcealUser;

	//更新界面
	UpdateGameView(NULL);
	return;
}

//艺术字体
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//变量定义
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//绘画边框
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//绘画字体
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//开始按钮
void CGameClientView::OnStart()
{
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);
	return;
}

//出牌按钮
void CGameClientView::OnOutCard()
{
	AfxGetMainWnd()->SendMessage(IDM_OUT_CARD,1,1);
	return;
}

//提示按钮
void CGameClientView::OnOutPrompt()
{
	AfxGetMainWnd()->SendMessage(IDM_OUT_PROMPT,0,0);
	return;
}

//留底按钮
void CGameClientView::OnSendConceal()
{
	AfxGetMainWnd()->SendMessage(IDM_SEND_CONCEAL,0,0);
	return;
}

//查看底牌
void CGameClientView::OnConcealCard()
{
	AfxGetMainWnd()->SendMessage(IDM_CONCEAL_CARD,0,0);
	return;
}

//请求离开
void CGameClientView::OnRuquestLeave()
{
	AfxGetMainWnd()->SendMessage(IDM_REQUEST_LEAVE,0,0);
	return;
}

//上轮扑克
void CGameClientView::OnLastTurnCard()
{
	AfxGetMainWnd()->SendMessage(IDM_LAST_TURN_CARD,0,0);
	return;
}

//拖管控制
void CGameClientView::OnStusteeControl()
{
	AfxGetMainWnd()->SendMessage(IDM_TRUSTEE_CONTROL,0,0);
	return;
}

//重新洗牌
void CGameClientView::OnReRandCard()
{
	AfxGetMainWnd()->SendMessage(IDM_RERANDCARD,0,0);
	return;

}

//炒翻底牌
void CGameClientView::OnOpenConcealCard()
{
	AfxGetMainWnd()->SendMessage(IDM_OPENCONCEALCARD,1,0);
	return;
}

//放弃炒底
void CGameClientView::OnGiveUp()
{
	AfxGetMainWnd()->SendMessage(IDM_OPENCONCEALCARD,0,0);
	return;
}

//鼠标消息
void CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//发送消息
	AfxGetMainWnd()->SendMessage(IDM_OUT_CARD,1,1);

	return;
}

//鼠标双击
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	//设置扑克
	m_btOutCard.EnableWindow(FALSE);
	m_btSendConceal.EnableWindow(FALSE);
	m_HandCardControl.ShootAllCard(false);

	return;
}

//////////////////////////////////////////////////////////////////////////
