#include "StdAfx.h"
#include "SkinImage.h"

/////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinImage::CSkinImage(void)
{
	m_nRefCount=0;
	m_bLoadAlways=false;
}

//析构函数
CSkinImage::~CSkinImage(void)
{
	Destroy();
}

//增加引用
int CSkinImage::LockResource()
{
	if (m_nRefCount==0)
	{
		if (m_LoadInfo.hResourceDLL==NULL) Load(m_LoadInfo.strFileName);
		else LoadFromResource(m_LoadInfo.hResourceDLL,m_LoadInfo.uResourceID);
		if (IsNull()) return 0;
	}
	return ++m_nRefCount;
}

//减少引用
int CSkinImage::UnLockResource()
{
	try
	{
		if (m_nRefCount>0) m_nRefCount--;
		if (m_nRefCount==0) Destroy();
	}
	catch (...) {}
	return m_nRefCount;
}

//是否设置加载信息
bool CSkinImage::IsSetLoadInfo()
{
	return ((m_LoadInfo.hResourceDLL)||(m_LoadInfo.strFileName.IsEmpty()==false));
}

//清除加载参数
bool CSkinImage::RemoveLoadInfo()
{
	m_LoadInfo.uResourceID=0;
	m_LoadInfo.hResourceDLL=NULL;
	m_LoadInfo.strFileName.Empty();
	return true;
}

//获取加载参数
bool CSkinImage::GetLoadInfo(tagImageLoadInfo & LoadInfo)
{
	LoadInfo=m_LoadInfo;
	return IsSetLoadInfo();
}

//设置加载参数
bool CSkinImage::SetLoadInfo(LPCTSTR pszFileName, bool bLoadAlways)
{
	//保存变量
	int nOldRefCount=m_nRefCount;
	bool bOldLoadAlways=m_bLoadAlways;

	//清理资源
	Destroy();
	m_nRefCount=0;
	m_bLoadAlways=bLoadAlways;

	//设置变量
	m_LoadInfo.uResourceID=0;
	m_LoadInfo.hResourceDLL=NULL;
	m_LoadInfo.strFileName=pszFileName;

	//恢复锁定
	if (bOldLoadAlways) nOldRefCount--;
	if (m_bLoadAlways) nOldRefCount++;
	while (nOldRefCount>0)
	{
		nOldRefCount--;
		LockResource();
	};

	return true;
}

//设置加载参数
bool CSkinImage::SetLoadInfo(UINT uResourceID, HINSTANCE hResourceDLL, bool bLoadAlways)
{
	//保存变量
	int nOldRefCount=m_nRefCount;
	bool bOldLoadAlways=m_bLoadAlways;

	//清理资源
	Destroy();
	m_nRefCount=0;
	m_bLoadAlways=bLoadAlways;
	m_LoadInfo.strFileName.Empty();

	//设置变量
	m_LoadInfo.uResourceID=uResourceID;
	m_LoadInfo.hResourceDLL=hResourceDLL;

	//恢复锁定
	if (bOldLoadAlways) nOldRefCount--;
	if (m_bLoadAlways) nOldRefCount++;
	while (nOldRefCount>0)
	{
		nOldRefCount--;
		LockResource();
	};

	return true;
}

//透明绘画
bool CSkinImage::AlphaDrawImage(CDC * pDestDC, int xDest, int yDest, COLORREF crTrans)
{
	return AlphaDrawImage(pDestDC,xDest,yDest,GetWidth(),GetHeight(),0,0,crTrans);
}

//透明绘画
bool CSkinImage::AlphaDrawImage(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, int xSrc, int ySrc, COLORREF crTrans)
{
	//效验状态
	if (IsNull()) return false;
	//this->BitBlt(pDestDC->m_hDC,0,0,SRCCOPY);
	//return true;
	//建立 DC
	CDC DCImage;
	DCImage.CreateCompatibleDC(pDestDC);
	DCImage.SelectObject(HBITMAP(*this));
	DCImage.SetBkColor(crTrans);
	DCImage.SetBkMode(TRANSPARENT);

	//建立掩码图
	CDC DCMask;
	CBitmap BMPMask;
	BMPMask.CreateBitmap(cxDest,cyDest,1,1,NULL);
	DCMask.CreateCompatibleDC(pDestDC);
	DCMask.SelectObject(&BMPMask);
	DCMask.BitBlt(0,0,cxDest,cyDest,&DCImage,xSrc,ySrc,SRCCOPY);

	//绘画目标图
	CDC DCDest;
	CBitmap BMPDest;
	DCDest.CreateCompatibleDC(pDestDC);
	BMPDest.CreateCompatibleBitmap(pDestDC,cxDest,cyDest);
	DCDest.SelectObject(&BMPDest);
	DCDest.BitBlt(0,0,cxDest,cyDest,pDestDC,xDest,yDest,SRCCOPY);
	DCDest.BitBlt(0,0,cxDest,cyDest,&DCImage,xSrc,ySrc,SRCINVERT);
	DCDest.BitBlt(0,0,cxDest,cyDest,&DCMask,0,0,SRCAND);
	DCDest.BitBlt(0,0,cxDest,cyDest,&DCImage,xSrc,ySrc,SRCINVERT);
	pDestDC->BitBlt(xDest,yDest,cxDest,cyDest,&DCDest,0,0,SRCCOPY);

	//清理资源
	DCMask.DeleteDC();
	DCDest.DeleteDC();
	DCImage.DeleteDC();
	BMPMask.DeleteObject();
	BMPDest.DeleteObject();
	
	return true;
}

//透明绘画
bool CSkinImage::AlphaDrawImageEx(CDC * pDestDC, int xDest, int yDest, COLORREF crTrans)
{
	return AlphaDrawImageEx(pDestDC,xDest,yDest,GetWidth(),GetHeight(),0,0,crTrans);
}

//透明绘画
bool CSkinImage::AlphaDrawImageEx(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, int xSrc, int ySrc, COLORREF crTrans)
{
	//效验状态
	if (IsNull()) return false;

	//建立 DC
	CDC DCImage;
	DCImage.CreateCompatibleDC(pDestDC);
	DCImage.SelectObject(HBITMAP(*this));
	DCImage.SetBkColor(crTrans);
	DCImage.SetBkMode(TRANSPARENT);

	//建立掩码图
	CDC DCMask;
	CBitmap BMPMask;
	BMPMask.CreateBitmap(cxDest,cyDest,1,1,NULL);
	DCMask.CreateCompatibleDC(pDestDC);
	DCMask.SelectObject(&BMPMask);
	DCMask.BitBlt(0,0,cxDest,cyDest,&DCImage,xSrc,ySrc,SRCCOPY);

	//绘画目标图
	pDestDC->BitBlt(xDest,yDest,cxDest,cyDest,pDestDC,xDest,yDest,SRCCOPY);
	pDestDC->BitBlt(xDest,yDest,cxDest,cyDest,&DCImage,xSrc,ySrc,SRCINVERT);
	pDestDC->BitBlt(xDest,yDest,cxDest,cyDest,&DCMask,0,0,SRCAND);
	pDestDC->BitBlt(xDest,yDest,cxDest,cyDest,&DCImage,xSrc,ySrc,SRCINVERT);

	//清理资源
	DCMask.DeleteDC();
	DCImage.DeleteDC();
	BMPMask.DeleteObject();
	
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CImageHandle::CImageHandle(CSkinImage * pImageObject)
{
	m_pImage=NULL;
	AttachResource(pImageObject);
	return;
}

//析构函数
CImageHandle::~CImageHandle()
{
	DetchResource();
	return;
}

//绑定资源
bool CImageHandle::AttachResource(IGDIResource * pImageObject)
{
	DetchResource();
	if (pImageObject)
	{
		CSkinImage * pSkinImage=(CSkinImage *)pImageObject;
		if (pImageObject->LockResource()>0)
		{
			m_pImage=pSkinImage;
			return true;
		}
	}
	return false;
}

//解除绑定
bool CImageHandle::DetchResource()
{
	if (m_pImage) 
	{
		m_pImage->UnLockResource();
		m_pImage=NULL;
		return true;
	} 
	return false;
}

//是否绑定
bool CImageHandle::IsAttached()
{
	return (m_pImage!=NULL);
}

//是否有效
bool CImageHandle::IsResourceValid()
{
	return ((m_pImage)&&(m_pImage->IsNull()==false));
}

//获取句柄
HBITMAP CImageHandle::GetBitmapHandle()
{
	if (IsResourceValid()==false) return NULL;
	return (HBITMAP)(*m_pImage);
}

/////////////////////////////////////////////////////////////////////////////////////////
