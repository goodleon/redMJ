#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//宏定义
#define IDI_TIME_GIVE_UP						1							//放弃定时器

//静态变量
const BYTE	CGameLogic::m_bCardArray[28]=									//扑克数组
{
	 1, 8, 9,10,11,12,13,
	14,21,22,23,24,25,26,
	27,34,35,36,37,38,39,
	40,47,48,49,50,51,52,
};

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//开始模式

//////////////////////////////////////////////////////////////////////////

//获取扑克花色
BYTE CGameLogic::GetHuaKind(BYTE bCard)
{
	return (bCard-1)/13;
}

//获取扑克数值
BYTE CGameLogic::GetCardValoe(BYTE bCard)
{
	BYTE bVolea=(bCard-1)%13;
	if (bVolea==0) bVolea=13;
	return bVolea;
}

//获取扑克类型
BYTE CGameLogic::GetCardKind(BYTE bCardList[], BYTE bCardCount)
{
	if (IsTongHuaShun(bCardList,bCardCount)) return TONG_HUA_SHUN;
	if (IsTieZhi(bCardList,bCardCount)) return TIE_ZHI;
	if (IsHuLu(bCardList,bCardCount)) return HU_LU;
	if (IsTongHua(bCardList,bCardCount)) return TONG_HUA;
	if (IsShunZhi(bCardList,bCardCount)) return SHUN_ZI;
	if (IsSanTiao(bCardList,bCardCount)) return SAN_TIAO;
	if (IsLiangDui(bCardList,bCardCount)) return LAING_DUI;
	if (IsDuiZhi(bCardList,bCardCount)) return DUI_ZI;
	return SHAN;
}

//查找对牌
BYTE CGameLogic::FindDoubleCard(BYTE bCardList[], BYTE bCardCount, BYTE * bOutDoubleCard)
{
	BYTE bDoubleCount=0;
	if (bCardCount<2) return 0;
	for (BYTE i=0;i<bCardCount-1;i++)
	{
		if (GetCardValoe(bCardList[i])==GetCardValoe(bCardList[i+1]))
		{
			*(bOutDoubleCard+bDoubleCount*2)=bCardList[i];
			*(bOutDoubleCard+bDoubleCount*2+1)=bCardList[i+1];
			bDoubleCount++;
			i++;
		}
	}
	return bDoubleCount;
}

//是否同花顺
bool CGameLogic::IsTongHuaShun(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount!=5) return false;
	
	BYTE bBaseHua=GetHuaKind(bCardList[0]);
	BYTE bFirstCardVoloe=GetCardValoe(bCardList[0]);
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetHuaKind(bCardList[i])!=bBaseHua) return false;
		if (bFirstCardVoloe!=(GetCardValoe(bCardList[i])+i)) return false;
	}
	return true;
}

//是否铁支
bool CGameLogic::IsTieZhi(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount<4) return false;
	
	BYTE bBaseVoloe=GetCardValoe(bCardList[1]),bNoFixCount=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (GetCardValoe(bCardList[i])!=bBaseVoloe) bNoFixCount++;
		if ((bCardCount-bNoFixCount)<4) return false;
	}
	return true;
}

//是否葫芦
bool CGameLogic::IsHuLu(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount!=5) return false;

	BYTE bFirstVoloe=GetCardValoe(bCardList[0]);
	BYTE bMidVoloe=GetCardValoe(bCardList[2]);
	BYTE bLastVoloe=GetCardValoe(bCardList[4]);
	if (GetCardValoe(bCardList[1])!=bFirstVoloe) return false;
	if (GetCardValoe(bCardList[3])!=bLastVoloe) return false;
	if ((bMidVoloe!=bFirstVoloe)&&(bMidVoloe!=bLastVoloe)) return false;
	
	return true;
}

//是否同花
bool CGameLogic::IsTongHua(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount!=5) return false;
	
	BYTE bBaseHua=GetHuaKind(bCardList[0]);
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetHuaKind(bCardList[i])!=bBaseHua) return false;
	}
	return true;
}

//是否顺子
bool CGameLogic::IsShunZhi(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount!=5) return false;
	
	BYTE bBaseHua=GetHuaKind(bCardList[0]);
	BYTE bFirstCardVoloe=GetCardValoe(bCardList[0]);
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (bFirstCardVoloe!=(GetCardValoe(bCardList[i])+i)) return false;
	}
	return true;
}

//是否三条
bool CGameLogic::IsSanTiao(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount<3) return false;
	
	BYTE bBaseVoloe=GetCardValoe(bCardList[2]),bBaseCount=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (GetCardValoe(bCardList[i])==bBaseVoloe) bBaseCount++;
	}
	return (bBaseCount==3);
}

//是否两对
bool CGameLogic::IsLiangDui(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount<4) return FALSE;
	
	BYTE bDoubleCount=0;
	for (BYTE i=0;i<bCardCount-1;i++)
	{
		if (GetCardValoe(bCardList[i])==GetCardValoe(bCardList[i+1]))
		{
			i++;
			bDoubleCount++;
			if (bDoubleCount==2) return true;
		}
	}
	return false;
}

//是否对子
bool CGameLogic::IsDuiZhi(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount<2) return false;
	
	BYTE bDoubleCount=0;
	for (BYTE i=0;i<bCardCount-1;i++)
	{
		if (GetCardValoe(bCardList[i])==GetCardValoe(bCardList[i+1])) return true;
	}
	return false;
}

//对比单只扑克
bool CGameLogic::CompareOnlyOne(BYTE bFirstCard, BYTE bLastCard)
{
	BYTE bFirstVolae=GetCardValoe(bFirstCard);
	BYTE bLastVolae=GetCardValoe(bLastCard);
	if (bFirstVolae==bLastVolae) return bFirstCard>bLastCard;
	return bFirstVolae>bLastVolae;
}

//对比同花顺
bool CGameLogic::CompareTongHuaShun(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[0],bLastList[0]);
}

//对比铁支
bool CGameLogic::CompareTeiZhi(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[2],bLastList[2]);
}

//对比葫芦
bool CGameLogic::CompareHuLu(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[2],bLastList[2]);
}

//对比同花
bool CGameLogic::CompareTongHua(BYTE bFirstList[], BYTE bLastList[])
{
	BYTE bFirst=0,bLast=0;
	for (BYTE i=0;i<5;i++)
	{
		bFirst=GetCardValoe(bFirstList[i]);
		bLast=GetCardValoe(bLastList[i]);
		if (bFirst!=bLast) return (bFirst>bLast);
	}
	return GetHuaKind(bFirstList[0])>GetHuaKind(bLastList[0]);
}

//对比顺子
bool CGameLogic::CompareShunZhi(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[0],bLastList[0]);
}

//对比三条
bool CGameLogic::CompareSanTiao(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[2],bLastList[2]);
}

//对比两对
bool CGameLogic::CompareLiangDui(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount)
{
	BYTE bDoubleCard[2][4];
	if (FindDoubleCard(bFirstList,bCardCount,bDoubleCard[0])!=2) return false;
	if (FindDoubleCard(bLastList,bCardCount,bDoubleCard[1])!=2) return true;
	BYTE bFirstVoloe=GetCardValoe(bDoubleCard[0][0]);
	BYTE bLastVoloe=GetCardValoe(bDoubleCard[1][0]);
	if (bFirstVoloe==bLastVoloe) 
	{
		bFirstVoloe=GetCardValoe(bDoubleCard[0][2]);
		bLastVoloe=GetCardValoe(bDoubleCard[1][2]);
		if (bFirstVoloe!=bLastVoloe) return bFirstVoloe>bLastVoloe;
		return (GetHuaKind(bDoubleCard[0][0])==HEI_TAO);
	}
	return bFirstVoloe>bLastVoloe;
}

//对比对子
bool CGameLogic::CompareDuiZhi(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount)
{
	BYTE bDoubleCard[2][4];
	if (FindDoubleCard(bFirstList,bCardCount,bDoubleCard[0])!=1) return false;
	if (FindDoubleCard(bLastList,bCardCount,bDoubleCard[1])!=1) return true;
	BYTE bFirstVoloe=GetCardValoe(bDoubleCard[0][0]);
	BYTE bLastVoloe=GetCardValoe(bDoubleCard[1][0]);
	if (bFirstVoloe==bLastVoloe) return (GetHuaKind(bDoubleCard[0][0])==HEI_TAO);
	return bFirstVoloe>bLastVoloe;
}

//对比散牌
bool CGameLogic::CompareSanCard(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount)
{
	return CompareOnlyOne(bFirstList[0],bLastList[0]);
}

//混乱扑克
void CGameLogic::RandCard(BYTE bCardBuffer[], BYTE bBufferCount)
{
	BYTE bSend=0,bStation=0,bCardList[CountArray(m_bCardArray)];
	CopyMemory(bCardList,m_bCardArray,sizeof(m_bCardArray));
	static long int dwRandCount=0L;
	srand((unsigned)time(NULL)+dwRandCount++);
	do
	{
		bStation=rand()%(CountArray(m_bCardArray)-bSend);
		bCardBuffer[bSend++]=bCardList[bStation];
		bCardList[bStation]=bCardList[CountArray(m_bCardArray)-bSend];
	} while (bSend<bBufferCount);

	return;
}

//排列扑克
void CGameLogic::SortCard(BYTE bCardList[], BYTE bCardCount)
{
	bool bSorted=true;
	BYTE bTemp,bLast=bCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++) 
		{
			if (CompareOnlyOne(bCardList[i],bCardList[i+1])==FALSE)
			{	
				bTemp=bCardList[i];
				bCardList[i]=bCardList[i+1];
				bCardList[i+1]=bTemp;
				bSorted=false;
			}	
		}
		bLast--;
	} while (bSorted==false);

	return;
}

//对比扑克，前者大于后者 TRUE，否则 FALSE
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount)
{
	//排列扑克
	SortCard(bFirstList,bCardCount);
	SortCard(bLastList,bCardCount);
	
	//获取类型
	BYTE bFirstKind=GetCardKind(bFirstList,bCardCount);
	BYTE bLastKind=GetCardKind(bLastList,bCardCount);
	
	//类型不相同，对比类型
	if (bFirstKind!=bLastKind) return bFirstKind>bLastKind;
	
	//类型相同
	switch(bFirstKind)
	{
		//散牌
	case SHAN: return CompareSanCard(bFirstList,bLastList,bCardCount);
		//对子
	case DUI_ZI: return CompareDuiZhi(bFirstList,bLastList,bCardCount);
		//两对
	case LAING_DUI: return CompareLiangDui(bFirstList,bLastList,bCardCount);
		//三条
	case SAN_TIAO: return CompareSanTiao(bFirstList,bLastList);
		//顺子
	case SHUN_ZI: return CompareShunZhi(bFirstList,bLastList);
		//同花
	case TONG_HUA: return CompareTongHua(bFirstList,bLastList);
		//葫芦
	case HU_LU: return CompareHuLu(bFirstList,bLastList);
		//铁支
	case TIE_ZHI: return CompareTeiZhi(bFirstList,bLastList);
		//同花顺
	case TONG_HUA_SHUN: return CompareTongHuaShun(bFirstList,bLastList);
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏变量
	m_wOperaCount=0;
	m_bSendCardCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_bTableCardCount,0,sizeof(m_bTableCardCount));
	memset(m_bTableCardArray,0,sizeof(m_bTableCardArray));

	//金币信息
	m_lMaxGold=0;
	m_lTurnBasicGold=0;
	memset(m_lTableGold,0,sizeof(m_lTableGold));

	//辅助变量
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=GET_OBJECTPTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//状态变量
	m_wOperaCount=0;
	m_bSendCardCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	memset(m_bTableCardArray,0,sizeof(m_bTableCardArray));
	memset(m_bTableCardCount,0,sizeof(m_bTableCardCount));

	//金币变量
	m_lMaxGold=0L;
	m_lTurnBasicGold=0L;
	memset(m_lTableGold,0,sizeof(m_lTableGold));

	return;
}

//开始模式
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//游戏状态
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	ASSERT(wChairID<m_wPlayerCount);
	return m_bPlayStatus[wChairID]?true:false;
}

//游戏开始
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//状态变量
	m_wOperaCount=0;
	m_bSendCardCount=0;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	memset(m_bTableCardCount,0,sizeof(m_bTableCardCount));

	//金币变量
	m_lMaxGold=0L;
	m_lTurnBasicGold=0L;
	memset(m_lTableGold,0,sizeof(m_lTableGold));

	//设置状态
	m_pITableFrame->SetGameStatus(GS_PLAYING);

	//变量定义
	const tagUserScore * pUserScore=NULL;
	IServerUserItem * pIServerUserItem=NULL;

	//计算下注
	bool bCalculateGold=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			m_bPlayStatus[i]=true;
			m_lTableGold[i*2+1]=m_pGameServiceOption->lCellScore;
			if (bCalculateGold==false)
			{
				bCalculateGold=true;
				pUserScore=pIServerUserItem->GetUserScore();
				ASSERT(pUserScore->lScore>m_pGameServiceOption->lCellScore);
				m_lMaxGold=pUserScore->lScore/10;//钱最少的玩家的10%
			}
			else 
			{
				pUserScore=pIServerUserItem->GetUserScore();
				ASSERT(pUserScore->lScore>1L);
				m_lMaxGold=__min(m_lMaxGold,pUserScore->lScore/10);//钱最少的玩家的10%
			}
		}
	}

	//分发扑克
	m_bSendCardCount=2;
	m_GameLogic.RandCard(m_bTableCardArray[0],sizeof(m_bTableCardArray)/sizeof(m_bTableCardArray[0][0]));
	for (WORD i=0;i<m_wPlayerCount;i++) 
	{
		if (m_bPlayStatus[i]==TRUE)
		{
			m_bTableCardCount[i]=m_bSendCardCount;
		}
	}
	m_wCurrentUser=DeduceWiner(1,1);

	//发送开始
	CMD_S_GameStart GameStart;
	memset(&GameStart,0,sizeof(GameStart));
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.lMaxGold=m_lMaxGold;
	GameStart.lTurnMaxGold=m_lMaxGold/4L;
	GameStart.lTurnBasicGold=m_lTurnBasicGold;
	for (WORD i=0;i<m_wPlayerCount;i++) GameStart.bCardData[i]=m_bTableCardArray[i][1];
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		GameStart.bFundusCard=0;
		if (m_bPlayStatus[i]==TRUE)
		{
			GameStart.bFundusCard=m_bTableCardArray[i][0];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	//设置定时器
	//m_pITableFrame->SetGameTimer(IDI_TIME_GIVE_UP,35000L,1,m_wCurrentUser);

	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
	case GER_NO_PLAYER:		//没有玩家
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//计算总注
			LONG lAllGold=0L;
			for (WORD i=0;i<CountArray(m_lTableGold);i++) lAllGold+=m_lTableGold[i];

			//统计信息
			WORD wWinerUser=DeduceWiner(0,4);
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//底牌信息
				if ((cbReason==GER_NO_PLAYER)||(m_bPlayStatus[i]==FALSE)) GameEnd.bUserCard[i]=0;
				else GameEnd.bUserCard[i]=m_bTableCardArray[i][0];

				//积分信息
				if (i==wWinerUser)
				{
					GameEnd.lGameGold[i]=lAllGold-m_lTableGold[i*2]-m_lTableGold[i*2+1];
					GameEnd.lTax=(GameEnd.lGameGold[i]>=100L)?GameEnd.lGameGold[i]/10L:0L;
					GameEnd.lGameGold[i]-=GameEnd.lTax;
				}
				else GameEnd.lGameGold[i]=-m_lTableGold[i*2]-m_lTableGold[i*2+1];
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//修改用户分数
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			memset(&ScoreInfo,0,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_bPlayStatus[i]==TRUE)
				{
					ScoreInfo[i].lScore=GameEnd.lGameGold[i];
					ScoreInfo[i].ScoreKind=(GameEnd.lGameGold[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
				}
			}
			m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,GameEnd.lTax);

			//删除定时器
			/*if (m_bTimeStation!=255)
			{
				KillTimer(m_bTimeStation);
				m_bTimeStation=255;
			}*/

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//用户强退
		{
			//效验参数
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//强退处理
			OnUserGiveUp(wChairID);

			return true;
		}
	}

	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	switch (bGameStatus)
	{
	case GS_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));
			StatusFree.dwBasicGold=m_pGameServiceOption->lCellScore;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_PLAYING:	//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lMaxGold=m_lMaxGold;
			StatusPlay.lTurnBasicGold=m_lTurnBasicGold;
			StatusPlay.lBasicGold=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bPlayStatus,m_bPlayStatus,sizeof(m_bPlayStatus));
			CopyMemory(StatusPlay.lTableGold,m_lTableGold,sizeof(m_lTableGold));

			//设置下注
			if (m_bSendCardCount<=2) StatusPlay.lTurnMaxGold=m_lMaxGold/4L;
			else if (m_bSendCardCount==3) StatusPlay.lTurnMaxGold=m_lMaxGold/2L;
			else StatusPlay.lTurnMaxGold=m_lMaxGold;

			//设置扑克
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_bPlayStatus[i]==TRUE)
				{
					ASSERT(m_pITableFrame->GetServerUserItem(i)!=NULL);
					StatusPlay.bTableCardCount[i]=m_bTableCardCount[i];
					if ((i==wChiarID)&&(bSendSecret==true)) StatusPlay.bTableCardArray[i][0]=m_bTableCardArray[i][0];
					CopyMemory(&StatusPlay.bTableCardArray[i][1],&m_bTableCardArray[i][1],(m_bTableCardCount[i]-1)*sizeof(BYTE));
				}
			}

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//效验结果
	ASSERT(FALSE);

	return false;
}

//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_ADD_GOLD:	//用户加注
		{
			//效验数据
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;
			if (m_bPlayStatus[pUserData->wChairID]==FALSE) return true;

			//消息处理
			CMD_C_AddGold * pAddGold=(CMD_C_AddGold *)pDataBuffer;
			OnUserAddGold(pUserData->wChairID,pAddGold->lGold,false);

			return true;
		}
	case SUB_C_GIVE_UP:		//用户放弃
		{
			//效验数据
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;
			if (m_bPlayStatus[pUserData->wChairID]==FALSE) return true;

			//消息处理
			OnUserGiveUp(pUserData->wChairID);

			return true;
		}
	}

	return false;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//放弃事件
bool CTableFrameSink::OnUserGiveUp(WORD wChairID)
{
	//状态判断
	ASSERT(m_bPlayStatus[wChairID]==TRUE);
	if (m_bPlayStatus[wChairID]==FALSE) return false;

	//设置数据
	m_bPlayStatus[wChairID]=FALSE;
	m_bTableCardCount[wChairID]=0;

	//发送放弃消息
	CMD_S_GiveUp GiveUp;
	GiveUp.wUserChairID=wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//写入数据库
	tagScoreInfo ScoreInfo;
	ScoreInfo.ScoreKind=enScoreKind_Lost;
	ScoreInfo.lScore=-m_lTableGold[wChairID*2]-m_lTableGold[wChairID*2+1];
	m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);

	//判断结束
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_bPlayStatus[i]==TRUE) wPlayerCount++;
	}
	if (wPlayerCount>=2)
	{
		if (m_wCurrentUser==wChairID) OnUserAddGold(wChairID,0L,true);
	}
	else OnEventGameEnd(INVALID_CHAIR,NULL,GER_NO_PLAYER);

	return true;
}

//加注事件
bool CTableFrameSink::OnUserAddGold(WORD wChairID, LONG lGold, bool bCancel)
{
	//设置数据
	if (bCancel==false)
	{
		//效验数据
		ASSERT(m_wCurrentUser==wChairID);
		if (m_wCurrentUser!=wChairID) return false;

		//计算限注
		LONG lMaxGold=0L;
		if (m_bSendCardCount<=2) lMaxGold=m_lMaxGold/4L;
		else if (m_bSendCardCount==3) lMaxGold=m_lMaxGold/2L;
		else lMaxGold=m_lMaxGold;
		
		//计算剩下的金币
		LONG lLeaveGold=lMaxGold-m_lTableGold[wChairID*2+1];
		m_lTableGold[wChairID*2]=__min(lLeaveGold,__max(lGold,m_lTurnBasicGold));
	}

	//操作次数
	bool bFinishTurn=false;
	WORD wNextUser=INVALID_CHAIR;
	for (WORD i=0;i<m_wPlayerCount-1;i++)
	{
		m_wOperaCount++;
		wNextUser=(m_wCurrentUser+i+1)%m_wPlayerCount;
		if (m_bPlayStatus[wNextUser]==TRUE) break;
	}
	
	//判断下注
	if (m_wOperaCount>=m_wPlayerCount)
	{
		LONG lTableGold=0L;
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if (m_bPlayStatus[i]==TRUE)
			{
				lTableGold=m_lTableGold[i*2];
				break;
			}
		}
		while (i<m_wPlayerCount)
		{
			if ((m_bPlayStatus[i]==TRUE)&&(m_lTableGold[i*2]!=lTableGold)) break;
			i++;
		}
		if (i==m_wPlayerCount) bFinishTurn=true;
	}

	//一轮判断
	if (bFinishTurn==true)
	{
		//发送数据
		CMD_S_AddGold GoldResult;
		GoldResult.lCurrentLessGold=0L;
		GoldResult.wLastChairID=wChairID;
		GoldResult.wCurrentUser=INVALID_CHAIR;
		GoldResult.lLastAddGold=m_lTableGold[wChairID*2];
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_GOLD,&GoldResult,sizeof(GoldResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_GOLD,&GoldResult,sizeof(GoldResult));

		//累计金币
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_lTableGold[i*2+1]+=m_lTableGold[i*2];
			m_lTableGold[i*2]=0L;
		}

		//开始一轮
		if (m_bSendCardCount<5) 
		{
			//设置变量
			m_wOperaCount=0;
			m_bSendCardCount++;
			m_lTurnBasicGold=0L;
			m_wCurrentUser=DeduceWiner(1,m_bSendCardCount-1);

			//发送扑克
			CMD_R_SendCard SendCard;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if (m_bPlayStatus[i]==TRUE) m_bTableCardCount[i]=m_bSendCardCount;
				SendCard.bUserCard[i]=m_bTableCardArray[i][m_bSendCardCount-1];
			}

			//删除旧定时器
			/*if (m_bTimeStation!=255)
			{
			KillTimer(m_bTimeStation);
			m_bTimeStation=255;
			}*/

			//设置定时器
			/*m_bTimeStation=TIME_PASS+m_wCurrentUser;
			SetTimer(m_bTimeStation,TIME_COUNT);*/

			//发送消息
			SendCard.wCurrentUser=m_wCurrentUser;
			SendCard.lMaxGold=(m_bSendCardCount!=3)?m_lMaxGold:m_lMaxGold/2L;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		}
		else OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
	}
	else
	{
		//设置变量
		m_wCurrentUser=wNextUser;
		m_lTurnBasicGold=__max(m_lTableGold[wChairID*2],m_lTurnBasicGold);

		//发送数据
		CMD_S_AddGold AddGold;
		AddGold.wLastChairID=wChairID;
		AddGold.wCurrentUser=m_wCurrentUser;
		AddGold.lCurrentLessGold=m_lTurnBasicGold;
		AddGold.lLastAddGold=m_lTableGold[wChairID*2];
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_GOLD,&AddGold,sizeof(AddGold));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_GOLD,&AddGold,sizeof(AddGold));

		//删除旧定时器
		/*if (m_bTimeStation!=255)
		{
			KillTimer(m_bTimeStation);
			m_bTimeStation=255;
		}

		//设置定时器
		m_bTimeStation=TIME_PASS+m_wCurrentUser;
		SetTimer(m_bTimeStation,TIME_COUNT);*/
	}

	return true;
}

//推断胜者
WORD CTableFrameSink::DeduceWiner(BYTE bBeginPos, BYTE bEndPos)
{
	//保存扑克
	BYTE bTableCardArray[m_wPlayerCount][5];
	CopyMemory(bTableCardArray,m_bTableCardArray,sizeof(m_bTableCardArray));

	//寻找玩家
	for (BYTE bWiner=0;bWiner<m_wPlayerCount;bWiner++)
	{
		if (m_bPlayStatus[bWiner]==TRUE) break;
	}

	//对比玩家
	for (BYTE i=(bWiner+1);i<m_wPlayerCount;i++)
	{
		if (m_bPlayStatus[i]==false) continue;
		if (m_GameLogic.CompareCard(bTableCardArray[i]+bBeginPos,bTableCardArray[bWiner]+bBeginPos,bEndPos-bBeginPos+1)==true) bWiner=i;
	}

	return bWiner;
}

//////////////////////////////////////////////////////////////////////////
