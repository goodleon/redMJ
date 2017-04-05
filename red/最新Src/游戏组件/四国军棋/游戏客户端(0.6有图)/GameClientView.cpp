#include "StdAfx.h"
#include "Resource.h"
#include "GameClientDlg.h"
#include "GameClientView.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////

//按钮标识 
#define IDC_PLACEMENT				100									//布局按钮
#define IDC_SAVEPLACEMENT			102									//保存按钮
#define IDC_LOADPLACEMENT			103									//载入按钮
#define IDC_RESETGAME				104									//重新游戏
#define IDC_LOAD_HISTORY			105									//载入棋局
#define IDC_PREV_STEP				106									//上一步棋
#define IDC_NEXT_STEP				107									//下一步棋
#define IDC_TURN_CHESS				108									//旋转棋盘
#define IDC_LOSE					109									//认输按钮
#define IDC_GIVEUP					110									//放弃按钮
#define IDC_PEACE					111									//和棋按钮




//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_PLACEMENT,OnHitPlacement)
	ON_BN_CLICKED(IDC_RESETGAME , OnHitResetGame)
	ON_BN_CLICKED(IDC_SAVEPLACEMENT , OnSavePlacement)
	ON_BN_CLICKED(IDC_LOADPLACEMENT , OnHitLoadPlacement)
	ON_BN_CLICKED(IDC_LOAD_HISTORY,OnHitLoadHistory)
	ON_BN_CLICKED(IDC_PREV_STEP,OnHitPrevStep)
	ON_BN_CLICKED(IDC_NEXT_STEP,OnHitNextStep)
	ON_BN_CLICKED(IDC_TURN_CHESS,OnHitTurn)
	ON_BN_CLICKED(IDC_LOSE,OnHitLose)
	ON_BN_CLICKED(IDC_GIVEUP,OnHitGiveUp)
	ON_BN_CLICKED(IDC_PEACE,OnHitPeace)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//设置变量
	m_bFrameX = 255 ;
	m_bFrameY = 255 ;


	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_lCurrentStepCount = 0 ;

	m_ChessBoard.SetParent(this) ;

	//获取大小

	return;
}

//析构函数
CGameClientView::~CGameClientView(void)
{
}

//建立消息
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//创建控件
	CSize size = m_ChessBoard.GetChessBoradSize() ;
	m_ChessBoard.CleanChess() ;

	//m_ScoreView.Create(IDD_GAME_SCORE,this);
	//m_ScoreView.ShowWindow(SW_HIDE);

	//创建按钮

	CRect CreateRect(0,0,0,0);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,CreateRect,this,IDD_GAME_SCORE);
	m_ScoreView.ShowWindow(SW_HIDE);
	m_btPlacement.Create("布局完成",WS_CHILD,CreateRect,this,IDC_PLACEMENT);
	m_btSavePlacement.Create("保存布局",WS_CHILD,CreateRect,this,IDC_SAVEPLACEMENT);
	m_btLoadPlacement.Create("载入布局",WS_CHILD,CreateRect,this,IDC_LOADPLACEMENT);
	m_btResetGame.Create("重新游戏",WS_CHILD,CreateRect,this,IDC_RESETGAME);
	m_btLoadHistory.Create(TEXT("装载棋局"),WS_CHILD|BS_OWNERDRAW,CreateRect,this,IDC_LOAD_HISTORY);
	m_btPrevStep.Create(TEXT("上一步棋"),WS_CHILD|BS_OWNERDRAW,CreateRect,this,IDC_PREV_STEP);	
	m_btNextStep.Create(TEXT("下一步棋"),WS_CHILD|BS_OWNERDRAW,CreateRect,this,IDC_NEXT_STEP);	
	m_btTurn.Create(TEXT("旋转棋盘"),WS_CHILD|BS_OWNERDRAW,CreateRect,this,IDC_TURN_CHESS);		
	m_btLose.Create(TEXT("本盘认输"),WS_CHILD|BS_OWNERDRAW,CRect(0,0,60,20),this,IDC_LOSE);
	m_btGiveUp.Create(TEXT("放弃一手"),WS_CHILD|BS_OWNERDRAW,CRect(0,0,60,20),this,IDC_GIVEUP);
	m_btPeace.Create(TEXT("请求和棋"),WS_CHILD|BS_OWNERDRAW,CRect(0,0,60,20),this,IDC_PEACE);



	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();

	m_btPlacement.SetButtonImage(IDB_BT_BUTTON,hInstance,false);
	m_btSavePlacement.SetButtonImage(IDB_BT_BUTTON,hInstance,false);
	m_btLoadPlacement.SetButtonImage(IDB_BT_BUTTON,hInstance,false);
	m_btResetGame.SetButtonImage(IDB_BT_BUTTON,hInstance,false);
	m_btLoadHistory.SetButtonImage(IDB_BT_BUTTON,hInstance,false);
	m_btPrevStep.SetButtonImage(IDB_BT_BUTTON,hInstance,false);
	m_btNextStep.SetButtonImage(IDB_BT_BUTTON,hInstance,false);
	m_btTurn.SetButtonImage(IDB_BT_BUTTON,hInstance,false);
	m_btLose.SetButtonImage(IDB_BT_BUTTON,hInstance,false);
	m_btGiveUp.SetButtonImage(IDB_BT_BUTTON,hInstance,false);
	m_btPeace.SetButtonImage(IDB_BT_BUTTON,hInstance,false);

	m_ImageHeadBg.SetLoadInfo(IDB_HEAD_BG1,hInstance);
	m_ImageLeve1.SetLoadInfo(IDB_LEVEL1,hInstance);
	m_ImageLeve2.SetLoadInfo(IDB_LEVEL2,hInstance);
	m_ImageLeve3.SetLoadInfo(IDB_LEVEL3,hInstance);
	m_ImageLeve4.SetLoadInfo(IDB_LEVEL4,hInstance);
	m_ImageLeve5.SetLoadInfo(IDB_LEVEL5,hInstance);
	m_ImageLeve6.SetLoadInfo(IDB_LEVEL6,hInstance);

	m_btPlacement.ShowWindow(SW_SHOW) ;
	m_btPlacement.EnableWindow(TRUE) ;
	m_btSavePlacement.ShowWindow(SW_SHOW) ;
	m_btSavePlacement.EnableWindow(TRUE) ;
	m_btLoadPlacement.ShowWindow(SW_SHOW) ;
	m_btLoadPlacement.EnableWindow(TRUE) ;

	m_btLoadHistory.ShowWindow(SW_SHOW) ;
	m_btLoadHistory.EnableWindow(TRUE) ;

	m_btPrevStep.ShowWindow(SW_SHOW) ;
	m_btPrevStep.EnableWindow(FALSE) ;
	m_btNextStep.ShowWindow(SW_SHOW) ;
	m_btNextStep.EnableWindow(FALSE) ;
	m_btTurn.ShowWindow(SW_SHOW) ;
	m_btTurn.EnableWindow(FALSE) ;

	m_bDeasilOrder = (AfxGetApp()->GetProfileInt(TEXT("GameOption") , TEXT("DeasilOrder") , FALSE)) ? true : false ;

	return 0;
}

//重置界面
void CGameClientView::ResetGameView()
{
	//棋盘控制
	m_ChessBoard.CleanChess();

	//控件控制
	if (m_PeaceRequest.GetSafeHwnd()) m_PeaceRequest.DestroyWindow();

	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{

	RECT btRect;
	m_btPlacement.GetClientRect(&btRect);
	CSize boardSize = m_ChessBoard.GetChessBoradSize() ;

	//按钮位置
	m_btPlacement.MoveWindow(nWidth-btRect.right-(nWidth-boardSize.cx)/2-10,nHeight-3*btRect.bottom-80,btRect.right,btRect.bottom);
	m_btSavePlacement.MoveWindow(nWidth-btRect.right-(nWidth-boardSize.cx)/2-10,nHeight-2*btRect.bottom-80,btRect.right,btRect.bottom);
	m_btLoadPlacement.MoveWindow(nWidth-btRect.right-(nWidth-boardSize.cx)/2-10,nHeight-btRect.bottom-80,btRect.right,btRect.bottom);
	m_btResetGame.MoveWindow(nWidth-btRect.right-(nWidth-boardSize.cx)/2-10,nHeight-3*btRect.bottom-80,btRect.right,btRect.bottom);

	m_btLoadHistory.MoveWindow(nWidth-2*btRect.right-(nWidth-boardSize.cx)/2-15,nHeight-4*btRect.bottom-80,btRect.right,btRect.bottom);
	m_btPrevStep.MoveWindow(nWidth-2*btRect.right-(nWidth-boardSize.cx)/2-15,nHeight-3*btRect.bottom-80,btRect.right,btRect.bottom);
	m_btNextStep.MoveWindow(nWidth-2*btRect.right-(nWidth-boardSize.cx)/2-15,nHeight-2*btRect.bottom-80,btRect.right,btRect.bottom);
	m_btTurn.MoveWindow(nWidth-2*btRect.right-(nWidth-boardSize.cx)/2-15,nHeight-btRect.bottom-80,btRect.right,btRect.bottom);

	m_btLose.MoveWindow(nWidth-2*btRect.right-(nWidth-boardSize.cx)/2-15,nHeight-3*btRect.bottom-80,btRect.right,btRect.bottom);
	m_btGiveUp.MoveWindow(nWidth-2*btRect.right-(nWidth-boardSize.cx)/2-15,nHeight-2*btRect.bottom-80,btRect.right,btRect.bottom);
	m_btPeace.MoveWindow(nWidth-2*btRect.right-(nWidth-boardSize.cx)/2-15,nHeight-btRect.bottom-80,btRect.right,btRect.bottom);



	//棋盘偏移
	//m_ChessBoard.SetExcursionX((nWidth-boardSize.cx)/2) ;
	m_ChessBoard.SetExcursionX(30) ;
	m_ChessBoard.SetExcursionY((nHeight-boardSize.cy)/2+30) ;


	//设置坐标
	if (m_bDeasilOrder==false)
	{
		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-145;
		m_ptFace[3].y=nHeight/2-m_nYFace-190;
		m_ptName[3].x=nWidth-m_nXBorder-m_nXFace-74;
		m_ptName[3].y=nHeight/2+m_nYFace-257;
		//m_ptTimer[3].x=nWidth-183;
		//m_ptTimer[3].y=171;
		m_ptTimer[3].x=nWidth-m_nXBorder-m_nXTimer-200;
		m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer/2-160;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-200;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-160;

		m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-145;
		m_ptFace[1].y=nHeight/2-m_nYFace-80;
		m_ptName[1].x=nWidth-m_nXBorder-m_nXFace-74;
		m_ptName[1].y=nHeight/2+m_nYFace-147;
		m_ptTimer[1].x=nWidth-m_nXBorder-m_nXTimer-200;
		m_ptTimer[1].y=nHeight/2-m_nYFace-m_nYTimer/2-50;
		m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-200;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-50;
	}
	else
	{
		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-145;
		m_ptFace[3].y=nHeight/2-m_nYFace-190;
		m_ptName[3].x=nWidth-m_nXBorder-m_nXFace-74;
		m_ptName[3].y=nHeight/2+m_nYFace-257;
		m_ptTimer[3].x=nWidth-m_nXBorder-m_nXTimer-200;
		m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer/2-160;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-200;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-160;

		m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-145;
		m_ptFace[1].y=nHeight/2-m_nYFace-80;
		m_ptName[1].x=nWidth-m_nXBorder-m_nXFace-74;
		m_ptName[1].y=nHeight/2+m_nYFace-147;
		m_ptTimer[1].x=nWidth-m_nXBorder-m_nXTimer-200;
		m_ptTimer[1].y=nHeight/2-m_nYFace-m_nYTimer/2-50;
		m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-200;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-50;

		//m_ptFace[1].x=9;
		//m_ptFace[1].y=nHeight/2-m_nYFace-190;
		//m_ptName[1].x=85;
		//m_ptName[1].y=nHeight/2+m_nYFace-257;		
		//m_ptTimer[1].x=28;
		//m_ptTimer[1].y=171;
		//m_ptReady[1].x=m_nXBorder+5+145+m_nXTimer;
		//m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-30;
	}

	//m_ptFace[0].x=nWidth/2-m_nXFace/2-70;
	//m_ptFace[0].y=m_nYBorder+40;
	//m_ptName[0].x=nWidth/2+5+m_nXFace/2-35;
	//m_ptName[0].y=m_nYBorder+37;
	//m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	//m_ptTimer[0].y=m_nYBorder+140;
	//m_ptReady[0].x=nWidth/2-m_nXTimer-m_nXFace/2+38;
	//m_ptReady[0].y=m_nYBorder+m_nYTimer/2+150;
	m_ptFace[0].x=nWidth-m_nXBorder-m_nXFace-145;
	m_ptFace[0].y=nHeight/2-m_nYFace+30;
	m_ptName[0].x=nWidth-m_nXBorder-m_nXFace-74;
	m_ptName[0].y=nHeight/2-m_nYFace+27;
	//m_ptTimer[0].x=nWidth-183;
	//m_ptTimer[0].y=171;
	m_ptTimer[0].x=nWidth-m_nXBorder-m_nXTimer-200;
	m_ptTimer[0].y=nHeight/2-m_nYFace-m_nYTimer/2+60;
	m_ptReady[0].x=nWidth-m_nXBorder-m_nXTimer-200;
	m_ptReady[0].y=nHeight/2-m_nYFace-m_nYTimer/2+60;

	m_ptFace[2].x=9;
	m_ptFace[2].y=nHeight-92;
	m_ptName[2].x=85;
	m_ptName[2].y=nHeight-95;
	//m_ptTimer[2].x=nWidth/2-m_nXFace/2;
	//m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer-5;
	m_ptTimer[2].x=nWidth/2-m_nXTimer-m_nXFace/2+30;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer/2-110;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2+30;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-110;

	POINT *pChatPt = GetChatPt();
	pChatPt[SCP_B].x =m_ptName[2].x-25;
	pChatPt[SCP_B].y =m_ptName[2].y-80;

	pChatPt[SCP_L].x =m_ptName[1].x-25;
	pChatPt[SCP_L].y =m_ptName[1].y+80;

	pChatPt[SCP_R].x =m_ptName[3].x-195;
	pChatPt[SCP_R].y =m_ptName[3].y+80;

	pChatPt[SCP_T].x =m_ptName[0].x-60;
	pChatPt[SCP_T].y =m_ptName[0].y+80;


	return;
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//绘画背景
	DrawViewImage(pDC,m_ImageBack,enMode_Spread);

	//绘画棋盘
	m_ChessBoard.DrawBoard(pDC) ;

	//绘画边框
	if(255!=m_bFrameX && 255!=m_bFrameY)
	{
		LONG lXPos = m_ChessBoard.GetChessXPos(m_bFrameX) , 
			 lYPos = m_ChessBoard.GetChessYPos(m_bFrameY) ;

		pDC->Draw3dRect(lXPos-2+3,lYPos-2+3,CHESS_WIDTH+4,CHESS_HEIGTH+4,RGB(225,0,0),RGB(100,0,0));
		pDC->Draw3dRect(lXPos-1+3,lYPos-1+3,CHESS_WIDTH+2,CHESS_HEIGTH+2,RGB(225,0,0),RGB(100,0,0));
		pDC->Draw3dRect(lXPos+2+3,lYPos+2+3,CHESS_WIDTH-4,CHESS_HEIGTH-4,RGB(255,0,0),RGB(200,0,0));
	}

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
	if(m_lCurrentStepCount>0)
	{
		CFont font;
		VERIFY(font.CreateFont(
			20,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			"Arial"));                 // lpszFacename
		COLORREF defColor = pDC->SetTextColor(RGB(255,255,255)) ;

		CFont* def_font = pDC->SelectObject(&font);
		CString strMsg ;
		strMsg.Format("当前步数为: %ld" , m_lCurrentStepCount) ;
		pDC->TextOut(m_nXBorder+5 , m_nYBorder+5, strMsg, strMsg.GetLength());
		pDC->SelectObject(def_font);
		pDC->SetTextColor(defColor) ;

		font.DeleteObject();
	}

	return;
}

//和棋请求
void CGameClientView::ShowPeaceRequest()
{
	if (m_PeaceRequest.m_hWnd==NULL) 
	{
		m_PeaceRequest.Create(IDD_USER_REQ,this);
		m_PeaceRequest.ShowWindow(SW_SHOW);
	}

	return;
}



//////////////////////////////////////////////////////////////////////////

void CGameClientView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CGameFrameView::OnLButtonUp(nFlags, point);

	BYTE bXPos, bYPos ;

	m_ChessBoard.SwitchMousePos(point , bXPos , bYPos) ;

	CString str ;
	str.Format("(%d,%d)" , bXPos , bYPos) ;

//	AfxMessageBox(str) ;

	//点击棋盘
	if(255!=bXPos && 255!=bYPos)
		GetParent()->SendMessage(WM_HIT_CHESS_BORAD , bXPos , bYPos) ;

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

void CGameClientView::OnHitPlacement()
{
	GetParent()->SendMessage(IDM_PLACEMENT , 0 , 0) ;
}

void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	BYTE bXPos, bYPos ;

	m_ChessBoard.SwitchMousePos(point , bXPos , bYPos) ;

	//点击棋盘
	if(255!=bXPos && 255!=bYPos)
		GetParent()->SendMessage(WM_CHANGE_CURSOR , bXPos , bYPos) ;


	CGameFrameView::OnMouseMove(nFlags, point);
}

//绘画边框
void CGameClientView::DrawChessFrame(BYTE bXPos , BYTE bYPos)
{
	m_bFrameX = bXPos ;
	m_bFrameY = bYPos ;

	UpdateGameView(NULL) ;
}

//得到边框
void CGameClientView::GetChessFrame(BYTE &bXPos , BYTE &bYPos)
{
	bXPos = m_bFrameX ;
	bYPos = m_bFrameY ;
}
//重新游戏
void CGameClientView::OnHitResetGame() 
{
	GetParent()->SendMessage(WM_RESET_GAME , 0 , 0) ;
}

//保存布局
void CGameClientView::OnSavePlacement()
{
	GetParent()->SendMessage(WM_SAVE_PLACEMENT , 0 , 0) ;
}

//装载布局
void CGameClientView::OnHitLoadPlacement() 
{
    GetParent()->SendMessage(WM_LOAD_PLACEMENT , 0 , 0) ;
}

//载入棋局
void CGameClientView::OnHitLoadHistory()
{
	GetParent()->SendMessage(IDM_LOAD_HISTORY,0,0);
}

//上一步棋			
void CGameClientView::OnHitPrevStep()
{
	GetParent()->SendMessage(IDM_PREV_STEP,0,0);
}

//下一步棋				
void CGameClientView::OnHitNextStep()
{
	GetParent()->SendMessage(IDM_NEXT_STEP,0,0);
}

//旋转棋盘				
void CGameClientView::OnHitTurn()
{
	GetParent()->SendMessage(IDM_TURN_CHESS,0,0);
}

//走棋顺序
void CGameClientView::SetUserOrder(bool bDeasilOrder)
{
	m_bDeasilOrder = bDeasilOrder ;


	//设置界面
	CRect rcClient;
	GetClientRect(&rcClient);

	RectifyGameView(rcClient.Width(),rcClient.Height());
	//更新界面
	UpdateGameView(NULL) ;
}

//走棋顺序
bool CGameClientView::IsDeasilOrder(bool bDeasilOrder)
{
	return bDeasilOrder ;
}

//认输按钮
void CGameClientView::OnHitLose()
{
	if(MessageBox("你是否确定要认输？","提示",MB_YESNO)==IDYES)
	{
		GetParent()->SendMessage(IDM_LOSE,0,0);
	}
}

//放弃按钮
void CGameClientView::OnHitGiveUp()
{
	if(MessageBox("你是否确定要放弃一手？","提示",MB_YESNO)==IDYES)
	{
		GetParent()->SendMessage(IDM_GIVEUP,0,0);
	}
}

//请求和棋
void CGameClientView::OnHitPeace()
{
	if(MessageBox("你是否确定要和棋？","提示",MB_YESNO)==IDYES)
	{
		GetParent()->SendMessage(IDM_PEACE,0,0);
	}
}

void CGameClientView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CGameFrameView::OnRButtonDown(nFlags, point);
	BYTE bXPos, bYPos ;

	m_ChessBoard.SwitchMousePos(point , bXPos , bYPos) ;

	CGameClientDlg *pParent = (CGameClientDlg *)GetParent() ;
	//非法判断
	if(CHESS_NULL == pParent->m_Chess[bXPos][bYPos].bChessID || CHESS_BACK!=pParent->m_Chess[bXPos][bYPos].bChessMode || CHESS_COLOR_NO==pParent->m_Chess[bXPos][bYPos].bColor)
        return ;

	//点击棋盘
	if(255!=bXPos && 255!=bYPos)
	{
		//标志图案
		m_ChessBoard.ShowFlagImage(point , true) ;
		m_ChessBoard.SetFlaChessPos(bXPos , bYPos) ;
	}

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

//设置步数 	
void CGameClientView::SetStepCount(LONG lStepCount)
{
	m_lCurrentStepCount = lStepCount ;
	//更新界面
	UpdateGameView(NULL) ;
}