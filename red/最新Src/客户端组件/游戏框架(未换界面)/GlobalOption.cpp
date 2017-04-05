#include "StdAfx.h"
#include "GlobalOption.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

#define REG_GLOBAL_OPTIONS			TEXT("GlobalOption")				//全局配置

//////////////////////////////////////////////////////////////////////////

CGlobalOption						g_GlobalOption;						//全局配置
//////////////////////////////////////////////////////////////////////////

//构造函数
CGlobalOption::CGlobalOption()
{
	//消息设置
	m_bShowInOutMessage=true;

	//颜色设置
	m_crChatTX=RGB(0,0,0);
	m_crMsgName=RGB(0,0,200);
	m_crMsgNormal=RGB(0,0,0);
	m_crMsgSysHead=RGB(200,0,0);
	m_crMsgSysString=RGB(0,0,200);

	m_ScreenW=0;
	GetDisplay(m_ScreenW,m_ScreenH,m_ScreenBits,m_ScreenFrequency);


	m_Font.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("宋体"));
	return;
}

//析构函数
CGlobalOption::~CGlobalOption()
{
	SetDisplayDefault();
}

//加载设置
void CGlobalOption::LoadOptionParameter()
{
	//变量定义
	CWinApp * pApp=AfxGetApp();

	//消息设置
	m_bShowInOutMessage=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("ShowInOutMessage"),m_bShowInOutMessage)?true:false;

	//颜色设置
	m_crChatTX=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("ChatTX"),m_crChatTX);
	m_crMsgName=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgName"),m_crMsgName);
	m_crMsgNormal=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgNormal"),m_crMsgNormal);
	m_crMsgSysHead=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgSysHead"),m_crMsgSysHead);
	m_crMsgSysString=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgSysString"),m_crMsgSysString);

	return;
}

//保存设置
void CGlobalOption::SaveOptionParameter()
{
	//变量定义
	CWinApp * pApp=AfxGetApp();

	//消息设置
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("ShowInOutMessage"),m_bShowInOutMessage);

	//颜色设置
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("ChatTX"),m_crChatTX);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgName"),m_crMsgName);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgNormal"),m_crMsgNormal);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgSysHead"),m_crMsgSysHead);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgSysString"),m_crMsgSysString);

	return;
}

bool CGlobalOption::GetDisplay(int &nWidth,int &nHeight,int &nBits,int &nFrequency)
{
	DEVMODE	devmode;
	if (::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode))
	{
		nWidth = devmode.dmPelsWidth;
		nHeight = devmode.dmPelsHeight;
		nBits = devmode.dmBitsPerPel;
		nFrequency = devmode.dmDisplayFrequency;
		return true;
	}
	return false;
}

void CGlobalOption::SetDisplayDefault()
{
	SetDisplay(m_ScreenW,m_ScreenH,m_ScreenBits,m_ScreenFrequency);
}

bool CGlobalOption::SetDisplay(int nWidth,int nHeight,int nBits,int nFrequency)
{
	DEVMODE	devmode;
	BOOL  bRetVal=true;
	DWORD iMode = 0;

	while (1)
	{
		BOOL bRetVal = ::EnumDisplaySettings(NULL, iMode, &devmode);
		if (!bRetVal)break;
		if(devmode.dmPelsWidth == nWidth && devmode.dmPelsHeight==nHeight && devmode.dmBitsPerPel==nBits && devmode.dmDisplayFrequency == nFrequency)
		{
			devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;
			::ChangeDisplaySettings(&devmode, 0);
			return true;
		}
		iMode++;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
