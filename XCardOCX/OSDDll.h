// OSDDll.h: interface for the OSDDll class.
//
//////////////////////////////////////////////////////////////////////
//#define MOCK OK;

#if !defined(AFX_OSDDLL_H__9A56B86F_EAD5_46BD_B0D6_4C29C934BA0C__INCLUDED_)
#define AFX_OSDDLL_H__9A56B86F_EAD5_46BD_B0D6_4C29C934BA0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define USE_RMBASE_CLSID
#define MOCK 0


class OSDDll  
{
public:
	int INNER_RIGHT;
	int INNER_LEFT;
	void showPicture(CString filename,int destx,int desty);
	int Flush();
	void hideText();
	void ExitOSD();
	int InitOSD(int top,int left);
	int Close();
	int showScreen(int destx, int desty,int width, int height, BYTE *pBits);
	int FormatAndDraw(CString text,	int iWidth,
		int iHeight, BYTE *pBits2, CString pcolor,CString bcolor,int psize,CString pfont,int wi=0,int hi=0, bool han=false);
	bool DrawMemory(CDC *pDC, int fsize,CString text,long r,long g,long b,CString textFont, bool han=false);
	OSDDll();
	virtual ~OSDDll();
	BOOL initialDevice(CString text,int left,int top,CString pcolor,CString bcolor,int psize,CString pfont,int wi=0,int hi=0, bool han=false);
	
protected:
	bool singleFlag;
	
};

#endif // !defined(AFX_OSDDLL_H__9A56B86F_EAD5_46BD_B0D6_4C29C934BA0C__INCLUDED_)
