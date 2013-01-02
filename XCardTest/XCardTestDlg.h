// XCardTestDlg.h : header file
//
//{{AFX_INCLUDES()
#include "xcardocx.h"
//}}AFX_INCLUDES

#if !defined(AFX_XCARDTESTDLG_H__FE16B3E9_B6DF_4667_9934_5591339003C0__INCLUDED_)
#define AFX_XCARDTESTDLG_H__FE16B3E9_B6DF_4667_9934_5591339003C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXCardTestDlg dialog

class CXCardTestDlg : public CDialog
{
// Construction
public:
	CXCardTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXCardTestDlg)
	enum { IDD = IDD_XCARDTEST_DIALOG };
	CXCardOCX	m_xcard;
	CString	m_songid;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXCardTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXCardTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBianhao();
	afx_msg void OnStart();
	afx_msg void OnDestroy();
	afx_msg void OnExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCARDTESTDLG_H__FE16B3E9_B6DF_4667_9934_5591339003C0__INCLUDED_)
