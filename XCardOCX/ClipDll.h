// lipDll.h: interface for the ClipDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIPDLL_H__7B447223_F78F_4287_B205_5EDF0A6A6A9A__INCLUDED_)
#define AFX_LIPDLL_H__7B447223_F78F_4287_B205_5EDF0A6A6A9A__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ClipDll  
{
public:
	bool waitforThreadStop(CWinThread *mythread,int maxTime);
	int getCurrentChannel();
	int getVolumeState();
	//void setTVOutput(int mode);
	void setNormalVolume(int volume);
	void Stop();
	void setPitchValue(long aPitch);

	long volumeDown(int step);
	long volumeUp(int step);
	bool OpenChannel(int channelid);
	void GoOn();
	void Pause();
	void setCallbackObject(bool* flag);


	void MovieProcess();
	int InitCard();

	void ReleaseMovie();
	int PlayFile (CString szFile,boolean encode,int volumeSong ,int volumeSinger, int songChannel, int singerChannel,int movieType,int mode);
	ClipDll();
	virtual ~ClipDll();
		long songVolume;
	long stereVolume;
	int onlySongChannel;
	int singerChannel;
	void Shutdown();
private:

	bool pauseFlag;
	int volumeState;
	int playingFlag;

	
		
	int currentChannel;
};

#endif // !defined(AFX_LIPDLL_H__7B447223_F78F_4287_B205_5EDF0A6A6A9A__INCLUDED_)
