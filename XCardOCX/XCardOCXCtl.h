#if !defined(AFX_XCARDOCXCTL_H__DC6C64DD_128D_4DD5_88EC_0DDBD03733CC__INCLUDED_)
#define AFX_XCARDOCXCTL_H__DC6C64DD_128D_4DD5_88EC_0DDBD03733CC__INCLUDED_

#include "..\MoiveCommControl\MoiveCommControl.h"	// Added by ClassView
#include "MainProcess.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// XCardOCXCtl.h : Declaration of the CXCardOCXCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CXCardOCXCtrl : See XCardOCXCtl.cpp for implementation.

class CXCardOCXCtrl : public COleControl
{
	DECLARE_DYNCREATE(CXCardOCXCtrl)

// Constructor
public:
	CXCardOCXCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXCardOCXCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CXCardOCXCtrl();

	DECLARE_OLECREATE_EX(CXCardOCXCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CXCardOCXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CXCardOCXCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CXCardOCXCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CXCardOCXCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CXCardOCXCtrl)
	afx_msg VARIANT initAll();
	afx_msg VARIANT volumeUp();
	afx_msg VARIANT songAhead();
	afx_msg VARIANT songNext();
	afx_msg VARIANT songStop();
	afx_msg VARIANT songPause();
	afx_msg VARIANT songStart();
	afx_msg VARIANT volumeDown();
	afx_msg VARIANT serviceOn();
	afx_msg VARIANT serviceOff();
	afx_msg VARIANT singerOn();
	afx_msg VARIANT singerOff();
	afx_msg VARIANT songControl();
	afx_msg VARIANT exitCard();
	afx_msg VARIANT showSongNum();
	afx_msg VARIANT showSongN(BSTR FAR* num);
	afx_msg VARIANT startZhangSheng();
	afx_msg VARIANT upAir();
	afx_msg VARIANT airDown();
	afx_msg VARIANT airUp();
	afx_msg VARIANT micUp();
	afx_msg VARIANT micDown();
	afx_msg VARIANT setSongP(BSTR FAR* mypd);
	afx_msg VARIANT updateSLOSD(BSTR FAR* songlist);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CXCardOCXCtrl)
	void FireRefreshGUI()
		{FireEvent(eventidRefreshGUI,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	HWND abouthandler;
	CMainProcess main;
	enum {
	//{{AFX_DISP_ID(CXCardOCXCtrl)
	dispidInitAll = 1L,
	dispidVolumeUp = 2L,
	dispidSongAhead = 3L,
	dispidSongNext = 4L,
	dispidSongStop = 5L,
	dispidSongPause = 6L,
	dispidSongStart = 7L,
	dispidVolumeDown = 8L,
	dispidServiceOn = 9L,
	dispidServiceOff = 10L,
	dispidSingerOn = 11L,
	dispidSingerOff = 12L,
	dispidSongControl = 13L,
	dispidExitCard = 14L,
	dispidShowSongNum = 15L,
	dispidShowSongN = 16L,
	dispidStartZhangSheng = 17L,
	dispidUpAir = 18L,
	dispidAirDown = 19L,
	dispidAirUp = 20L,
	dispidMicUp = 21L,
	dispidMicDown = 22L,
	dispidSetSongP = 23L,
	dispidUpdateSLOSD = 24L,
	eventidRefreshGUI = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCARDOCXCTL_H__DC6C64DD_128D_4DD5_88EC_0DDBD03733CC__INCLUDED)
