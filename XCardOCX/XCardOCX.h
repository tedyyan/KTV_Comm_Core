#if !defined(AFX_XCARDOCX_H__7DA996EA_F651_415D_8AD5_DF1C7330F08D__INCLUDED_)
#define AFX_XCARDOCX_H__7DA996EA_F651_415D_8AD5_DF1C7330F08D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// XCardOCX.h : main header file for XCARDOCX.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CXCardOCXApp : See XCardOCX.cpp for implementation.

class CXCardOCXApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCARDOCX_H__7DA996EA_F651_415D_8AD5_DF1C7330F08D__INCLUDED)
