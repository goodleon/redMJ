#include "StdAfx.h"
#include "Resource.h"
#include ".\heapcard.h"

//构造函数
CHeapCard::CHeapCard()
{
	//控制变量
	m_ControlPoint.SetPoint(0,0);
	

	//扑克变量
	m_wFullCount=0;
	m_wMinusHeadCount=0;
	m_wMinusLastCount=0;
	for(BYTE i=0;i<10;i++)m_wMinusMiddlePos[i]=0x00;

	m_ImageBack.LoadFromResource(AfxGetInstanceHandle(),IDB_CARDHEAP);

	return;
}

//析构函数
CHeapCard::~CHeapCard()
{
	m_ImageBack.Destroy();
}

//绘画扑克
void CHeapCard::DrawCardControl(CDC * pDC)
{
	//绘画扑克
	if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
	{		
		//变量定义
		int nXPos=0,nYPos=0;		
		
		bool  bMinusMiddlePos[10];
		for(BYTE i=0;i<10;i++)bMinusMiddlePos[i]=false;
		for(BYTE i=0;i<10;i++)
		{
			if(m_wMinusMiddlePos[i]==0x00)continue;
			if(m_wFullCount+1<m_wMinusMiddlePos[i])continue;
			bMinusMiddlePos[m_wFullCount-m_wMinusMiddlePos[i]+1]=true;
		}	

		//中间扑克
		for (WORD i=m_wMinusHeadCount;i<m_wFullCount;i++)
		{
			nYPos=m_ControlPoint.y;
			nXPos=m_ControlPoint.x+i*20;
			if(bMinusMiddlePos[i]==false)					
				m_ImageBack.AlphaDrawImage(pDC,nXPos,nYPos+10,RGB(255,0,255));		
				
		}

	}
	return;
}

//设置扑克
bool CHeapCard::SetCardData(WORD wMinusHeadCount, WORD wMinusLastCount, WORD wFullCount,WORD wAddMinusMiddleCount)
{
	//设置变量
	m_wFullCount=wFullCount;
	m_wMinusHeadCount=wMinusHeadCount;
	m_wMinusLastCount=wMinusLastCount;	

	if(wAddMinusMiddleCount)
	{
		for(BYTE i=0;i<10;i++)
		{
			if(m_wMinusMiddlePos[i]==0x00)
			{
				m_wMinusMiddlePos[i]=wAddMinusMiddleCount;
				break;
			}
		}
		
	}

	return true;
}

// 重置变量
void CHeapCard::ResetMinusMiddleCount()
{
	for(BYTE i=0;i<10;i++)m_wMinusMiddlePos[i]=0x00;

}
