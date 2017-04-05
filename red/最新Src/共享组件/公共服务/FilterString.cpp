#include "StdAfx.h"
#include "FilterString.h"


CFilterString g_FilterString;
//////////////////////////////////////////////////////////////////////////

CFilterString::CFilterString()
{
	m_DicBuf = 0;
	TCHAR szFile[MAX_PATH];
	::GetModuleFileName(NULL,szFile,MAX_PATH);
	*strrchr(szFile,'\\')=0;
	strcat(szFile,FILTER_STRING_FILENAME);
	LoadDict(szFile);
}

//加载关键字
bool CFilterString::LoadDict(LPCTSTR szFile)
{
	CFile hFile;
	if (hFile.Open(szFile,CFile::modeRead,NULL)==FALSE) return false;
	ULONG nSize = (ULONG)hFile.GetLength();
	if (nSize<1) return false;

	if(m_DicBuf)
	{
		delete []m_DicBuf;
		m_DicBuf=NULL;
	}
	m_DicBuf =new char[nSize+1];
	m_DicBuf[nSize]=0;
	hFile.Read(m_DicBuf,nSize);


	TCHAR *p = m_DicBuf;
	m_Strings.push_back(p);
	while(1)
	{
		p=strstr(p+1,"\r\n");
		if(!p)break;
		*p=0;
		*(p+1)=0;
		p += 2;
		if(strlen(p)>0)
			m_Strings.push_back(p);
	}
	return true;
}

CFilterString::~CFilterString()
{
	if(m_DicBuf)
	{
		delete []m_DicBuf;
		m_DicBuf=NULL;
	}
	m_Strings.clear();
}

//是否有不文明语言
bool CFilterString::IsBadString(LPCTSTR pszSrcData)
{
	for(ULONG i=0;i<m_Strings.size();i++)
	{
		if(strlen(m_Strings[i])>0)
		{
			if(strstr(pszSrcData,m_Strings[i]))
				return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

#define INI_SECTION_NAME TEXT("Config")

CIni::CIni()
{
	::InitializeCriticalSection(&critsec);
}

CIni::~CIni()
{
	::DeleteCriticalSection(&critsec);
}

CIni::CIni(LPCTSTR pFile)
{
	::InitializeCriticalSection(&critsec);
	SetIniFile(pFile);
}

void CIni::SetIniFile(LPCTSTR pFile)
{
	::EnterCriticalSection(&critsec);
	_tcscpy(m_IniFile,pFile);
	::LeaveCriticalSection(&critsec);
}

bool CIni::GetString(LPCTSTR pItemName,CString pOut)
{
TCHAR *buf;
UINT retsize,bufsize=8092;
	pOut="";
	while(1)
	{
		buf = new TCHAR[bufsize];
		::EnterCriticalSection(&critsec);
		retsize=GetPrivateProfileString(INI_SECTION_NAME,pItemName,NULL,buf,bufsize,m_IniFile);
		::LeaveCriticalSection(&critsec);
		if(retsize<1)return false;
		if(retsize<(bufsize-1))
		{
			pOut=buf;
			delete []buf;
			break;
		}
		delete []buf;
		bufsize *= 2;
	}
	return true;
}
int CIni::GetInteger(LPCTSTR pItemName)
{
int st;
	::EnterCriticalSection(&critsec);
	st=GetPrivateProfileInt(INI_SECTION_NAME,pItemName,0,m_IniFile);
	::LeaveCriticalSection(&critsec);
	return st;
}
bool CIni::SetString(LPCTSTR pItemName,LPCTSTR pValue)
{
	::EnterCriticalSection(&critsec);
	bool ret=::WritePrivateProfileString(INI_SECTION_NAME,pItemName,pValue,m_IniFile);
	::LeaveCriticalSection(&critsec);
	return ret;
}
bool CIni::SetInteger(LPCTSTR pItemName,int nValue)
{
TCHAR buf[32];
	sprintf(buf,"%d",nValue);
	::EnterCriticalSection(&critsec);
	bool ret=::WritePrivateProfileString(INI_SECTION_NAME,pItemName,buf,m_IniFile);
	::LeaveCriticalSection(&critsec);
	return ret;
}

//////////////////////////////////////////////////////////////////////////

