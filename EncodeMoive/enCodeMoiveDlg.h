// enCodeMoiveDlg.h : header file
//

#if !defined(AFX_ENCODEMOIVEDLG_H__C6B8F0C6_7850_4556_A7CB_BA5CD05C0BDA__INCLUDED_)
#define AFX_ENCODEMOIVEDLG_H__C6B8F0C6_7850_4556_A7CB_BA5CD05C0BDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEnCodeMoiveDlg dialog

class CEnCodeMoiveDlg : public CDialog
{
// Construction
public:
	void encode1(CString sourceFile, CString destFile);
	CEnCodeMoiveDlg(CWnd* pParent = NULL);	// standard constructor
	void BrowseFile(int CallNum, CString strFile,CString destDir);
// Dialog Data
	//{{AFX_DATA(CEnCodeMoiveDlg)
	enum { IDD = IDD_ENCODEMOIVE_DIALOG };
	CString	m_dfname;
	CString	m_sfname;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnCodeMoiveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEnCodeMoiveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSourceFile();
	afx_msg void OnDestinationFile();
	afx_msg void OnEncodeFile();
	afx_msg void OnDecode();
	afx_msg void OnEncodeDir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENCODEMOIVEDLG_H__C6B8F0C6_7850_4556_A7CB_BA5CD05C0BDA__INCLUDED_)
