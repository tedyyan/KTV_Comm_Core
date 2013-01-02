// enCodeMoive.h : main header file for the ENCODEMOIVE application
//

#if !defined(AFX_ENCODEMOIVE_H__CE583CD3_98ED_4DD8_8CC6_23D42E8F9A75__INCLUDED_)
#define AFX_ENCODEMOIVE_H__CE583CD3_98ED_4DD8_8CC6_23D42E8F9A75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEnCodeMoiveApp:
// See enCodeMoive.cpp for the implementation of this class
//

class CEnCodeMoiveApp : public CWinApp
{
public:
	CEnCodeMoiveApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnCodeMoiveApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEnCodeMoiveApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENCODEMOIVE_H__CE583CD3_98ED_4DD8_8CC6_23D42E8F9A75__INCLUDED_)
