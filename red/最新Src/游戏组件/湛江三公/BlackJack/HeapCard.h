#pragma once

//堆立扑克
class CHeapCard
{
	//控制变量
protected:
	CPoint							m_ControlPoint;						//基准位置
	CSkinImage						m_ImageBack;						//资源图片

	//扑克变量
protected:
	WORD							m_wFullCount;						//全满数目
	WORD							m_wMinusHeadCount;					//头部空缺
	WORD							m_wMinusLastCount;					//尾部空缺	
	WORD							m_wMinusMiddlePos[10];				//中部空缺

	//函数定义
public:
	//构造函数
	CHeapCard();
	//析构函数
	virtual ~CHeapCard();

	//功能函数
public:
	//绘画扑克
	void DrawCardControl(CDC * pDC);
	//设置扑克	
	bool SetCardData(WORD wMinusHeadCount, WORD wMinusLastCount, WORD wFullCount,WORD wAddMinusMiddleCount);	
	

	//控件控制
public:	
	//基准位置
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
	// 重置变量
	void ResetMinusMiddleCount(void);
};