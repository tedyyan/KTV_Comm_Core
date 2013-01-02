// OSDTester.h : main header file for the OSDTESTER application
//

#if !defined(AFX_OSDTESTER_H__F9BD8554_7755_4B74_8228_A1F6FF855373__INCLUDED_)
#define AFX_OSDTESTER_H__F9BD8554_7755_4B74_8228_A1F6FF855373__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COSDTesterApp:
// See OSDTester.cpp for the implementation of this class
//

class COSDTesterApp : public CWinApp
{
public:
	COSDTesterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COSDTesterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COSDTesterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OSDTESTER_H__F9BD8554_7755_4B74_8228_A1F6FF855373__INCLUDED_)
