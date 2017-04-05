#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
const BYTE	CGameLogic::m_bCardArray[52] =									//扑克数组
{
	// 1, 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,
	//14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
	//27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	//40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
	//1,2,3,4,5,6,7,8,9,10,
	//1,2,3,4,5,6,7,8,9,10,
	//1,2,3,4,5,6,7,8,9,10,
	//1,2,3,4,5,6,7,8,9,10,
	 1, 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,
	14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
	27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
};

//获取扑克数值
BYTE CGameLogic::GetCardValue(BYTE bCard)
{
	//BYTE bValue = bCard%13;
	//if (bValue == 0 || bValue == 11 || bValue == 12) bValue=10;
	//if (bValue == 1) bValue = 11;
	//return bValue;
	BYTE bValue = bCard%13;
	if (bValue == 10 || bValue == 11 || bValue == 12 || bValue == 13) bValue=0;
	return bValue;
}

//获取扑克总值
BYTE CGameLogic::GetCardSum(const BYTE bCardBuffer[], const BYTE bCardCount)
{
	BYTE cbCardSum = 0;
	
	for(int i = 0; i < bCardCount; i++)
	{
		cbCardSum += GetCardValue(bCardBuffer[i]);
	}
	
	if(cbCardSum > 21)
	{
		for(int i = 0; i < bCardCount; i++)
		{
			//是否有A
			if(GetCardValue(bCardBuffer[i]) == 11)
			{
				//按1计算
				cbCardSum -= 10;
			}
		}
	}

	return cbCardSum;
}

//是否爆牌
bool CGameLogic::IsBurst(const BYTE bCardBuffer[], BYTE bCardCount)
{
	WORD wCardSum = 0;
	
	for(int i = 0; i < bCardCount; i++)
	{
		wCardSum += GetCardValue(bCardBuffer[i]);
	}

	if(wCardSum > 21)
	{
		for(int i = 0; i < bCardCount; i++)
		{
			//是否有A
			if(GetCardValue(bCardBuffer[i]) == 11)
			{
				//按1计算
				wCardSum -= 10;
			}
		}
	}

	if(wCardSum > 21) return true;
	
	return false;
}

//能否分牌
bool CGameLogic::CanCutCard(const BYTE bFirstCard, const BYTE bLastCard)
{
	BYTE bFirstValue = bFirstCard % 13;
	BYTE bLastValue  = bLastCard  % 13;
	if (bFirstValue == bLastValue) return true;
	return false;
}

//是否BlackJack
bool CGameLogic::IsBlackJack(const BYTE bCardBuffer[])
{
	BYTE bCardSum=0;
	if((GetCardValue(bCardBuffer[0]) == 1) && (GetCardValue(bCardBuffer[1]) == 10)) return true;
	if((GetCardValue(bCardBuffer[1]) == 1) && (GetCardValue(bCardBuffer[0]) == 10)) return true;
	return false;
}

//对比单只扑克
bool CGameLogic::CompareOnlyOne(BYTE bFirstCard, BYTE bLastCard)
{
	BYTE bFirstValue = GetCardValue(bFirstCard);
	BYTE bLastValue  = GetCardValue(bLastCard);
	if (bFirstValue == bLastValue) return bFirstValue > bLastValue;
	return bFirstValue > bLastValue;
}


//获取扑克数值
BYTE CGameLogic::GetCardValoe(BYTE bCard)
{
	BYTE bValue = bCard%13;
	if (bValue == 10 || bValue == 11 || bValue == 12 || bValue == 13) bValue=0;
	return bValue;
}

//是否为公牌
bool CGameLogic::IsGong(BYTE bCard)
{
	BYTE bValue = bCard%13;
	if (bValue == 11 || bValue == 12 || bValue == 13) return true;
	return false;
}

//是否333超级三公
bool CGameLogic::IsSanZhangSan(const BYTE bCardList[], const BYTE bCardCount)
{
	if (bCardCount<3) return false;

	BYTE c1,c2,c3;
	c1 = GetCardValoe(bCardList[0]);
	c2 = GetCardValoe(bCardList[1]);
	c3 = GetCardValoe(bCardList[2]);

	if (c1==3&&c2==3&&c3==3) return true;

	return false;
}

//是否三张一样公牌
bool CGameLogic::IsSanZhangGong(const BYTE bCardList[], const BYTE bCardCount)
{
	if (bCardCount<3) return false;
	
	//判断是否为公
	for(int i = 0; i <= bCardCount-1; i++)
	{
		if (!IsGong(bCardList[i])) return false;
	}

	BYTE c1,c2,c3;
	c1 = GetCardValoe(bCardList[0]);
	c2 = GetCardValoe(bCardList[1]);
	c3 = GetCardValoe(bCardList[2]);

	if(c1==c2&&c1==c3) return true;

	return false;
}

//是否对子公牌
bool CGameLogic::IsSanGongDui(const BYTE bCardList[], const BYTE bCardCount)
{
	if (bCardCount<3) return false;

	//判断是否为公
	for(int i = 0; i <= bCardCount-1; i++)
	{
		if (!IsGong(bCardList[i])) return false;
	}

	BYTE c1,c2,c3;
	c1 = GetCardValoe(bCardList[0]);
	c2 = GetCardValoe(bCardList[1]);
	c3 = GetCardValoe(bCardList[2]);
	
	if (((c1==c2)&&c3==0) && ((c2==c3)&&c1>10) && ((c1==c3)&&c2>10)) return true;
	return false;
}

//是否JQK公牌
bool CGameLogic::IsSanGong(const BYTE bCardList[], const BYTE bCardCount)
{
	if (bCardCount<3) return false;

	//判断是否为公
	for(int i = 0; i <= bCardCount-1; i++)
	{
		if (!IsGong(bCardList[i])) return false;
	}

	BYTE c1,c2,c3;
	c1 = GetCardValoe(bCardList[0]);
	c2 = GetCardValoe(bCardList[1]);
	c3 = GetCardValoe(bCardList[2]);

	if(c1==0&&c2==0&&c3==0) return true;

	return false;
}
//获取扑克类型
BYTE CGameLogic::GetCardKind(const BYTE bCardList[], const BYTE bCardCount)
{
	//是否对子
	//if (IsDuiZhi(bCardList,bCardCount)) return DUI_ZI;
	//return SHAN; //返回为散牌

	//CString a;
	//a.Format("牌数为%d",bCardCount);
	//AfxMessageBox(a);

	//是否333超级三公
	if(IsSanZhangSan(bCardList,bCardCount)) return SAN_ZHANG_SAN;
	//是否对子公牌
	if(IsSanGongDui(bCardList,bCardCount)) return SAN_GONG_DUI;
	//是否三张一样公牌
	if(IsSanZhangGong(bCardList,bCardCount)) return SAN_ZHANG_GONG;	
	//是否JQK公牌
	if(IsSanGong(bCardList,bCardCount)) return SAN_GONG;
	return SHAN; //返回为散牌

}

//对比对子
BYTE CGameLogic::CompareDuiZhi(const BYTE bFirstList[], const BYTE bLastList[], BYTE bCardCount)
{
	//return CompareOnlyOne(bFirstList[0],bLastList[0]);1
	BYTE bFirstVolae=GetCardValoe(bFirstList[0]);
	BYTE bLastVolae=GetCardValoe(bLastList[0]);
	//if(bFirstVolae==bLastVolae) return 0;
	if(bFirstVolae>bLastVolae) return 1;
	if(bFirstVolae<bLastVolae) return 2;

	return 0;
}

//对比散牌
BYTE CGameLogic::CompareSanCard(const BYTE bFirstList[], const BYTE bLastList[], BYTE bCardCount)
{
	BYTE bFirstCard, bLastCard;

	bFirstCard = (GetCardValoe(bFirstList[0])+GetCardValoe(bFirstList[1])+GetCardValoe(bFirstList[2]))%10;
	bLastCard = (GetCardValoe(bLastList[0])+GetCardValoe(bLastList[1])+GetCardValoe(bLastList[2]))%10;
	//CString a;
	//a.Format("庄:%d+%d+%d=%d,闲%d",bFirstList[0],bFirstList[1],bFirstList[2],bFirstCard,bLastCard);
	//a.Format("庄:%d,闲%d",bFirstCard,bLastCard);
	//AfxMessageBox(a);
	//if(bFirstCard==bLastCard) return 0;
	if(bFirstCard>bLastCard) return 1;
	if(bFirstCard<bLastCard) return 2;

	return 0;
}

//对比对子公牌
BYTE CGameLogic::CompareSanGongDui(const BYTE bFirstCard[], const BYTE bLastCard[])
{
	BYTE a1,a2,a3,a4,a5,b1,b2,b3,b4,b5;
	a1 = GetCardValue(bFirstCard[0]);
	a2 = GetCardValue(bFirstCard[1]);
	a3 = GetCardValue(bFirstCard[2]);

	if (a1 == a2) //如果第1张和第2张相同
	{	
		a4 = a1; //相同的牌为第1张
		a5 = a3; //不同的牌为第3张
	}
	else //如果第1张和第2张不相同，即是第1张和第3张相同
	{
		if (a1 == a3) //如果第1张和第3张相同
		{
			a4 = a1; //相同的牌为第1张
			a5 = a2; //不同的牌为第2张
		}
		else
		{
			a4 = a2; //因为第1张和第3张不相同，那么相同的牌为第2张
			a5 = a1; //不同的牌为第1张
		}
	}

	b1 = GetCardValue(bLastCard[0]);
	b2 = GetCardValue(bLastCard[1]);
	b3 = GetCardValue(bLastCard[2]);

	if(b1 == b2) //如果第1张和第2张相同
	{	
		b4 = b1; //相同的牌为第1张
		b5 = b3; //不同的牌为第3张
	}
	else //如果第1张和第2张不相同，即是第1张和第3张相同
	{
		if (b1 == b3) //如果第1张和第3张相同
		{
			b4 = b1; //相同的牌为第1张
			b5 = b2; //不同的牌为第2张
		}
		else
		{
			b4 = b2; //因为第1张和第3张不相同，那么相同的牌为第2张
			b5 = b1; //不同的牌为第1张
		}
	}

	if (a4 == b4) //如果对子一样大
	{
		if (a5 == b5) return 0; //和局庄赢
		else if (a5 > b5) return 1; //庄赢
		else if (a5 < b5) return 2; //闲赢
	}
	else if (a4 > b4) return 1; //庄赢
	else if (a4 < b4) return 2; //闲赢
}

//推断胜者
BYTE CGameLogic::DeduceWiner(const BYTE bBankerCard[],const BYTE bBankerCardCount,
							 const BYTE bIdleCard[],  const BYTE bIdleCardCount,BYTE &cbWinnerCardType)
{
	//初始化返回值
	cbWinnerCardType=SHAN;

	BYTE bFirstKind=GetCardKind(bBankerCard,bBankerCardCount);
	BYTE bLastKind=GetCardKind(bIdleCard,bIdleCardCount);
	
	//庄家赢
	if (bFirstKind>bLastKind) 
	{
		cbWinnerCardType=bFirstKind;
		return 1;
	}

	//闲家赢
	if (bFirstKind<bLastKind)
	{
		cbWinnerCardType=bLastKind;
		return 2;
	}

	cbWinnerCardType=bFirstKind;

	//类型相同
	switch(bFirstKind)
	{
		//散牌
	case SHAN:return CompareSanCard(bBankerCard,bIdleCard,3);		
		//是否为公牌
	case SAN_GONG: return 1; //都为JQK庄赢
		//是否对子公牌
	case SAN_GONG_DUI: return CompareSanGongDui(bBankerCard,bIdleCard);
		//是否三张一样公牌
	case SAN_ZHANG_GONG: return CompareOnlyOne(bBankerCard[0],bIdleCard[0]);
		//是否333超级三公
	case SAN_ZHANG_SAN: return 1; //这种情况1副牌不可能发生，除非是多副牌，预留 庄赢
	}

	//平,庄赢
	if (bFirstKind==bLastKind && bFirstKind!=0) return 0;
}

//混乱扑克
void CGameLogic::RandCard(BYTE bCardBuffer[], BYTE bBufferCount)
{
	BYTE bSend = 0,bStation = 0, bCardList[CountArray(m_bCardArray)];

	CopyMemory(bCardList, m_bCardArray, sizeof(m_bCardArray));
	
	static long int dwRandCount = 0L;
	
	srand((unsigned)time(NULL) + dwRandCount++);
	
	do
	{
		bStation = rand() % (CountArray(m_bCardArray) - bSend);

		bCardBuffer[bSend++] = bCardList[bStation];
		bCardList[bStation]  = bCardList[CountArray(m_bCardArray)-bSend];
	
	} while (bSend < bBufferCount);	
}

//排列扑克
void CGameLogic::SortCard(BYTE bCardList[], BYTE bCardCount)
{
	bool bSorted     = true;
	BYTE bTemp,bLast = bCardCount - 1;
	do
	{
		bSorted = true;
		for (BYTE i = 0; i < bLast; i++) 
		{
			if (CompareOnlyOne(bCardList[i],bCardList[i+1]) == FALSE)
			{	
				bTemp = bCardList[i];
				bCardList[i]   = bCardList[i+1];
				bCardList[i+1] = bTemp;
				bSorted = false;
			}	
		}
		bLast--;
	} while (bSorted == false);

	return;
}
//////////////////////////////////////////////////////////////////////////