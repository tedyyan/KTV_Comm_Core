// MainProcess.cpp: implementation of the CMainProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "MainProcess.h"
#include "SongDAO.h"
#include "..\MoiveCommControl\MoiveCommControl.h"
#define DEFAULTPATH "c:\\ktv"
#define INTERVAL_SYN 200
#define COUNT_SYN 5
#define MYPP "710719"

#define THREAD_TIMEOUT 3000
#define INTERVAL_IDEL 100
#define INIT_SONG_VOLUME_STATUS 5
#define INIT_AIR_VOLUME_STATUS 5
#include "ini.h"

#define DLLWORDPASS 710719
struct OSDTime{
	long waittime0;
	long showtime;
	long show2time;
	long waittime;
	
} ADosd,*osd1,PlayOSD;

struct osdinitthreadParam{
	OSDDll osdctl;
	OSDTime osdTime;
	CSongDAO dao;
	CString text;
	bool han;
	CMainProcess* mainProcess;
} initOsdThread_p,playOsdThread_p,ShowIDThread_p,ShowButtonThread_p;

struct SongThreadParam{

	CSongDAO dao;
	CString songFile;
	int volumeSong;
	int volumeSinger;
	int songChannel;
	int singerChannel;
	int mpeg4flag;
	boolean encodeflag;
	CComm comm;
	CMainProcess* mainProcess;

} initSongThread_p,playSongThread_p, timercontorl_p;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool initsong=true;
bool initosd=true;
bool playsong=true;
bool playosd=true;
bool showIDFlag=false;
bool showButtonFlag=false;

bool timerStopFlag=false;
bool timercontrol=true;
//whether the SongID is changed by user for show redraw the OSD
bool showIDConFlag=false;
bool buttonConFlag=false;
bool scoreflag=false;
//
int msstatus=INIT_SONG_VOLUME_STATUS;
int micstatus=6;
int airstatus=INIT_AIR_VOLUME_STATUS;

//whether the we have called initCard
//if not, we should call it
//bool globalflag=false;

//deprecated
//bool playosdshowing=true;

//whether the adv-osd is in stop status
//bool stoposdadv=false;

int restorethread=0;
//bool playosdpause=false;
TCHAR szFull[_MAX_PATH];
TCHAR* getCurrentMyPath();

TCHAR dbPath[_MAX_PATH];
TCHAR iniPath[_MAX_PATH];
TCHAR adPath1[_MAX_PATH];
TCHAR adPath2[_MAX_PATH];
TCHAR adPath3[_MAX_PATH];
TCHAR adPath4[_MAX_PATH];
TCHAR adPath5[_MAX_PATH];
TCHAR adPath6[_MAX_PATH];


TCHAR showtext[_MAX_PATH];

TCHAR BmpFileName[_MAX_PATH];	
TCHAR OSDFileName[_MAX_PATH];

CIni Ini(getCurrentMyPath());

void splitString(const CString csOriginalWord,const CString splitter,CString csResult[]){
	  int splitterLength=splitter.GetLength();
      int iBegin=0;
      CString csResultWord;
		int i=0;
      csResultWord.Format("%s",csOriginalWord);
	  
      while( (iBegin=csResultWord.Find(splitter,0))>0 ){

		csResult[i]=csResultWord.Left(iBegin);
        csResultWord.Delete(0,iBegin+splitterLength);
		i++;
      }
      csResult[i]=csResultWord;

}




UINT playCurrentSongCommThread(LPVOID pParam){
	((CMainProcess*)pParam)->playCurrentSongComm();
	return S_OK;
}
UINT playNextSongCommThread(LPVOID pParam){
	((CMainProcess*)pParam)->playNextSongComm();
	return S_OK;
}
UINT CommSongThread(LPVOID pParam){
	//0: cancel
	//1: playing
	//2: ideal
	SongThreadParam* para=(SongThreadParam*)pParam;
	CSongDAO dao=para->dao;
	CMainProcess* process=para->mainProcess;
	int curStatus = CMoiveControlApp::setCallbackObject(&(process->globalSongOverFlag));
	TRACE("***0current Status:%d",curStatus);
	while(!process->globalSongOverFlag)
	{
		if (Status_Ideal == curStatus)
		{
			TRACE("***Status:Ideal reSong:%d",process->reSong);
			if ((process->reSong) == false)
			{
				dao.deleteCurrentSong();
			}
			AfxBeginThread(playCurrentSongCommThread,process);
			Sleep(200);
		}
		if (Status_Playing == curStatus)
		{
			TRACE("***Status:Playing");
		}
		if (Status_Cancel == curStatus)
		{
			TRACE("***Status:Cancelled");
			if ((process->reSong) == false)
			{
				//dao.deleteCurrentSong();
			}
		}
		curStatus = CMoiveControlApp::setCallbackObject(&process->globalSongOverFlag);
		TRACE("***current Status:%d",curStatus);
	}
	TRACE("***exit CommSongThread");
	return S_OK;
}
void CMainProcess::playCurrentSongComm()
{
	int volumeSong,volumeSinger,songChannel,singerChannel;
	boolean encode=false;
	bool han=false;
	int mpeg4flag=0;
	int count;
		int i=0;
		
		CString songid=dao.getNextSongName(NULL,&encode,&volumeSong,&volumeSinger,&songChannel,&singerChannel,&han,&mpeg4flag);
		while ((songid.GetLength()<=2)&&(i<5)){		
			Sleep(100);
			bool dbflag=dao.closeDBConnection();		
			songid=dao.getNextSongName(NULL,&encode,&volumeSong,&volumeSinger,&songChannel,&singerChannel,&han,&mpeg4flag);
			i++;
		}
	if (songid.GetLength()>0)
	{
		TRACE("playCurrentSongComm:%s",songid);
		CMoiveControlApp::PlayFile(songid,encode,mpeg4flag,encode);
	}
	
}


void CMainProcess::playNextSongComm()
{
	int volumeSong,volumeSinger,songChannel,singerChannel;
	boolean encode=false;
	bool han=false;
	int mpeg4flag=0;
	CString nextSongid="";
	CString songid = dao.getNextSongName(&nextSongid,&encode,&volumeSong,&volumeSinger,&songChannel,&singerChannel,&han,&mpeg4flag);
	if (nextSongid.GetLength()==0)
	{
		TRACE("no next song playNextSongComm");
		return;
	}
	TRACE("playNextSongComm:%s",nextSongid);
	CMoiveControlApp::PlayFile(nextSongid,encode,mpeg4flag,encode);
}
CMainProcess::CMainProcess()
{
	// ---------------------------------------------------------------------------------------------------
	showidthread=NULL;
	//pass word control
	globalFlag=true;
	showIDFlag=false;
	updateOSDFlag=false;
	globalSongOverFlag = false;
	reSong = false;

	// 初始化COM,创建ADO连接等操作
	AfxOleInit();
	AfxEnableControlContainer();
	status=0;
}

CMainProcess::~CMainProcess()
{	
	

}
unsigned long CMainProcess::previousTime = 0;
bool CMainProcess::songNext()
{
	
	DWORD now = GetTickCount();
	if ((previousTime>0)&&(now - previousTime<=6000))
	{
		TRACE("??????????????????????? %lu %lu\n",previousTime,now);
		return true;
	}else
	{
		TRACE("??????------------???????? %lu %lu\n",previousTime,now);
	}
	previousTime = now;
	TRACE(TEXT("\n--------------PlayNextSong"));

	if (CMoiveControlApp::getCommStatus() == Status_Playing)
	{
		//this->cleanThreads();
		
		CMoiveControlApp::ReleaseMovie();

		CMoiveControlApp::ReleaseMovie();

		Sleep(500);
		dao.deleteCurrentSong();
		this->playCurrentSongComm();
		TRACE(TEXT("\n++++++++++++PlayNextSong 11111111111"));
		Sleep(1000);
		TRACE(TEXT("\n++++++++++++PlayNextSong 22222222222"));
		return true;
	}
	if (CMoiveControlApp::getCommStatus() == Status_Cancel)
	{
		int count;
		int i=0;
		dao.deleteCurrentSong();
		count=dao.getPlayCount();
		while ((count==0)&&(i<3)){
			TRACE("\nRetry to get play list");
			bool dbflag=dao.closeDBConnection();
			count=dao.getPlayCount();
			Sleep(200);
			i++;
		}
		
		CMoiveControlApp::ReleaseMovie_2();
		this->playCurrentSongComm();
		TRACE(TEXT("\n++++++++++++PlayNextSong 33333333333"));
		Sleep(1000);
		TRACE(TEXT("\n++++++++++++PlayNextSong 44444444444"));
		return true;
	}
}

void CMainProcess::initKTV()
{
	TRACE(TEXT("\n--------------initKTV"));
	if (!this->globalFlag){
		return;
	}
	this->cleanThreads();
	this->status=0;
	try{
		initosd=true;
		
		initsong=true;
		int mpeg4flag = 0;
		boolean encodeflag=false;
		int mode = 0;
		CString firstSong = Ini.ReadText("comm","firstSongNum");
		
		playSongThread_p.mainProcess = this;
		
		AfxBeginThread(CommSongThread,&playSongThread_p);
		Sleep(200);
		CMoiveControlApp::PlayFile(firstSong,encodeflag,mpeg4flag,mode);

	}catch (CException* e){			
		TRACE("\ninitKTV");
		LPTSTR err;
		UINT maxerr=10;
		e->GetErrorMessage(err,maxerr);
		TRACE(err);
		return;
	}
	TRACE(TEXT("\n++++++++++++initKTV"));
	
}
bool CMainProcess::initxCard()
{
	if (!this->globalFlag){
		TRACE("\n pass");
		return false;
	}
	TRACE(TEXT("\n------------initxCard"));
	songinitthread=NULL;
	osdinitthread=NULL;
	osdplaythread=NULL;
	songplaythread=NULL;
	showbuttonthread=NULL;
	showidthread=NULL;

CMoiveControlApp::setP(DLLWORDPASS);
		CMoiveControlApp::InitCard();

	//Order is key
	char* left = Ini.ReadText("index","osdleft");	
	char* top= Ini.ReadText("index","osdtop");	
	
	singleOSDctl.InitOSD(atoi(top),atoi(left));
	
	//dao.initDB();

	//Song thread value
	
	initSongThread_p.dao=this->dao;
	initSongThread_p.mainProcess=this;

	playSongThread_p.dao=this->dao;
	playSongThread_p.mainProcess=this;

	//dao.deleteAllTemp();

	//-----------------------------------------
	//OSD
	//-----------------------------------------

	//AD OSD thread value
	char* s_show = Ini.ReadText("index","tt");	
	CString abc[4];
	splitString(s_show,",",abc);
	ADosd.showtime=atol(abc[0]);
	ADosd.waittime=atol(abc[1]);
	initOsdThread_p.osdTime=ADosd;
	initOsdThread_p.osdctl=singleOSDctl;
	initOsdThread_p.mainProcess=this;

	//OSD thread value
	s_show = Ini.ReadText("index","o1tt");
	splitString(s_show,",",abc);
	PlayOSD.waittime0=atol(abc[0]);
	PlayOSD.showtime=atol(abc[1]);
	PlayOSD.show2time=atol(abc[2]);
	PlayOSD.waittime=atol(abc[3]);
	
	playOsdThread_p.osdTime=PlayOSD;
	playOsdThread_p.osdctl=singleOSDctl;
	playOsdThread_p.dao=this->dao;
	playOsdThread_p.mainProcess=this;

	//OSD show id thread
	CString o3tt=Ini.ReadText("index","o3tt");
	ShowIDThread_p.osdTime.showtime=atoi(o3tt);
	ShowIDThread_p.dao=this->dao;
	ShowIDThread_p.osdctl=this->singleOSDctl;
	ShowIDThread_p.mainProcess=this;
	//OSD Button
	CString o2tt=Ini.ReadText("index","o2tt");
	ShowButtonThread_p.osdTime.showtime=atoi(o2tt);
	ShowButtonThread_p.dao=this->dao;
	ShowButtonThread_p.osdctl=this->singleOSDctl;
	ShowButtonThread_p.mainProcess=this;
	//comm.open();
	TRACE(TEXT("\n+++++++++++++initxCard"));
	return true;
}

VARIANT CMainProcess::volumeUp() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	
	CMoiveControlApp::setVolume(1);
	
	TRACE("\n++++++++++volumeUp");
	return vaResult;
	
}

VARIANT CMainProcess::volumeDown() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	
	CMoiveControlApp::setVolume(-1);
	
	return vaResult;
	
}


VARIANT CMainProcess::songStop() 
{
	TRACE("\n----songStop-----");
	VARIANT vaResult;	
	VariantInit(&vaResult);
	
	
	vaResult.vt = VT_BOOL;
	vaResult.boolVal = VARIANT_TRUE;
		
	CMoiveControlApp::Stop();
	
	TRACE("\n++++++++songStop+++++++++");
	return vaResult;
}

VARIANT CMainProcess::songAhead() 
{
	VARIANT vaResult;	
	VariantInit(&vaResult);
	
	DWORD now = GetTickCount();
	if ((previousTime>0)&&(now - previousTime<=6000))
	{
		TRACE("??????????????????????? %lu %lu\n",previousTime,now);
		return vaResult;
	}else
	{
		TRACE("??????------------???????? %lu %lu\n",previousTime,now);
	}
	previousTime = now;
	
	
	TRACE("\n------------songAhead");
	
	
	if (CMoiveControlApp::getCommStatus() == Status_Playing)
	{
		reSong = true;
		TRACE("\n-Current status is playing");
		CMoiveControlApp::ReleaseMovie();

		CMoiveControlApp::ReleaseMovie();

		Sleep(200);
		
		this->playCurrentSongComm();
		reSong = false;
		return vaResult;
	}
	Sleep(1000);
		return vaResult;
}

VARIANT CMainProcess::songStart() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	
	vaResult.vt = VT_BOOL;
	vaResult.boolVal = VARIANT_TRUE;
	DWORD now = GetTickCount();
	if ((previousTime>0)&&(now - previousTime<=6000))
	{
		TRACE("??????????????????????? %lu %lu\n",previousTime,now);
		return vaResult;
	}else
	{
		TRACE("??????------------???????? %lu %lu\n",previousTime,now);
	}
	previousTime = now;
	//status s 0 ->init
	//status s 1 ->playing
	//status s 2 ->pause
	//status s 3 ->stop
	CString value="";
	boolean encode=false;
	int volumeSong=0;
	int volumeSinger=0;
	int songChannel=0;
	int singerChannel=0;
	bool han=false;

	TRACE("\n----songStart-----");
	
	if (CMoiveControlApp::getCommStatus() == Status_Playing)
	{
		return vaResult;
	}
	if (CMoiveControlApp::getCommStatus() == Status_Ideal)
	{
		
		int count;
		int i=0;
		count=dao.getPlayCount();
		while ((count==0)&&(i<3)){
			TRACE("\nRetry to get play list");
			bool dbflag=dao.closeDBConnection();
			count=dao.getPlayCount();
			Sleep(200);
			i++;
		}
		
		CMoiveControlApp::ReleaseMovie_2();
		playCurrentSongComm();
		Sleep(2000);
		return vaResult;
	}
	if (CMoiveControlApp::getCommStatus() == Status_Cancel)
	{
		
		int count;
		int i=0;
		dao.deleteCurrentSong();
		count=dao.getPlayCount();
		while ((count==0)&&(i<3)){
			TRACE("\nRetry to get play list");
			bool dbflag=dao.closeDBConnection();
			count=dao.getPlayCount();
			Sleep(200);
			i++;
		}
		
		CMoiveControlApp::ReleaseMovie_2();
		playCurrentSongComm();
		Sleep(2000);
		return vaResult;
	}
	
	return vaResult;
}

VARIANT CMainProcess::songPause() 
{
	CString value,tempNext;
			boolean encode;
	int volumeSong,volumeSinger,songChannel,singerChannel,mpeg4flag;
	bool hanflag;
	TRACE("\n----songPause-----");
	VARIANT vaResult;
	VariantInit(&vaResult);
	
	CMoiveControlApp::Pause();

	return vaResult;
}

VARIANT CMainProcess::singerOff() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);

	
	CMoiveControlApp::OpenChannel(0);
	
	return vaResult;
	
}


VARIANT CMainProcess::singerOn() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	
	CMoiveControlApp::OpenChannel(0);
	
	return vaResult;
	
}

VARIANT CMainProcess::serviceOn() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	CMoiveControlApp::Service();
	return vaResult;
}

VARIANT CMainProcess::serviceOff() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	CMoiveControlApp::Service();
	return vaResult;
}

void CMainProcess::exitMainProcess()
{

	TRACE("\n-----------exitMainProcess");	
	
	CMoiveControlApp::setTVOutput(1);
	initsong=false;
	initosd=false;
	playsong=false;
	playosd=false;
	globalSongOverFlag = true;
	Sleep(1000);

	CMoiveControlApp::Shutdown();
	TRACE("\n++++++++++return exitMainProcess ");
}

void CMainProcess::cleanThreads()
{
	Sleep(300);	
	TRACE("\n++++++++++come into clean Threads++++++++++++");
}


int CMainProcess::PlayFirstSongInDB()
{
	
	
	return true;
}

void CMainProcess::buttonOSD(CString otext,bool overFlag)
{

	
}

CString getNextSongForScore(CString newfile,CSongDAO dao,int *oscore)
{
	return "";
}

void CMainProcess::startZhangSheng(HWND h)
{	

	CMoiveControlApp::Applause();

}
bool idsflag=true;
void CMainProcess::showSongN(CString numSong)
{
	return;

}


TCHAR* getCurrentMyPath(){
	CString strPath;
	TCHAR szFull2[_MAX_PATH];
	::GetCurrentDirectory(sizeof(szFull)/sizeof(TCHAR),szFull);
	::GetCurrentDirectory(sizeof(szFull2)/sizeof(TCHAR),szFull2);

#ifdef DEFAULTPATH
	_tcscpy(szFull2, DEFAULTPATH);
	_tcscpy(szFull, DEFAULTPATH);

#endif
	int j=0;
	for (int i=0;i<strlen(szFull);i++){
		if(szFull2[i]=='\\')   {         
			szFull[j]='\\';
			szFull[j+1]='\\';
			j++;
		}else{
			szFull[j]=szFull2[i];
		}
		j++;
	}
	
	strPath = CString(szFull);

	strcat(dbPath,"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");
	strcat(dbPath,szFull);
	strcat(dbPath,"\\\\ktvdb.mdb");

	strcat(iniPath,szFull);
	strcat(iniPath,"\\\\test.ini");

	strcat(adPath1,szFull);
	strcat(adPath1,"\\\\guang1.bmp");

	strcat(adPath2,szFull);
	strcat(adPath2,"\\\\guang2.bmp");

	strcat(adPath3,szFull);
	strcat(adPath3,"\\\\guang3.bmp");

	strcat(adPath4,szFull);
	strcat(adPath4,"\\\\guang4.bmp");

	strcat(adPath5,szFull);
	strcat(adPath5,"\\\\guang5.bmp");

	strcat(adPath6,szFull);
	strcat(adPath6,"\\\\guang6.bmp");

	strcat(OSDFileName,szFull);
	strcat(OSDFileName,"\\\\test.osd");

	strcat(BmpFileName,szFull);
	strcat(BmpFileName,"\\\\test.bmp");

	return iniPath;

}

void CMainProcess::cancelCurrentOSDThread()
{
	
	Sleep(300);
	
}

void CMainProcess::cancelOSDThreadnoButton()
{
	
	Sleep(300);	
}

void CMainProcess::micDown()
{
	CMoiveControlApp::micDown();
}

void CMainProcess::micUp()
{
	CMoiveControlApp::micUp();

}

void CMainProcess::airUp()
{
	CMoiveControlApp::setPitch(1);
	
}

void CMainProcess::airDown()
{
	CMoiveControlApp::setPitch(-1);
	
}

void CMainProcess::hideShowIDOSD()
{
	
}

int CMainProcess::setSongP(CString pp)
{
	if (pp==MYPP){
		this->globalFlag=true;
		return 1;
	}else{
		this->globalFlag=false;
		return -1;
	}
}

bool CMainProcess::waitforThreadStop(CWinThread* mythread,int maxTime)
{
	DWORD dwStatus;
	int time=0;
	for (;;){
		::GetExitCodeThread(mythread->m_hThread, &dwStatus);
		if (dwStatus == STILL_ACTIVE){
			Sleep(100);
			time=time+100;
			if (time>maxTime){
				TRACE("\nERROR timeout for MainProcess");
				return false;
			}
		}else{
			break;
		}
	}
	return true;
}
