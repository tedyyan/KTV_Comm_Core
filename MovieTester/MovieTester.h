// MovieTester.h : main header file for the MOVIETESTER application
//

#if !defined(AFX_MOVIETESTER_H__761DDB5D_1A65_4C64_973C_A2C703A72E9D__INCLUDED_)
#define AFX_MOVIETESTER_H__761DDB5D_1A65_4C64_973C_A2C703A72E9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMovieTesterApp:
// See MovieTester.cpp for the implementation of this class
//

class CMovieTesterApp : public CWinApp
{
public:
	CMovieTesterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMovieTesterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMovieTesterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVIETESTER_H__761DDB5D_1A65_4C64_973C_A2C703A72E9D__INCLUDED_)
