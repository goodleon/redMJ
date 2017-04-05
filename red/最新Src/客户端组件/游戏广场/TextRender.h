// TextRender.h: interface for the CTextRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTRENDER_H__2CF5BC3D_A5F5_490B_B96A_9E049FA1A11E__INCLUDED_)
#define AFX_TEXTRENDER_H__2CF5BC3D_A5F5_490B_B96A_9E049FA1A11E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

/* Tptal keyword number */
#define KW_NUMBER	9

/* Horizontal alignment flags */
#define THA_LEFT	1
#define THA_CENTER	2
#define THA_RIGHT	4

/* Vertical alignment flags */
#define TVA_TOP		8
#define TVA_MIDDLE	16
#define TVA_BOTTOM	32


struct TextRenderHrefInfo
{
	RECT rect;
	char Href[256];
};



class CTextRenderWnd
{
private:
	DWORD m_TextAlignment;
	int m_OffsetTop;
	BOOL m_NewLine;
	LOGFONT m_LogFont;
	LOGFONT m_TempFont;

	CBrush       m_Brush;
	//字体属性
	char m_szFontFace[LF_FACESIZE];
	LONG m_nHeight;
	BYTE m_IsHref;

	std::vector<TextRenderHrefInfo*> m_Rects;

	char m_Keywords[9][10];//语法关键字

	void DrawFormattedText(HDC hDC, int x, int y, int width, int height, LPTSTR text, int len, BOOL draw);
	int SetTextFormat(LPTSTR text, int len);
	bool CheckKeyword(LPTSTR keyword);
	void ClearFormatting();

public:
	CTextRenderWnd();
	virtual ~CTextRenderWnd();
	void SetFontFace(const char *szFont,LONG nHeight);

	//int BtnIndex=m_TextRender.PtInHref(point);
	//if(BtnIndex>=0)::MessageBox(0,"1",0,0);
	int PtInHref(POINT &pt);
	TextRenderHrefInfo *GetHref(int nIndex);

	COLORREF m_color;
	int CalculateTextHeight(HDC hDC, char* text, int len, RECT *rect, RECT* margins, int width, int height);
	void EnhDrawText(HDC hDC, LPTSTR text, int len, RECT* rect, RECT* margins, DWORD textAlignment);
	void EnhDrawText2(HDC hDC, LPTSTR text, int nLeft,int nTop);
public:

	bool OnSetCursor(CWnd* pWnd);
	void OnLButtonDown(UINT nFlags, CPoint point);
};

#endif // !defined(AFX_TEXTRENDER_H__2CF5BC3D_A5F5_490B_B96A_9E049FA1A11E__INCLUDED_)
