#include "Stdafx.h"
#include "BJ.h"
#include "BJGameDlg.h"
#include "BJGameView.h"
#include "..\消息定义\CMD_BlackJack.h"


//////////////////////////////////////////////////////////////////////////

//定时器标识
#define IDI_STRAT_GAME					200								//继续游戏定时器
#define IDI_GIVE_UP						201								//放弃定时器

#define IDI_ADD_GOLD					202								//加注定时器
#define IDI_GET_CARD					203								//要牌定时器

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBJGameDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_GAME_START,OnStart)
	ON_MESSAGE(IDM_ADD_GOLD,OnAddGold)
	ON_MESSAGE(IDM_GIVE_UP,OnGiveUp)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////

//构造函数
CBJGameDlg::CBJGameDlg() : CGameFrameDlg(&m_GameView)
{	
	//辅助变量
	m_lGoldShow    = 0L;	
	m_lTurnMaxGold = 0L;
	m_bMeAddGold   = false;
	m_wCardSum     = 0;
	m_bCutCard     = false;
	memset(m_szName, 0, sizeof(m_szName));
	return;
}

//析构函数
CBJGameDlg::~CBJGameDlg()
{
}

//初始函数
bool CBJGameDlg::InitGameFrame()
{
	//设置标题
	SetWindowText(TEXT("湛江三公游戏"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	return true;
}

//重置框架
void CBJGameDlg::ResetGameFrame()
{	
	//辅助变量
	m_lGoldShow    = 0L;	
	m_lTurnMaxGold = 0L;
	m_bMeAddGold   = false;
	m_wCardSum     = 0;
	memset(m_szName, 0, sizeof(m_szName));
	memset(m_GameView.m_cbPlayStatus,0,sizeof(m_GameView.m_cbPlayStatus));
	return;
}

//游戏设置
void CBJGameDlg::OnGameOptionSet()
{
	return;
}

//时间消息
bool CBJGameDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_ADD_GOLD:		    //加注定时器
		{
			if ((nElapse == 0)&&(wChairID==GetMeChairID()))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_OUT"));
				
				OnAddGold(0,0);			
				return true;
			}
			if (nElapse <= 10) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}	
	case IDI_STRAT_GAME:       //继续游戏定时器
		{
			if (nElapse==0)
			{
				OnStart(0,0);
				return false;
			}
			if (nElapse<=10) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			return true;
		}
	}
	return false;
}

//旁观状态
void CBJGameDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{

}

//游戏场景
bool CBJGameDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//空闲状态
		{		
			//效验数据
			if (wDataSize != sizeof(CMD_S_StatusFree)) return false;

			CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pBuffer;

			//辅助变量
			m_bMeAddGold = false;

			//设置界面
			m_lGoldShow      = 0L;
			m_lTurnBasicGold = 0L;
			m_GameView.SetGoldTitleInfo(0, pStatusFree->dwBasicGold);

		//	m_GameView.m_HeapCard.ResetMinusMiddleCount();
		//	m_GameView.m_HeapCard.SetCardData(0,0,10,0);

			//设置控件
			if (IsLookonMode() == false)
			{
				m_GameView.m_btBegin.ShowWindow(SW_SHOW);
				m_GameView.m_btBegin.SetFocus();
			}
			return true;
		}
	case GS_ADDGOLDING:		//加注状态
		{		
			//效验数据
			if (wDataSize != sizeof(CMD_S_StatusAddGold)) return false;

			CMD_S_StatusAddGold * pStatusAddGold = (CMD_S_StatusAddGold *)pBuffer;	

			for(WORD i = 0; i < GAME_PLAYER; i++)
			{
				WORD wViewChairID = SwitchViewChairID(i);			

				//加注金币
				m_GameView.SetUserGoldInfo(wViewChairID,true,pStatusAddGold->lTableGold[2 * i]);
				//m_GameView.SetUserGoldInfo(wViewChairID,false,pStatusAddGold->lTableGold[2 * i + 1]);
				
				const tagUserData * pUserData = GetUserData(i);
				if (pUserData != NULL)
				{
					lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));
					if((IsLookonMode() == false) && (GetMeChairID() != pStatusAddGold->wBankerStation))
					{
						//加注界面
						if(!pStatusAddGold->bAddGoldEnd[GetMeChairID()])
						{
							m_GameView.m_GoldControl.SetMaxGold(pStatusAddGold->lMaxGold);
							m_GameView.m_GoldControl.ShowWindow(SW_SHOW);	
							m_GameView.m_btAddGold.ShowWindow(SW_SHOW);
							m_GameView.m_btGiveUp.ShowWindow(SW_SHOW);	
						}
						
						//设置定时器
						if(GetMeChairID() == pStatusAddGold->wCurrentUser)
						{
							SetGameTimer(GetMeChairID(), IDI_ADD_GOLD, 30);
						}						
					}
					//标识状态
					if(!pStatusAddGold->bAddGoldEnd[i] && i != pStatusAddGold->wBankerStation )
					{
						m_GameView.m_cbPlayStatus[wViewChairID] = 1;
					}		
					m_GameView.m_wBankerStation = SwitchViewChairID(pStatusAddGold->wBankerStation);
				}			
			}			
			return true;
		}

	case GS_PLAYING:	//游戏状态
		{
			//效验数据
			if (wDataSize != sizeof(CMD_S_StatusPlay)) return false;

			CMD_S_StatusPlay * pStatusPlay = (CMD_S_StatusPlay *)pBuffer;

			for(WORD i = 0; i < GAME_PLAYER; i++)
			{
				//变量定义
				WORD wViewChairID = SwitchViewChairID(i);
				const tagUserData * pUserData = GetUserData(i);

				if (pUserData != NULL)
				{
					lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));			
					m_GameView.m_CardControl[wViewChairID].SetCardData(pStatusPlay->cbTableCardArray[i],pStatusPlay->cbTableCardCount[i]);

					if(i == pStatusPlay->wBankerStation) continue;				

					//庄家标识
					m_GameView.m_wBankerStation = SwitchViewChairID(pStatusPlay->wBankerStation);
				}	
				else
				{
					m_GameView.m_CardControl[wViewChairID].SetCardData(NULL,0);
				}

				//加注金币
				m_GameView.SetUserGoldInfo(wViewChairID, true,  pStatusPlay->lTableGold[2 * i]);
				//m_GameView.SetUserGoldInfo(wViewChairID, false, pStatusPlay->lTableGold[2 * i + 1]);							
			}	

			//设置控件
			m_GameView.m_CardControl[SwitchViewChairID(GetMeChairID())].ShowFirstCard(true);
			
			if (IsLookonMode() == false) 
			{
				ActiveGameFrame();
				m_GameView.m_CardControl[2].AllowPositively(true);
			}	

			m_GameView.UpdateGameView(NULL);

			return true;
		}		
	}
	return false;
}

//网络消息
bool CBJGameDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch(wSubCmdID)
	{
	case SUB_S_GAME_START:           //游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_GOLD:			//加注结果
		{
			return OnSubAddGold(pBuffer,wDataSize);
		}
	case SUB_S_INIT_CARD:           //初始发牌
		{
			return OnSubInitCard(pBuffer,wDataSize);
		}	
	case SUB_S_USER_LEFT:		   //用户强退
		{						
			return OnSubUserLeft(pBuffer,wDataSize);
		}	
	case SUB_S_GAME_END:		   //游戏结束
		{			
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	
	}
	return false;
}


//游戏开始
bool CBJGameDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{	
	//效验数据
	if (wDataSize != sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart = (CMD_S_GameStart *)pBuffer;

	//游戏变量
	m_lTurnMaxGold   = pGameStart->lMaxGold;
	//基础金币
	//m_lTurnBasicGold = pGameStart->lTurnBasicGold;
	m_lTurnBasicGold = m_GameView.m_lBasicGold;
	memset(m_szName, 0, sizeof(m_szName));

	//变量定义
	WORD wMeChairID  = GetMeChairID();
	bool bLookonMode = IsLookonMode();
	LONG lBaseGold   = m_lTurnBasicGold;

	//设置状态
	SetGameStatus(GS_ADDGOLDING);
	m_GameView.SetGoldTitleInfo(pGameStart->lMaxGold,lBaseGold);
	//m_GameView.m_HeapCard.SetCardData(0,0,10,0);

	//设置界面
	m_GameView.m_wBankerStation = SwitchViewChairID(pGameStart->wBankerStation);	
	
	//设置控制
	if (bLookonMode == false && wMeChairID == pGameStart->wCurrentUser)
	{
		//设置界面
		m_GameView.m_btGiveUp.ShowWindow(SW_SHOW);
		m_GameView.m_btAddGold.ShowWindow(SW_SHOW);
		
		//m_GameView.SetUserGoldInfo(2, false, m_lTurnBasicGold);
		m_GameView.SetUserGoldInfo(2, true, m_lTurnBasicGold);
		m_GameView.m_GoldControl.SetMaxGold(pGameStart->lMaxGold - m_lTurnBasicGold);
		m_GameView.m_GoldControl.ShowWindow(SW_SHOW);

		//设置定时器	
		SetGameTimer(GetMeChairID(), IDI_ADD_GOLD, 30);		
	}

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//变量定义
		WORD wViewChairID = SwitchViewChairID(i);
		const tagUserData * pUserData = GetUserData(i);

		//设置界面
		if (pUserData != NULL)
		{
			m_GameView.SetUserGoldInfo(wViewChairID,false,0L);
			m_GameView.SetUserGoldInfo(wViewChairID,true,lBaseGold);
			lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));			
			if(wViewChairID != m_GameView.m_wBankerStation)
				m_GameView.m_cbPlayStatus[wViewChairID] = 1;
		}
		else
		{	
			m_GameView.SetUserGoldInfo(wViewChairID,true,0L);
			m_GameView.SetUserGoldInfo(wViewChairID,false,0L);	
		}

	}
	if (bLookonMode == false) 
	{
		ActiveGameFrame();
		m_GameView.m_CardControl[2].AllowPositively(true);
	}	
	else
	{	
		//设置界面			
		m_GameView.m_ScoreView.ShowWindow(SW_HIDE);	
		for (int i=0;i<CountArray(m_GameView.m_CardControl);i++) m_GameView.m_CardControl[i].SetCardData(NULL,0);
		memset(m_GameView.m_cbTopCardSum,0,sizeof(m_GameView.m_cbTopCardSum));
		memset(m_GameView.m_cbCardType,0,sizeof(m_GameView.m_cbCardType));
		memset(m_GameView.m_cbBottomCardSum,0,sizeof(m_GameView.m_cbBottomCardSum));

	}
	m_GameView.UpdateGameView(NULL);
	
	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_BEGIN"));

	return true;
}

//初始发牌
bool CBJGameDlg::OnSubInitCard(const void * pBuffer, WORD wDataSize)
{
	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

	//效验数据
	if (wDataSize != sizeof(CMD_S_InitCard)) return false;
	CMD_S_InitCard * pInitCard = (CMD_S_InitCard *)pBuffer;
	
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//变量定义
		WORD wViewChairID = SwitchViewChairID(i);
		const tagUserData * pUserData = GetUserData(i);

		//设置界面
		if (pUserData != NULL)
		{
			m_GameView.m_CardControl[wViewChairID].SetCardData(pInitCard->cbCardData[i],CARD_COUNT);	

			//标识状态
			if(wViewChairID != m_GameView.m_wBankerStation)
				m_GameView.m_cbPlayStatus[wViewChairID] = 2;
		}
		else
		{
			m_GameView.m_CardControl[wViewChairID].SetCardData(NULL,0);
		}		

		//设置控件
		m_GameView.m_CardControl[wViewChairID].ShowFirstCard(true);
	}

	//界面设置
	//WORD wDiceCount = pInitCard->wDiceCount;
	//for(WORD i=wDiceCount;i<wDiceCount+4;i++)
		//m_GameView.m_HeapCard.SetCardData(0,0,10,i);	

	//显示扑克总值
	m_GameView.m_cbTopCardSum[2] = pInitCard->cbCardSum;
	m_GameView.UpdateGameView(NULL);

	return true;
}

//用户加注
bool CBJGameDlg::OnSubAddGold(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize != sizeof(CMD_S_AddGold)) return false;
	CMD_S_AddGold * pAddGold = (CMD_S_AddGold *)pBuffer;	

	//变量定义
	WORD wMeChairID   = GetMeChairID();
	WORD wViewChairID = SwitchViewChairID(pAddGold->wPrevUser);

	//处理数据
	//m_GameView.SetUserGoldInfo(wViewChairID,false,pAddGold->lLastAddGold);

	m_GameView.SetUserGoldInfo(wViewChairID,true,pAddGold->lLastAddGold);

	if ((IsLookonMode() == false)&&(pAddGold->wPrevUser == wMeChairID))
	{
		//设置数据
		m_bMeAddGold = true;	
		
		PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
		
		ActiveGameFrame();
	}

	if(pAddGold->wCurrentUser!=INVALID_CHAIR)
	{
		if ((IsLookonMode() == false)&&(pAddGold->wCurrentUser == wMeChairID))
		{
			//设置界面
			m_GameView.m_btGiveUp.ShowWindow(SW_SHOW);
			m_GameView.m_btAddGold.ShowWindow(SW_SHOW);
			
			//m_GameView.SetUserGoldInfo(2, false, m_lTurnBasicGold);
			m_GameView.SetUserGoldInfo(2, true, m_lTurnBasicGold);
			m_GameView.m_GoldControl.SetMaxGold(pAddGold->lMaxGold - m_lTurnBasicGold);
			m_GameView.m_GoldControl.ShowWindow(SW_SHOW);

			//设置定时器	
			SetGameTimer(GetMeChairID(), IDI_ADD_GOLD, 30);	

		}

	}
	else 
	{
		//设置状态
		SetGameStatus(GS_PLAYING);
	}


	//标识状态
	if(wViewChairID != m_GameView.m_wBankerStation)
		m_GameView.m_cbPlayStatus[wViewChairID] = 0;
	m_GameView.UpdateGameView(NULL);

	return true;
}

//用户强退
bool CBJGameDlg::OnSubUserLeft(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//游戏结束
bool CBJGameDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{	
	//设置定时器
	KillGameTimer(IDI_GET_CARD);

	//效验参数
	if (wDataSize != sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd = (CMD_S_GameEnd *)pBuffer;
    
	//游戏正常结束
	BYTE cbCardData[13];
	m_GameView.m_ScoreView.ResetScore();

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//设置信息
		const tagUserData * pUserData = GetUserData(i);

		if (pUserData != NULL)
		{
			lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));
		}
		
		if (m_szName[i][0] != 0) 
		{

			if(i==pGameEnd->wBankerUser)strcat(m_szName[i],TEXT("(庄家)"));
			m_GameView.m_ScoreView.SetGameScore(i,m_szName[i],pGameEnd->lGameGold[i]);

			if (pGameEnd->cbUserCard[i] != 0)
			{
				WORD wViewStation = SwitchViewChairID(i);
				WORD wCardCount = m_GameView.m_CardControl[wViewStation].GetCardData(cbCardData,CountArray(cbCardData));
				cbCardData[0] = pGameEnd->cbUserCard[i];
				m_GameView.m_CardControl[wViewStation].SetCardData(cbCardData,wCardCount);
				m_GameView.m_CardControl[wViewStation].SortCardData();
				m_GameView.m_CardControl[wViewStation].ShowFirstCard(true);	

				/*wCardCount = m_GameView.m_CardControl[wViewStation+4].GetCardData(cbCardData,CountArray(cbCardData));
				cbCardData[0] = pGameEnd->cbCutCard[i];
				m_GameView.m_CardControl[wViewStation+4].SetCardData(cbCardData,wCardCount);
				m_GameView.m_CardControl[wViewStation+4].SortCardData();
				m_GameView.m_CardControl[wViewStation+4].ShowFirstCard(true);	*/	
				
				//显示总值
				m_GameView.m_cbTopCardSum[wViewStation]    = pGameEnd->cbCardSum[i];
				m_GameView.m_cbCardType[wViewStation]  =pGameEnd->cbCardType[i];
				//m_GameView.m_cbBottomCardSum[wViewStation] = pGameEnd->cbCutCardSum[i];
			}			
		}			
	}
	m_GameView.m_ScoreView.SetTax(pGameEnd->lTax);
	m_GameView.m_ScoreView.ShowWindow(SW_SHOW);	

	//游戏变量
	m_lTurnMaxGold   = 0L;
	m_lTurnBasicGold = 0L;

	//界面变量
	m_lGoldShow  = 0L;	
	m_GameView.m_wBankerStation = 255;
	m_GameView.m_cbCutCardMark  = 0;
	memset(m_szName,0,sizeof(m_szName));
	memset(m_GameView.m_cbPlayStatus,0,sizeof(m_GameView.m_cbPlayStatus));

	//标识分牌
	m_bCutCard = false;

	//设置界面
	if (IsLookonMode() == false)
	{
		m_GameView.m_btBegin.ShowWindow(SW_SHOW);
		m_GameView.m_btBegin.SetFocus();
		SetGameTimer(GetMeChairID(),IDI_STRAT_GAME,30);
	}
	
	m_GameView.m_GoldControl.ShowWindow(SW_HIDE);
	m_GameView.m_btGiveUp.ShowWindow(SW_HIDE);
	m_GameView.m_btAddGold.ShowWindow(SW_HIDE);
	
	m_GameView.UpdateGameView(NULL);

	//设置状态
	SetGameStatus(GS_FREE);

	//播放声音
	if(pGameEnd->lGameGold[GetMeChairID()] > 0)
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));	

	return true;
}

//游戏开始
LRESULT	CBJGameDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("HIT_BUTTON"));
	
	//设置界面	
	m_GameView.m_btBegin.ShowWindow(SW_HIDE);
	m_GameView.m_ScoreView.ShowWindow(SW_HIDE);	
	for (int i=0;i<CountArray(m_GameView.m_CardControl);i++) m_GameView.m_CardControl[i].SetCardData(NULL,0);
	memset(m_GameView.m_cbTopCardSum,0,sizeof(m_GameView.m_cbTopCardSum));
	memset(m_GameView.m_cbCardType,0,sizeof(m_GameView.m_cbCardType));
	memset(m_GameView.m_cbBottomCardSum,0,sizeof(m_GameView.m_cbBottomCardSum));
	memset(m_GameView.m_cbPlayStatus,0,sizeof(m_GameView.m_cbPlayStatus));
	//m_GameView.m_HeapCard.ResetMinusMiddleCount();
	//m_GameView.m_HeapCard.SetCardData(0,0,10,0);
	

	KillGameTimer(IDI_STRAT_GAME);

	//发送消息
    SendUserReady(NULL,0);

	return true;
}

//用户下注
LRESULT	CBJGameDlg::OnAddGold(WPARAM wParam, LPARAM lParam)
{	
	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
	
	LONG lGold =  m_GameView.m_GoldView[5].GetGold() + m_GameView.m_GoldControl.GetGold();

	//LONG lGold = m_lTurnBasicGold;

	lGold = __max(lGold,1);

	//设置信息
	CMD_C_AddGold AddPoint;
	//AddPoint.bDoubleGold = false;
	AddPoint.lGold       = lGold;
	AddPoint.wChairID    = GetMeChairID();

	SendData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));

	//设置定时器
	KillGameTimer(IDI_ADD_GOLD);	

	//更新界面
	m_GameView.m_GoldControl.ShowWindow(SW_HIDE);
	m_GameView.m_btGiveUp.ShowWindow(SW_HIDE);
	m_GameView.m_btAddGold.ShowWindow(SW_HIDE);
	m_GameView.UpdateGameView(NULL);

	return 0;
}

//用户取消
LRESULT	CBJGameDlg::OnGiveUp(WPARAM wParam, LPARAM lParam)
{
	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("HIT_BUTTON"));

	CMD_C_AddGold AddPoint;
	AddPoint.lGold       = 1L;
	//AddPoint.bDoubleGold = false;
	AddPoint.wChairID    = GetMeChairID();

	//设置定时器
	KillGameTimer(IDI_ADD_GOLD);
	
	//设置界面
	m_GameView.m_GoldControl.ShowWindow(SW_HIDE);
	m_GameView.m_btGiveUp.ShowWindow(SW_HIDE);
	m_GameView.m_btAddGold.ShowWindow(SW_HIDE);
    
	//发送数据
	SendData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));

	return true;
}