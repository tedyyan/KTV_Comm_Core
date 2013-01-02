// XCardTest.h : main header file for the XCARDTEST application
//

#if !defined(AFX_XCARDTEST_H__EEB34479_64FA_42B2_B8F7_F151130A2540__INCLUDED_)
#define AFX_XCARDTEST_H__EEB34479_64FA_42B2_B8F7_F151130A2540__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXCardTestApp:
// See XCardTest.cpp for the implementation of this class
//

class CXCardTestApp : public CWinApp
{
public:
	CXCardTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXCardTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXCardTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCARDTEST_H__EEB34479_64FA_42B2_B8F7_F151130A2540__INCLUDED_)
