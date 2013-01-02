// MoiveControl.h : main header file for the MOIVECONTROL DLL
//
//#define MOCK OK;

#if !defined(AFX_MOIVECONTROL_H__90A59699_AA9C_4BC8_BB04_A43FBFA11488__INCLUDED_)
#define AFX_MOIVECONTROL_H__90A59699_AA9C_4BC8_BB04_A43FBFA11488__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <streams.h>
#include "CommCommander.h"

#define SONG_BEGIN "SongBegin"
#define SONG_CANCEL "SongCancel"
#define SONG_IDEL "SongIdel"
#define Status_Cancel 0
#define Status_Playing 1
#define Status_Ideal 2

/////////////////////////////////////////////////////////////////////////////
// CMoiveControlApp
// See MoiveControl.cpp for the implementation of this class
//  

class __declspec(dllexport) CMoiveControlApp : public CWinApp
{
public:
	static IPin* FetchpPin(IBaseFilter *pFilter, PIN_DIRECTION PinDirection, CString mtype);

	static HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
	static void RemoveGraphFromRot(DWORD pdwRegister);


	static int setP(long pp);
	static IPin* GetPin(IBaseFilter *filter, int iPinNum);
	
	static IPin* FindPinByDirection(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
	static void Stop();
	static void setVolume(long value);
	static bool OpenChannel(int channelid);
	static bool getCurrentPlaying();
	static void GoOn();
	static void Pause();
	static int setCallbackObject(bool* flag);
	static void setPitch(long pitchValue);

	static void MovieProcess();
	static void InitCard();
	static void Applause();
	static void Shutdown();
	static void ReleaseMovie();
	static void ReleaseMovie_2();
	static int PlayFile (CString szFile,bool encode,int movieType,int mode);
	CMoiveControlApp();
	static int getCommStatus();
	static void micUp();
	static void micDown();
	static void Service();
	static void setTVOutput(int mode);
private:
	
	static HRESULT EnumFilters (IGraphBuilder *pGraph, IBaseFilter * o_pFilter);
	static IPin *GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
	static IPin *EmunPin(IBaseFilter *pFilter, CString filterflag, PIN_DIRECTION PinDir);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMoiveControlApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMoiveControlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	static AM_MEDIA_TYPE* EnumMediaType(IPin* thepin);
	static CCommCommander m_CommCommander;
	static long m_Volume;
	static long m_Pitch;
	
	static int commStatus;

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOIVECONTROL_H__90A59699_AA9C_4BC8_BB04_A43FBFA11488__INCLUDED_)
