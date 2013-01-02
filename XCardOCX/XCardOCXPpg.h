#if !defined(AFX_XCARDOCXPPG_H__8A7BE164_7DEC_4C86_A592_EF59E9BD4209__INCLUDED_)
#define AFX_XCARDOCXPPG_H__8A7BE164_7DEC_4C86_A592_EF59E9BD4209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// XCardOCXPpg.h : Declaration of the CXCardOCXPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CXCardOCXPropPage : See XCardOCXPpg.cpp.cpp for implementation.

class CXCardOCXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CXCardOCXPropPage)
	DECLARE_OLECREATE_EX(CXCardOCXPropPage)

// Constructor
public:
	CXCardOCXPropPage();

// Dialog Data
	//{{AFX_DATA(CXCardOCXPropPage)
	enum { IDD = IDD_PROPPAGE_XCARDOCX };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CXCardOCXPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCARDOCXPPG_H__8A7BE164_7DEC_4C86_A592_EF59E9BD4209__INCLUDED)
