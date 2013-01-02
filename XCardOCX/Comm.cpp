// Comm.cpp: implementation of the CComm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Comm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
#define SONGPLAY 0xf801
#define SONGOVER 0xf802
#define SCORE 0xf803
#define SCORE100 0xf804
#define CALLSERVICE 0xf805
#define ZHANGSHENG 0xf806
#define AIRUP 0xf807
#define AIRDOWN 0xf808
#define MICUP 0xf809
#define MICDOWN 0xf810
#define VOLUMEUP 0xf811
#define VOLUMEDOWN 0xf812
*/

#define SONGPLAY "f8 01"
#define SONGOVER "f8 02"
#define SCORE "f8 03"
#define SCORE100 "f8 04"
#define CALLSERVICE "f8 05"
#define ZHANGSHENG "f8 06"
#define AIRUP "f8 07"
#define AIRDOWN "f8 08"
#define MICUP "f8 09"
#define MICDOWN "f8 10"
#define VOLUMEUP "f8 11"
#define VOLUMEDOWN "f8 12"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComm::CComm()
{
	
}

CComm::~CComm()
{

}

void CComm::songStart()
{
	WinExec(_T("C:\\c.bat"),SW_SHOWMINIMIZED);
	Write(SONGPLAY);
}

void CComm::songCancel()
{

}

void CComm::songNext()
{
	Write(SONGOVER);
}


void CComm::scoreStart(int score)
{
	if (score==100){
		Write(SCORE100);
	}else{
		Write(SCORE);
	}
	
}

void CComm::serviceOn()
{
	Write(CALLSERVICE);
}

void CComm::serviceOff()
{
	Write(CALLSERVICE);
}

void CComm::startZhangSheng()
{
	Write(ZHANGSHENG);
}

void CComm::airUp()
{
	Write(AIRUP);
}

void CComm::airDown()
{
	Write(AIRDOWN);
}

void CComm::micUp()
{
	Write(MICUP);
}

void CComm::micDown()
{
	Write(MICDOWN);
}

void CComm::volumeUp()
{
	WinExec(_T("C:\\a.bat"),SW_SHOWMINIMIZED);
	Write(VOLUMEUP);
}

void CComm::volumeDown()
{
	WinExec(_T("C:\\b.bat"),SW_SHOWMINIMIZED);
	Write(VOLUMEDOWN);
}

bool CComm::open()
{
	comstatus.OpenConnection();
	return true;
}

int CComm::Read()
{

	::ReadCommBlock(comstatus,"",1);
	return 1;
}

bool CComm::Write(LPSTR cc)
{
	//::WriteCommBlock(comstatus,cc,1);
	//BOOL bWriteStatus;

	//int dwBytesRead=8;
	
	//LPOVERLAPPED m_OverlappedRead;
	//bWriteStatus = WriteFile(m_hIDComDev, buffer,
	//	dwBytesRead, (unsigned long *)&dwBytesRead, m_OverlappedRead );
	//return bWriteStatus;
	return true;
}

void CComm::Close()
{
	comstatus.CloseConnection();
}