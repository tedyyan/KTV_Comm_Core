// CommCommander.h: interface for the CCommCommander class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMCOMMANDER_H__0A253F78_E83D_45F3_8506_3A8CC0D260FD__INCLUDED_)
#define AFX_COMMCOMMANDER_H__0A253F78_E83D_45F3_8506_3A8CC0D260FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SerialPort.h"

class CCommCommander  
{
public:
	
	CCommCommander();
	virtual ~CCommCommander();
	bool OpenComm(CString iniFilePathAndName);
	void One();
	void Two();
	void Three();
	void Four();
	void Five();

	void Six();
	void Seven();
	void Eight();
	void Nine();
	void Zero();
	void VolumneUp();
	void VolumneDown();
	void SingerOn();
	void SingerOff();
	void MicDown();
	void MicUp();
	void Service();
	CString ChangeCharstr2Hexstr(CString Charstr);
	char CCommCommander::CombineHexChar(char CharH,char CharL);
	int m_nSendBytes ;
	CString m_strSendBytes;
	bool m_bHexS ;
	void Shutdown();

	CString m_Zero;
	CString m_One;
	CString m_Two;
	CString m_Three;
	CString m_Four;
	CString m_Five;
	CString m_Six;
	CString m_Seven;
	CString m_Eight;
	CString m_Nine;
	CString m_VolumneUp;
	CString m_VolumneDown;
	CSerialPort m_SerialPort;
	CString m_SingerOn;
	CString m_SingerOff;
	CString m_Play;
	CString m_Cancel1;
	CString m_Cancel2;
	CString	m_Pause;
	CString m_Applause;
	CString m_TuneUp;
	CString m_TuneDown;
	CString m_ZhangSheng;
	CString m_Service;
	CString m_micUp;
	CString m_micDown;
	void Play();
	void Cancel1();
	void Clear();
	void Cancel2();

	void Pause();
	void Applause();
	void TuneUp();
	void TuneDown();

	CString m_SongBegin;
	CString m_SongIdel;
	CString m_SongCancel;
    CString m_Status;

	CString GetEvent(bool* stopping);

	DWORD m_dwThreadId;
	HANDLE m_threadhandle;	

	HANDLE m_ReceiveEvent;
	bool m_stoppingFlag;

	void HexStringFilter(CString &str);
	static unsigned char* m_innerBuffer;
	static int m_innerBufferLength;
	static void __stdcall OnReceiveChar(UINT ch, LONG port);
};

#endif // !defined(AFX_COMMCOMMANDER_H__0A253F78_E83D_45F3_8506_3A8CC0D260FD__INCLUDED_)
