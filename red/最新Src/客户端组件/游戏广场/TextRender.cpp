// TextRender.cpp: implementation of the CTextRender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TextRender.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextRenderWnd::CTextRenderWnd()
{
	// Set keywords
	strcpy( m_Keywords[0], "b1" );
	strcpy( m_Keywords[1], "b0" );
	strcpy( m_Keywords[2], "i1" );
	strcpy( m_Keywords[3], "i0" );
	strcpy( m_Keywords[4], "ul1" );
	strcpy( m_Keywords[5], "ul0" );
	strcpy( m_Keywords[6], "par" );
	strcpy( m_Keywords[7], "a1" );
	strcpy( m_Keywords[8], "a0" );

	// Set new line flag
	m_NewLine = FALSE;
	m_IsHref=false;
	m_color=0;

	// Set text alignment
	m_TextAlignment = THA_LEFT | TVA_TOP;
	ClearFormatting();
}

CTextRenderWnd::~CTextRenderWnd()
{
}

int CTextRenderWnd::PtInHref(POINT &pt)
{
	for(ULONG i=0;i<m_Rects.size();i++)
	{
		if(PtInRect(&m_Rects[i]->rect,pt))return i;
	}
	return -1;
}

TextRenderHrefInfo *CTextRenderWnd::GetHref(int nIndex)
{
	return m_Rects[nIndex];
}

void CTextRenderWnd::EnhDrawText2(HDC hDC, LPTSTR text, int nLeft,int nTop)
{
	EnhDrawText( hDC, text, (int)strlen(text), &CRect(nLeft,nTop,nLeft+1000,nTop+1000), &CRect(0,0,0,0), THA_LEFT | TVA_TOP );
}

void CTextRenderWnd::EnhDrawText(HDC hDC, LPTSTR text, int len, RECT *rect, RECT* margins, DWORD textAlignment)
{
	for(ULONG i1=0;i1<m_Rects.size();i1++)
	{
		delete []m_Rects[i1];
	}
	m_Rects.clear();
	// Get default font
	HFONT hOldFont = (HFONT)GetCurrentObject( hDC, OBJ_FONT );
	GetObject( hOldFont, sizeof(LOGFONT), &m_LogFont );

	// Save last formatting
	memcpy( &m_TempFont, &m_LogFont, sizeof(LOGFONT) );

	// Set text alignment
	m_TextAlignment = textAlignment;

	// Set default top offset
	m_OffsetTop = 0;

	// Get bounding rectangle dimensions
	int width = rect->right - rect->left - (margins->right+margins->left);
	int height = rect->bottom - rect->top - (margins->top+margins->bottom);
	SIZE size;

	// Check bounding rectangle width
	if ( width <= 0 )
		return;

	// Calculate text height
	int textHeight = CalculateTextHeight( hDC, text, len, rect, margins, width, height );

	if ( textHeight < height )
	{
		if ( m_TextAlignment & TVA_TOP )
			m_OffsetTop = 0;
		else if ( m_TextAlignment & TVA_MIDDLE )
			m_OffsetTop = (height - textHeight) / 2;
		else if ( m_TextAlignment & TVA_BOTTOM )
			m_OffsetTop = height - textHeight - 1;
	}
	else
		m_OffsetTop = 0;

	// Allocate temporary memory
	char* str = new char[len+1];
	strcpy( str, "" );

	// Parse string
	int i=0, j=0, last=0, length, kwl;
	while ( i < len )
	{
		// Get line of text
		do
		{
			// Check for the keywords
			while ( ( i < len ) && ( text[i] == '\\' ) )
			{
				// Set new text format
				kwl = SetTextFormat( text+i, (int)strlen(text+i) );

				// Skip keyword
				i += (kwl+1);

				// Check new line flag
				if ( m_NewLine == TRUE )
					break;
			}

			// Check substring length
			if ( ( i < len ) && ( m_NewLine == FALSE ) )
			{
				// Add letter from source string to temporary string
				strncat( str, text+i, 1 );
				str[strlen(str)] = '\0';

				// Get temporary string length
				GetTextExtentPoint32( hDC, str, (int)strlen(str), &size );

				// Get next letter from the source string
				i++;
			}
		}
		while ( ( i < len ) && ( size.cx < width ) && ( m_NewLine == FALSE ) );

		// Check for the middle of the word
		if ( m_NewLine == FALSE )
		{
			if ( i < len )
			{
				i--;
				if ( ( i < len ) && ( text[i] != ' ' ) )
				{
					while ( ( i > last ) && ( text[i] != ' ' ) )
						i--;

					// Skip blanks
					while ( ( i < len ) && ( text[i] == ' ' ) )
						i++;
				}
			}
			else
				i = len;
		}
		else
			i -= (kwl+1);

		// Check substring length
		length = i-last;
		if ( m_NewLine == FALSE )
		{
			if ( length == 0 )
			{
				length = len - last;
				for ( j=i; j<len; j++ )
				{
					// Check for keyword
					if ( text[j] == '\\' )
					{
						// Set new text format
						kwl = SetTextFormat( text+j, (int)strlen(text+j) );

						// Skip keyword
						if ( m_NewLine == TRUE )
						{
							i = j;
							length = j - last;
							break;
						}
					}
					if ( text[j] == ' ' )
					{
						length = j - last;
						i = j;
						break;
					}
				}
			}
		}

		// Check text top offset
		if ( ( m_OffsetTop+size.cy-1 <= height ) && ( strlen(str) != 0 ) )
		{
			// Draw formatted text
			DrawFormattedText( hDC, rect->left+margins->left, rect->top+margins->top+m_OffsetTop, width, height, text+last, length, TRUE );
		}
		if ( m_NewLine == TRUE )
		{
			m_NewLine = FALSE;
			i += (kwl+1);
		}
		m_OffsetTop += size.cy;

		if ( i == last )
			break;

		// Skip blanks
		while ( ( i < len ) && ( text[i] == ' ' ) )
			i++;
		last = i;
		
		// Clear temporary string
		strcpy( str, "" );
	}

	// Free temporary memory
	SafeDelete(str);

	// Set default font
	SelectObject( hDC, hOldFont );
}

void CTextRenderWnd::DrawFormattedText(HDC hDC, int x, int y, int width, int height, LPTSTR text, int len, BOOL draw)
{
	// Clear all previous font formatting
	ClearFormatting();
	SetTextColor(hDC,m_color);
	// Temporary font object
	HFONT hFont = NULL;

	// Allocate temporary memory
	char* str = new char[len+1];
	strcpy( str, "" );

	// Get default font height
	SIZE size;
	GetTextExtentPoint32( hDC, "A", 1, &size );

	// Get substring without formatting
	int j = 0, kwl;
	char* substring = new char[len+1];
	strcpy( substring, "" );
	while ( j < len )
	{
		// Check for keyword
		if ( text[j] == '\\' )
		{
			// Set text formatting
			kwl = SetTextFormat( text+j, len-j );

			// Get next letter
			j += (kwl+1);
		}
		else
		{
			// Append substring
			strncat( substring, text+j, 1 );

			// Get next letter
			j++;
		}
	}

	// Load last formatting
	memcpy( &m_LogFont, &m_TempFont, sizeof(LOGFONT) );

	// Get total text width
	SIZE textWidth;
	GetTextExtentPoint32( hDC, substring, (int)strlen(substring), &textWidth );

	// Check horizontal text alignment
	int offsetLeft = 0;
	if ( textWidth.cx < width )
	{
		if ( m_TextAlignment & THA_LEFT )
			offsetLeft = 0;
		else if ( m_TextAlignment & THA_CENTER )
			offsetLeft = (width - textWidth.cx) / 2;
		else if ( m_TextAlignment & THA_RIGHT )
			offsetLeft = width - textWidth.cx;
	}

	SafeDelete(substring);

	int i=0, offsetTop=0;
	while ( i < len )
	{
		// Check for keyword
		if ( text[i] == '\\' )
		{
			// Check for temporary string
			if ( strlen(str) > 0 )
			{
				// Create new font object
				if ( hFont )
					DeleteObject(hFont);
				hFont = CreateFontIndirect(&m_LogFont);
				SelectObject( hDC, hFont );

				// Set left offset
				GetTextExtentPoint32( hDC, str, (int)strlen(str), &size );
				if(m_IsHref)
				{
					TextRenderHrefInfo *ti=new TextRenderHrefInfo;
					ti->rect.left=x+offsetLeft;
					ti->rect.top=y+offsetTop;
					ti->rect.right=x+offsetLeft+size.cx;
					ti->rect.bottom=y+offsetTop+size.cy;
					memset(ti->Href,0,sizeof(ti->Href));
					strncpy(ti->Href,str,sizeof(ti->Href)-1);
					m_Rects.push_back(ti);
					//::FillRect(hDC,&ti->rect,CBrush((COLORREF)0xff));
				}

				// Check text top offset
				if ( offsetTop+size.cy-1 <= height )
				{
					// Draw part of the source text
					if ( draw == TRUE )
						TextOut( hDC, x+offsetLeft, y+offsetTop, str, (int)strlen(str) );
				}
				offsetLeft += size.cx;
				strcpy( str, "" );
			}

			// Set text formatting
			kwl = SetTextFormat( text+i, len-i );

			// Get next letter
			i += (kwl+1);
		}
		else
		{
			// Append temporary string
			strncat( str, text+i, 1 );

			// Get next letter
			i++;
		}
	}

	// Draw rest of the source text
	if ( strlen(str) > 0 )
	{
		// Create new font object
		if ( hFont )
			DeleteObject(hFont);
		hFont = CreateFontIndirect(&m_LogFont);
		SelectObject( hDC, hFont );

		// Check text top offset
		if ( offsetTop+size.cy-1 <= height )
		{
			// Draw text
			if ( draw == TRUE )
				TextOut( hDC, x+offsetLeft, y+offsetTop, str, (int)strlen(str) );
		}
	}

	// Save last formatting
	memcpy( &m_TempFont, &m_LogFont, sizeof(LOGFONT) );

	// Delete font object
	if ( hFont )
		DeleteObject(hFont);

	// Free temporary memory
	SafeDelete(str);
}

int CTextRenderWnd::SetTextFormat(LPTSTR text, int len)
{
	// Set default keyword length
	int result = 0;
	// Allocate temporary memory
	char* keyword = new char[len+1];

	// Parse source string
	int k = 1;
	while ( k < len )
	{
		// "\" and " " - escape sequences
		if ( ( text[k] == '\\' ) || ( text[k] == ' ' ) )
			break;

		// Append keyword string
		strncpy( keyword, text+1, k );
		keyword[k] = '\0';

		// Check keyword string
		if ( CheckKeyword(keyword) == true )
		{
			result = (int)strlen(keyword);
			break;
		}

		k++;
	}

	// Free temporary memory
	SafeDelete(keyword);

	// Return keyword length
	return result;
}

bool CTextRenderWnd::CheckKeyword(LPTSTR keyword)
{
	bool result = false;

	for ( int i=0; i<KW_NUMBER; i++ )
	{
		// Check keyword
		if ( strcmp( keyword, m_Keywords[i] ) == 0 )
		{
			// Find keyword
			if ( strcmp( keyword, "b1" ) == 0 )
				m_LogFont.lfWeight = FW_BOLD;
			else if ( strcmp( keyword, "b0" ) == 0 )
				m_LogFont.lfWeight = FW_NORMAL;
			else if ( strcmp( keyword, "i1" ) == 0 )
				m_LogFont.lfItalic = TRUE;
			else if ( strcmp( keyword, "i0" ) == 0 )
				m_LogFont.lfItalic = FALSE;
			else if ( strcmp( keyword, "ul1" ) == 0 )
				m_LogFont.lfUnderline = TRUE;
			else if ( strcmp( keyword, "ul0" ) == 0 )
				m_LogFont.lfUnderline = FALSE;
			else if ( strcmp( keyword, "par" ) == 0 )
				m_NewLine = TRUE;
			else if ( strcmp( keyword, "a1" ) == 0 )
				m_IsHref = TRUE;
			else if ( strcmp( keyword, "a0" ) == 0 )
				m_IsHref = FALSE;

			// Keyword recognized
			result = true;
			break;
		}
	}

	return result;
}

void CTextRenderWnd::ClearFormatting()
{
	// Clear font formatting
	m_LogFont.lfWeight = FW_NORMAL;
	m_LogFont.lfItalic = FALSE;
	m_LogFont.lfUnderline = FALSE;

	m_LogFont.lfCharSet=DEFAULT_CHARSET;
	SetFontFace("宋体",12);
}

void CTextRenderWnd::SetFontFace(const char *szFont,LONG nHeight)
{
	m_nHeight=nHeight;
	strcpy(m_szFontFace,szFont);

	m_LogFont.lfHeight=m_nHeight;
	m_LogFont.lfWidth=m_nHeight/2+ (m_nHeight%2 ? 1 : 0);
	strcpy(m_LogFont.lfFaceName,m_szFontFace);
}

int CTextRenderWnd::CalculateTextHeight(HDC hDC, char *text, int len, RECT *rect, RECT* margins, int width, int height)
{
	// Get default font
	HFONT hOldFont = (HFONT)GetCurrentObject( hDC, OBJ_FONT );
	GetObject( hOldFont, sizeof(LOGFONT), &m_LogFont );
	SetFontFace(m_szFontFace,m_nHeight);

	// Save last formatting
	memcpy( &m_TempFont, &m_LogFont, sizeof(LOGFONT) );

	// Set default top offset
	m_OffsetTop = 0;

	// Allocate temporary memory
	char* str = new char[len+1];
	strcpy( str, "" );

	// Parse string
	int i=0, j=0, last=0, length, kwl;
	SIZE size;
	while ( i < len )
	{
		// Get line of text
		do
		{
			// Check for the keywords
			while ( ( i < len ) && ( text[i] == '\\' ) )
			{
				// Set new text format
				kwl = SetTextFormat( text+i, (int)strlen(text+i) );

				// Skip keyword
				i += (kwl+1);

				// Check new line flag
				if ( m_NewLine == TRUE )
					break;
			}

			// Check substring length
			if ( ( i < len ) && ( m_NewLine == FALSE ) )
			{
				// Add letter from source string to temporary string
				strncat( str, text+i, 1 );
				str[strlen(str)] = '\0';

				// Get temporary string length
				GetTextExtentPoint32( hDC, str, (int)strlen(str), &size );

				// Get next letter from the source string
				i++;
			}
		}
		while ( ( i < len ) && ( size.cx < width ) && ( m_NewLine == FALSE ) );

		// Check for the middle of the word
		if ( m_NewLine == FALSE )
		{
			if ( i < len )
			{
				i--;
				if ( ( i < len ) && ( text[i] != ' ' ) )
				{
					while ( ( i > last ) && ( text[i] != ' ' ) )
						i--;

					// Skip blanks
					while ( ( i < len ) && ( text[i] == ' ' ) )
						i++;
				}
			}
			else
				i = len;
		}
		else
			i -= (kwl+1);

		// Check substring length
		length = i-last;
		if ( m_NewLine == FALSE )
		{
			if ( length == 0 )
			{
				length = len - last;
				for ( j=i; j<len; j++ )
				{
					// Check for keyword
					if ( text[j] == '\\' )
					{
						// Set new text format
						kwl = SetTextFormat( text+j, (int)strlen(text+j) );

						// Skip keyword
						if ( m_NewLine == TRUE )
						{
							i = j;
							length = j - last;
							break;
						}
					}
					if ( text[j] == ' ' )
					{
						length = j - last;
						i = j;
						break;
					}
				}
			}
		}

		// Check text top offset
		if ( strlen(str) != 0 )
		{
			// Draw formatted text
			DrawFormattedText( hDC, rect->left+margins->left, rect->top+margins->top+m_OffsetTop, width, height, text+last, length, FALSE );
		}
		if ( m_NewLine == TRUE )
		{
			m_NewLine = FALSE;
			i += (kwl+1);
		}
		m_OffsetTop += size.cy;

		if ( i == last )
			break;

		// Skip blanks
		while ( ( i < len ) && ( text[i] == ' ' ) )
			i++;
		last = i;

		// Clear temporary string
		strcpy( str, "" );
	}

	// Free temporary memory
	SafeDelete(str);

	// Set default font
	SelectObject( hDC, hOldFont );

	return m_OffsetTop;
}

int g_BtnIndex=-1;
bool CTextRenderWnd::OnSetCursor(CWnd* pWnd)
{
	CPoint point;
	GetCursorPos(&point);
	pWnd->ScreenToClient(&point);

	int BtnIndex=PtInHref(point);
#if 0
	if(BtnIndex>=0)
	{	//是OnMouseOver吗?
		SetCursor(LoadCursor(0,MAKEINTRESOURCE(32649)));
		return true;
	}
#else
///*--------------------------------------------------------------------------(+)
	if(BtnIndex>=0)
	{	//是OnMouseOver吗?
		SetCursor(LoadCursor(0,MAKEINTRESOURCE(32649)));
		if(g_BtnIndex != (-1))return true;//如果画过了就不要画了
		g_BtnIndex=BtnIndex;
	}
	else
	{
		BtnIndex=g_BtnIndex;
		g_BtnIndex=-1;
	}
	if(BtnIndex>=0)
	{
		TextRenderHrefInfo *p=GetHref(BtnIndex);
		if(p)
		{
			HDC hdc=GetWindowDC(pWnd->GetSafeHwnd());
			for(int i=p->rect.left;i<p->rect.right;i++)
			{
				for(int j=p->rect.top;j<p->rect.bottom;j++)
				{
					if(g_BtnIndex)
					{
						if(::GetPixel(hdc,i+1,j+1)==0x0102ff)
						{
							::SetPixel(hdc,i+1,j+1,m_color);
						}
					}
					else
					{
						if(::GetPixel(hdc,i+1,j+1)==m_color)
						{
							::SetPixel(hdc,i+1,j+1,0x0102ff);
						}
					}
				}
			}
			ReleaseDC(pWnd->GetSafeHwnd(),hdc);
		}
		return true;
	}
//--------------------------------------------------------------------------(-)*/
#endif
	SetCursor(LoadCursor(0,MAKEINTRESOURCE(32512)));
	return false;
}
void CTextRenderWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	int BtnIndex=PtInHref(point);
	if(BtnIndex>=0)
	{
		TextRenderHrefInfo *p;
		p=GetHref(BtnIndex);
		if(p)
		{
			CString szUrl;
			szUrl="http://"ServerDomain"/GameLink/?";
			szUrl += p->Href;
			ShellExecute(NULL,TEXT("open"),szUrl,NULL,NULL,SW_SHOW);
			return;
		}
	}
}









