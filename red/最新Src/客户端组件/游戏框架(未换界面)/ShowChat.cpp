// ShowChat.cpp : 实现文件
//

#include "stdafx.h"
#include "GameFrame.h"
#include "GameFrameDlg.h"

#include "ShowChat.h"
#include "resource.h"

// CShowChat

CShowChat ShowChat;

LPPOINT GAME_FRAME_CLASS GetChatPt()
{
	return &ShowChat.PosPt[0];
}

//字符转ASCII码 output输出的字符串 longth为输出的行数 howlongth 每行的长度
int CharASCII(CString m_input,CString * output,int nLineLen)
{
int nIndex=0;
LPCSTR p = (LPCSTR)m_input;
int nLen = m_input.GetLength();
	for(int k=0;k<nLen;k++)
	{
		if(output[nIndex].GetLength()>nLineLen)
		{
			nIndex++;
		}
		BYTE c = *(p + k);
		if(!c)break;
		if(c>127)
		{
			output[nIndex] += c;
			output[nIndex] += *(p + k + 1);
			k++;
		}
		else
		{
			output[nIndex] += c;
		}
		if(!c)break;
	}
	return nIndex;
}


CShowChat::CShowChat()
{
	//加载资源
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);

	//m_ChatImg_T.LoadResource(FindResource(hInstance,(LPCSTR)IDB_MSG_T,RT_BITMAP),CXIMAGE_FORMAT_BMP,hInstance);
	//m_ChatImg_B.LoadResource(FindResource(hInstance,(LPCSTR)IDB_MSG_B,RT_BITMAP),CXIMAGE_FORMAT_BMP,hInstance);
	//m_ChatImg_L.LoadResource(FindResource(hInstance,(LPCSTR)IDB_MSG_L,RT_BITMAP),CXIMAGE_FORMAT_BMP,hInstance);
	//m_ChatImg_R.LoadResource(FindResource(hInstance,(LPCSTR)IDB_MSG_R,RT_BITMAP),CXIMAGE_FORMAT_BMP,hInstance);
	//m_ChatImg_BL.LoadResource(FindResource(hInstance,(LPCSTR)IDB_MSG_BL,RT_BITMAP),CXIMAGE_FORMAT_BMP,hInstance);
	//m_ChatImg_BR.LoadResource(FindResource(hInstance,(LPCSTR)IDB_MSG_BR,RT_BITMAP),CXIMAGE_FORMAT_BMP,hInstance);
	m_ChatImg_T.SetLoadInfo(IDB_MSG_T,hInstance);
	m_ChatImg_B.SetLoadInfo(IDB_MSG_B,hInstance);
	m_ChatImg_L.SetLoadInfo(IDB_MSG_L,hInstance);
	m_ChatImg_R.SetLoadInfo(IDB_MSG_R,hInstance);
	m_ChatImg_BL.SetLoadInfo(IDB_MSG_BL,hInstance);
	m_ChatImg_BR.SetLoadInfo(IDB_MSG_BR,hInstance);


	for(WORD i=0;i<MAX_PLAYER;i++)
	{
		m_szMsg[i] = "";
		m_lStartTick[i]=0;
		m_lTime[i]=0;
	}

	PosPt[SCP_T].x = 320;
	PosPt[SCP_T].y = 90;

	PosPt[SCP_TL].x = 120;
	PosPt[SCP_TL].y = 90;

	PosPt[SCP_L].x = 120;
	PosPt[SCP_L].y = 290;

	PosPt[SCP_BL].x = 120;
	PosPt[SCP_BL].y = 320;

	PosPt[SCP_B].x = 320;
	PosPt[SCP_B].y = 320;

	PosPt[SCP_BR].x = 500;
	PosPt[SCP_BR].y = 320;

	PosPt[SCP_R].x = 500;
	PosPt[SCP_R].y = 290;

	PosPt[SCP_TR].x = 500;
	PosPt[SCP_TR].y = 90;

}

CShowChat::~CShowChat()
{
}

bool CShowChat::ShowChat(LPCTSTR szString,tagUserData *pUser,int lTime)
{
	//转换椅子
	WORD vChair = GFDlg->XChair(pUser->wChairID);
	m_szMsg[vChair] = szString;
	m_lStartTick[vChair] = GetTickCount();
	m_lTime[vChair] = lTime;
	return true;
}

//要放在相应的OnPaint事件中
void CShowChat::OnDraw(CDC *pDC)
{
	for(WORD i=0;i<MAX_PLAYER;i++)
	{
		if(m_lTime[i]>0)
		{
			//超时就隐藏
			if(abs((int)GetTickCount() - m_lStartTick[i])>=m_lTime[i])
			{
				m_lTime[i] = 0;
			}
			else
			{
				int x,y,x1,y1;
				//CxImage *pMsgImage=GetChairPos(i,x,y,x1,y1);
				CSkinImage *pMsgImage=GetChairPos(i,x,y,x1,y1);
				CImageHandle ImageMLHandle(pMsgImage);
				if(pMsgImage)
				{
					pMsgImage->AlphaDrawImage(pDC,x, y, pMsgImage->GetWidth(), pMsgImage->GetHeight(),0,0,RGB(255,0,255));
				//	pMsgImage->TransparentBlt(pDC->m_hDC,20,(0xff00ff),x,y);
				}
				//CString s;
				//s.Format("%d",GFDlg->XChair(i));
				//pDC->TextOut(x+x1,y+y1,m_szMsg[i]);

				CString OutPut[20];

				UINT nAlign = pDC->GetTextAlign();
				pDC->SetTextAlign(0);

				int nLines = CharASCII(m_szMsg[i],&OutPut[0],18);
				nLines = __min(nLines+1,3);
				for(WORD j=0;j<nLines;j++)
				{
					pDC->TextOut(x+x1,y+y1+j*14,OutPut[j]);
				}
				pDC->SetTextAlign(nAlign);
			}
		}
	}
}


//CxImage *CShowChat::GetChairPos(WORD wViewChair,int &x,int &y,int &Word_x,int &Word_y)
CSkinImage *CShowChat::GetChairPos(WORD wViewChair,int &x,int &y,int &Word_x,int &Word_y)
{
	x = y = 0;
	//CxImage *pImage=&m_ChatImg_B;
	CSkinImage *pImage=&m_ChatImg_B;
	const WORD wChairCount = GFDlg->GetChairCount();
	Word_x = 10;
	Word_y = 8;

	switch(wChairCount)
	{
	case 2:
		{
			switch(wViewChair)
			{
			case 0:
				return 0;
			case 1:
				return 0;
			}
		}
		return 0;
	case 3:
		{
			switch(wViewChair)
			{
			case 0:
				x = PosPt[SCP_L].x;
				y = PosPt[SCP_L].y;
				Word_x = 35;
				return &m_ChatImg_L;
			case 1:
				x = PosPt[SCP_B].x;
				y = PosPt[SCP_B].y;
				return &m_ChatImg_B;
			case 2:
				x = PosPt[SCP_R].x;
				y = PosPt[SCP_R].y;
				return &m_ChatImg_R;
			}
		}
		return 0;
	case 4:
		{
			switch(wViewChair)
			{
			case 0:
				x = PosPt[SCP_T].x;;
				y = PosPt[SCP_T].y;
				Word_x = 10;
				Word_y = 30;
				return &m_ChatImg_T;
			case 1:
				x = PosPt[SCP_R].x;
				y = PosPt[SCP_R].y;
				return &m_ChatImg_R;
			case 2:
				x = PosPt[SCP_B].x;
				y = PosPt[SCP_B].y;
				return &m_ChatImg_B;
			case 3:
				x = PosPt[SCP_L].x;
				y = PosPt[SCP_L].y;
				Word_x = 35;
				return &m_ChatImg_L;
			}
		}
		return 0;
	case 6:
		{
			switch(wViewChair)
			{
			case 0:
				x = PosPt[SCP_R].x;
				y = PosPt[SCP_R].y;
				return &m_ChatImg_R;
			case 1:
				x = PosPt[SCP_TR].x;
				y = PosPt[SCP_TR].y;
				return &m_ChatImg_R;
			case 2:
				x = PosPt[SCP_T].x;
				y = PosPt[SCP_T].y;
				Word_x = 10;
				Word_y = 30;
				return &m_ChatImg_T;
			case 3:
				x = PosPt[SCP_TL].x;
				y = PosPt[SCP_TL].y;
				Word_x = 35;
				return &m_ChatImg_L;
			case 4:
				x = PosPt[SCP_L].x;
				y = PosPt[SCP_L].y;
				Word_x = 35;
				return &m_ChatImg_L;
			case 5:
				x = PosPt[SCP_B].x;
				y = PosPt[SCP_B].y;
				return &m_ChatImg_B;
			}
		}
		return 0;
	}
	return 0;
}
