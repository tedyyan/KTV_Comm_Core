// MainProcess.h: interface for the CMainProcess class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_MAINPROCESS_H__FD952935_3189_4ED5_8665_6250704F9CA9__INCLUDED_)
#define AFX_MAINPROCESS_H__FD952935_3189_4ED5_8665_6250704F9CA9__INCLUDED_

#include "OSDDll.h"	// Added by ClassView
#include "ClipDll.h"	// Added by ClassView
#include "SongDAO.h"
#include "Comm.h"	// Added by ClassView
#include "PlayWav.h"	// Added by ClassView
#include "ini.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMainProcess  
{
public:
	bool updateOSDFlag;
	bool waitforThreadStop(CWinThread* mythread,int maxTime);

	int setSongP(CString pp);
	void hideShowIDOSD();
	void airDown();
	void airUp();
	void micUp();
	void micDown();
	void cancelCurrentOSDThread();
	void cancelOSDThreadnoButton();
	CIni myIni;
	void showSongN(CString);

	void startZhangSheng(HWND h);
	CComm comm;
	int status;
	
	void exitMainProcess();
	CWinThread* songinitthread;
	CWinThread* osdinitthread;
	CWinThread* osdplaythread;
	CWinThread* songplaythread;
	CWinThread* showidthread;
	CWinThread* showbuttonthread;
	CWinThread* timeControlThread;
	OSDDll singleOSDctl;
	CSongDAO dao;
	//ClipDll* songctl;
	
	bool initxCard();
	void initKTV();
	bool songNext();
	CMainProcess();
	virtual ~CMainProcess();
	VARIANT volumeUp();
	VARIANT volumeDown();
	VARIANT singerOn();
	VARIANT singerOff();
	VARIANT serviceOn();
	VARIANT serviceOff();
	VARIANT songPause();
	VARIANT songStop();
	VARIANT songStart();
	VARIANT songAhead();
	//VARIANT showSongID(VARIANT idStr);

	void playCurrentSongComm();
	void playNextSongComm();

	boolean duringStartingPlay;
	bool globalSongOverFlag ;
	boolean reSong;
protected:
	void cleanThreads();
private:

	bool globalFlag;
	void buttonOSD(CString otext,bool overFlag=true);
	int PlayFirstSongInDB();
	
	static DWORD previousTime;	
};




#endif // !defined(AFX_MAINPROCESS_H__FD952935_3189_4ED5_8665_6250704F9CA9__INCLUDED_)
