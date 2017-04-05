#ifndef FILTERSTRING_HEAD_FILE
#define FILTERSTRING_HEAD_FILE

#pragma once

#include "ComService.h"
#include <vector>


#define FILTER_STRING_FILENAME TEXT("\\FilterString.dat")

//脏字过滤
class COM_SERVICE_CLASS CFilterString
{
public:
	CFilterString();
	~CFilterString();
	bool IsBadString(LPCTSTR pszSrcData);//是否有不文明语言
	bool ReplaceBadString(LPCTSTR pszSrcData);//替换不文明语言
	bool LoadDict(LPCTSTR szFile);//加载关键字

private:
	TCHAR *m_DicBuf;
	std::vector<TCHAR*>	m_Strings;
};

//Ini的读取
class COM_SERVICE_CLASS CIni
{
public:
	CIni();
	~CIni();
	CIni(LPCTSTR pFile);
	void SetIniFile(LPCTSTR pFile);
	bool GetString(LPCTSTR pItemName,CString pOut);
	int GetInteger(LPCTSTR pItemName);
	bool SetString(LPCTSTR pItemName,LPCTSTR pValue);
	bool SetInteger(LPCTSTR pItemName,int nValue);
private:
	TCHAR m_IniFile[MAX_PATH];
	CRITICAL_SECTION critsec;
};

extern COM_SERVICE_CLASS CFilterString g_FilterString;

//////////////////////////////////////////////////////////////////////////

#endif//FILTERSTRING_HEAD_FILE