#include "StdAfx.h"
#include "Mmsystem.h"
#include ".\waveshow.h"

CWaveShow g_WaveShow;


DWORD WINAPI DoPlayBufferSoundThread(LPVOID lpThreadParameter)
{
	::PlaySound((LPCSTR)lpThreadParameter,NULL,SND_SYNC|SND_MEMORY);
	delete []lpThreadParameter;
	return 0;
}

CWaveShow::CWaveShow(void)
{
	m_data=NULL;
	srand(GetTickCount());
}

CWaveShow::~CWaveShow(void)
{
	Clear();
}

LPCSTR CWaveShow::GetEventWaveFile(int nEvent)
{
	if(!m_data)return NULL;
	std::vector<tagWaveInfo*> wi;

	for(ULONG i=0;i<m_WaveInfo.size();i++)
	{
		if(nEvent == m_WaveInfo[i]->nEvent)
		{
			wi.push_back(m_WaveInfo[i]);
		}
	}

	if(wi.size()<1)
		return NULL;

	LPCSTR p = wi[rand()%wi.size()]->szFile;
	wi.clear();
	return p;
}

LPCSTR CWaveShow::GetEventWaveFile(int nEvent,char *szAuthorName)
{
	if(!m_data)return NULL;
	std::vector<tagWaveInfo*> wi;
	for(ULONG i=0;i<m_WaveInfo.size();i++)
	{
		if(nEvent == m_WaveInfo[i]->nEvent)
		{
			if(strncmp(m_WaveInfo[i]->szFile,szAuthorName,lstrlen(szAuthorName))==0)
			{
				wi.push_back(m_WaveInfo[i]);
			};
		}
	}

	if(wi.size()<1)
		return NULL;

	LPCSTR p = wi[rand()%wi.size()]->szFile;
	wi.clear();
	return p;
}

bool CWaveShow::PlayWav(int nEvent)
{
	if(nEvent<1)return false;
	LPCSTR p = GetEventWaveFile(nEvent);
	if(!p)return false;
	return PlayWav(p);
}

bool CWaveShow::PlayWav(int nEvent,char *szAuthorName)
{
	if(nEvent<1)return false;
	return PlayWav(GetEventWaveFile(nEvent,szAuthorName));
}

//播放声音
bool CWaveShow::PlayWav(LPCTSTR pszSoundName)
{
	int nLen;
	if(!pszSoundName)
		return false;
	BYTE *p=m_fpack.GetFile(pszSoundName,nLen);
	if(p)
	{
		CloseHandle(CreateThread(NULL,0,DoPlayBufferSoundThread,p,0,NULL));
		return true;
	}
	return false;
}
bool CWaveShow::Clear()
{
	for(ULONG i=0;i<m_WaveInfo.size();i++)
	{
		delete m_WaveInfo[i];
	}
	m_WaveInfo.clear();

	if(m_data)delete []m_data;
	m_data = 0;

	return true;
}


//读取参数信息
bool CWaveShow::SetScript(LPCSTR pszScript)
{
	Clear();
	//::MessageBox(0,pszScript,0,0);
	int nLen = lstrlen(pszScript);
	if(nLen<1)return false;
	m_data = new char[nLen+1];
	memcpy(m_data,pszScript,nLen);
	m_data[nLen]=0;

	return LexWaveInfo(m_data);

}

//设制当前WavPackageFile
bool CWaveShow::SetWaveFile(LPCSTR pszWavPackageFile)
{
	bool bRet = m_fpack.Open(pszWavPackageFile);
	if(!bRet)::MessageBox(0,"声音包打开失败。。",0,0);
	return bRet;
}

//分析文件
bool CWaveShow::LexWaveInfo(char *pData)
{
	char *p1,*p2;
	p1= pData;

	p2 = strstr(p1,"\r\n[");

	while(p2)
	{
		p2 += 2;
		p1 = strstr(p2,"\r\n[");
		if(p1)
		{
			*p1 = 0;
		}
		GetWaveEvent(p2);
		
		if(!p1)break;
		p2 = p1;
	}
	if(m_WaveInfo.size()<3)::MessageBox(0,"声音脚本载入失败！可能听不到自已的声音",0,0);

	return true;
}

//分析字串
bool CWaveShow::GetWaveEvent(char *pData)
{
char *p1,*p2;
int nEvent=0;
	p1 = strchr(pData,',');
	if(!p1)return false;
	p1 ++;
	p2 = strchr(p1,']');
	if(!p2)return false;
	*p2=0;
	nEvent = atoi(p1);//得到事件ID

	if(nEvent<1)return false;


	p2 ++;

	while(1)
	{
		p1 = strstr(p2,"\r\n\"");
		if(!p1)return false;
		p1 += 3;
		p2 = strchr(p1,'\"');
		if(!p2)return false;
		*p2 = 0;
		//-----------(+)
		tagWaveInfo *w = new tagWaveInfo;
		w->nEvent = nEvent;
		w->szFile = p1;
		m_WaveInfo.push_back(w);
		//-----------(-)
		p2 ++;
	}
	return true;
}



