#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

#define SHAN								0
#define SAN_GONG							1
#define	SAN_GONG_DUI						2
#define	SAN_ZHANG_GONG						3
#define	SAN_ZHANG_SAN						4

//游戏逻辑类
class CGameLogic
{
	//变量定义
private:
	static const BYTE				m_bCardArray[52];					//扑克定义

	//功能函数
public:	
	//混乱扑克
	void RandCard(BYTE bCardBuffer[], BYTE bBufferCount);
	//获取扑克数值
	BYTE GetCardValoe(BYTE bCard);
	//是否333超级三公
	bool IsSanZhangSan(const BYTE bCardList[], const BYTE bCardCount);
	//是否三张一样公牌
	bool IsSanZhangGong(const BYTE bCardList[], const BYTE bCardCount);
	//是否对子公牌
	bool IsSanGongDui(const BYTE bCardList[], const BYTE bCardCount);
	//是否JQK公牌
	bool IsSanGong(const BYTE bCardList[], const BYTE bCardCount);
	//是否为公牌
	bool IsGong(const BYTE bCardList);
	//获取扑克类型
	BYTE GetCardKind(const BYTE bCardList[], const BYTE bCardCount);
	//获取扑克数值
	BYTE GetCardValue(BYTE bCard);
	//获取扑克总值
	BYTE GetCardSum(const BYTE bCardBuffer[], const BYTE bCardCount);	
	//对比对子
	BYTE CompareDuiZhi(const BYTE bFirstList[], const BYTE bLastList[], BYTE bCardCount);
	//对比散牌
	BYTE CompareSanCard(const BYTE bFirstList[], const BYTE bLastList[], BYTE bCardCount);
	//对比对子公牌
	BYTE CompareSanGongDui(const BYTE bFirstCard[], const BYTE bLastCard[]);
	//对比单只扑克
	bool CompareOnlyOne(BYTE bFirstCard, BYTE bLastCard);		
	//排列扑克
	void SortCard(BYTE bCardList[], BYTE bCardCount);
	
	//是否爆牌
	bool IsBurst(const BYTE bCardBuffer[], BYTE bCardCount);
	//是否BlackJack
	bool IsBlackJack(const BYTE bCardBuffer[]);
	//能否分牌
	bool CanCutCard(const BYTE bFirstCard, const BYTE bLastCard);
	
	//推断胜者
	BYTE DeduceWiner(const BYTE bBankerCard[],const BYTE bBankerCardCount,
	                  const BYTE bIdleCard[],const BYTE bIdleCardCount,BYTE &cbWinnerCardType);

};

//////////////////////////////////////////////////////////////////////////

#endif