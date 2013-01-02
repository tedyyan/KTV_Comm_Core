// Comm.h: interface for the CComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMM_H__0A5FF29C_6410_49B2_8306_0533EE778FAA__INCLUDED_)
#define AFX_COMM_H__0A5FF29C_6410_49B2_8306_0533EE778FAA__INCLUDED_

#include "COM232.H"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CComm  
{
public:
	void volumeDown();
	void volumeUp();
	void micDown();

	void micUp();
	void airDown();
	void airUp();
	void startZhangSheng();
	void serviceOff();
	void serviceOn();
	CComStatus comstatus;
	void Close();
	bool CComm::Write(LPSTR cc);
	int Read();

	bool open();
	void scoreStart(int score);

	void songNext();
	void songCancel();
	void songStart();
	CComm();
	virtual ~CComm();

};

#endif // !defined(AFX_COMM_H__0A5FF29C_6410_49B2_8306_0533EE778FAA__INCLUDED_)
