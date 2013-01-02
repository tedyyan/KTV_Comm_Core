// OSDTesterDlg.h : header file
//

#if !defined(AFX_OSDTESTERDLG_H__38627F6B_E93A_441B_A015_E25EB0207D11__INCLUDED_)
#define AFX_OSDTESTERDLG_H__38627F6B_E93A_441B_A015_E25EB0207D11__INCLUDED_

#include "..\XCARDOCX\OSDDll.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COSDTesterDlg dialog

class COSDTesterDlg : public CDialog
{
// Construction
public:
	OSDDll osd;
	COSDTesterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(COSDTesterDlg)
	enum { IDD = IDD_OSDTESTER_DIALOG };
	CString	m_x;
	CString	m_y;
	CString	m_text;
	CString	m_color;
	CString	m_fsize;
	CString	m_bcolor;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COSDTesterDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(COSDTesterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInit();
	afx_msg void OnShow();
	afx_msg void OnHide();
	afx_msg void OnExit();
	afx_msg void OnFontsize1();
	afx_msg void OnChangeFontsize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OSDTESTERDLG_H__38627F6B_E93A_441B_A015_E25EB0207D11__INCLUDED_)
