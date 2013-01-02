// lipDll.cpp: implementation of the ClipDll class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ClipDll.h"

#include <windows.h>
#include <mmsystem.h>
#include "..\MoiveCommControl\MoiveCommControl.h"
//ABC
//#include <streams.h>
//
//#include "playfile.h"

#include "resource.h"

#include <initguid.h>
//#include "sdinterf.h"

//#define MPEG4_SUPPORT

#define APPLICATIONNAME "PlayFile"
#define CLASSNAME "PlayFile"

#define WM_GRAPHNOTIFY	WM_USER+13

#define HELPER_RELEASE(x) { if (x) x->Release(); x = NULL; }

//HWND      ghApp;
//HINSTANCE ghInst;
//HMENU     ghMenu;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define DLLWORDPASS 710719

#define MAX_VOLUMEUP_COUNT 3
#define MAX_VOLUMEDOWN_COUNT -4
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



ClipDll::ClipDll()
{
	TRACE("\nconstructor clip dll");
	this->playingFlag=0;
	pauseFlag=false;
	songVolume=-500;
	stereVolume=-500;
	volumeState=0;
	CMoiveControlApp::setP(DLLWORDPASS);
}

ClipDll::~ClipDll()
{
	//CMoiveControlApp::ReleaseMovie();
}

int ClipDll::PlayFile(CString szFile,boolean encode,int volumeSong ,int volumeSinger, int songChannel, int singerChannel,int movieType,int mode)
{
	CFileFind find;
	/*
	if(!find.FindFile(szFile))
	{
		TRACE("\n File not found %s",szFile);
		return -1;
	}
	*/

	pauseFlag=false;
	TRACE("\n======PlayFile=====%s\n",szFile);
	TRACE("\nsongv: %d, singv:%d, songchannnel:%d",volumeSong,volumeSinger,songChannel);
	this->songVolume=volumeSong;
	this->stereVolume=volumeSinger;
	this->singerChannel=singerChannel;
	this->onlySongChannel=songChannel;	
	ClipDll::ReleaseMovie();
	int re=CMoiveControlApp::PlayFile(szFile,encode,movieType,mode);
	if (re>=0){
		//this->OpenChannel(songChannel);
		//this->setNormalVolume(this->songVolume);
		volumeState=0;
		this->playingFlag=1;
	}
	return re;
}

void ClipDll::setPitchValue(long aPitch)
{
	CMoiveControlApp::setPitch(aPitch);
}

void ClipDll::Shutdown()
{
}

void ClipDll::ReleaseMovie()
{	

}

int ClipDll::InitCard()
{
	
	return 1;
}

void ClipDll::MovieProcess()
{
		
		//pimc->Stop();
		//pimc->Release();
}


void ClipDll::setCallbackObject(bool* flag)
{

	
	return;
}

void ClipDll::Pause()
{
	
	
		TRACE("----ClipDll::Pause-----\n");
		CMoiveControlApp::Pause();
	
}

void ClipDll::GoOn()
{	
	TRACE("----ClipDll::GoOn-----\n");
	CMoiveControlApp::GoOn();		
}


bool ClipDll::OpenChannel(int channelid)
{
	TRACE("\nchannel id is: %d",channelid);
	CMoiveControlApp::OpenChannel(channelid);
	this->currentChannel=channelid;
	return true;
}

long ClipDll::volumeUp(int step)
{
	
	if (volumeState<MAX_VOLUMEUP_COUNT){
		volumeState++;
		stereVolume=stereVolume+step;
		songVolume=songVolume+step;
		if (songVolume>0){
			songVolume=0;
			return songVolume;
		}
		if (songVolume<-1000){
			songVolume=-1000;
			return songVolume;
		}
		if (stereVolume>0){
			stereVolume=0;
			return stereVolume;
		}
		if (stereVolume<-1000){
			stereVolume=-1000;
			return stereVolume;
		}
		
		if (onlySongChannel==currentChannel){
			TRACE("\nsongVolume is %d\n",songVolume);
			CMoiveControlApp::setVolume(songVolume);
			return songVolume;
		}else{
			TRACE("\nsongVolume is %d\n",stereVolume);
			CMoiveControlApp::setVolume(stereVolume);
			return stereVolume;
		}
	}
	return 0;
}

long ClipDll::volumeDown(int step)
{
	
	if (volumeState>MAX_VOLUMEDOWN_COUNT){
		songVolume=songVolume-step;
		stereVolume=stereVolume-step;
		--volumeState;
		if (songVolume>0){
			songVolume=0;
			return songVolume;
		}
		if (songVolume<-1000){
			songVolume=-1000;
			return songVolume;
		}
		if (stereVolume>0){
			stereVolume=0;
			return stereVolume;
		}
		if (stereVolume<-1000){
			stereVolume=-1000;
			return stereVolume;
		}
		if (onlySongChannel==currentChannel){		
					
			TRACE("\nsongVolume is %d\n",songVolume);
			CMoiveControlApp::setVolume(songVolume);
			return songVolume;
		}else{			
					
			TRACE("\nsongVolume is %d\n",stereVolume);
			CMoiveControlApp::setVolume(stereVolume);
			return stereVolume;
		}
	}
	return 0;
}

void ClipDll::Stop()
{
	CMoiveControlApp::Stop();
}

void ClipDll::setNormalVolume(int volume)
{
	TRACE("\nsongVolume is %d\n",volume);	
	CMoiveControlApp::setVolume(volume);
}

/*void ClipDll::setTVOutput(int mode)
{
	TRACE("\n ---------setTVOutput");
	CMoiveControlApp::setTVOutput(mode);
}*/

int ClipDll::getVolumeState()
{
	return volumeState;
}

int ClipDll::getCurrentChannel()
{
	return this->currentChannel;
}

bool ClipDll::waitforThreadStop(CWinThread *mythread, int maxTime)
{
	DWORD dwStatus;
	int time=0;	
	for (;;){
		if (mythread==NULL) {
			TRACE("\nmythread is NULL");
			return true;
		}
		::GetExitCodeThread(mythread->m_hThread, &dwStatus);
		if (dwStatus == STILL_ACTIVE){
			Sleep(100);
			time=time+100;
			if (time>maxTime){
				TRACE("\n******ERROR timeout for event");
				return false;
			}
		}else{
			break;
		}
	}
	return true;
}
