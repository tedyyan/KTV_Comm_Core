// MovieTesterDlg.h : header file
//

#if !defined(AFX_MOVIETESTERDLG_H__9629D8DB_369C_4EF6_96A3_BE2A09F4D5D5__INCLUDED_)
#define AFX_MOVIETESTERDLG_H__9629D8DB_369C_4EF6_96A3_BE2A09F4D5D5__INCLUDED_

#include "..\..\MOIVECOMMCONTROL\MoiveCommControl.h"	// Added by ClassView
#include "..\XCARDOCX\ClipDll.h"	// Added by ClassView
#include "..\XCARDOCX\Comm.h"	// Added by ClassView
#include "..\XCARDOCX\SongDAO.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMovieTesterDlg dialog

class CMovieTesterDlg : public CDialog
{
// Construction
public:
	bool waitforThreadStop(CWinThread *mythread, int maxTime);
	CWinThread* playthread;
	CSongDAO dao;
	CComm comm;
	ClipDll clipctl;

	CMovieTesterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMovieTesterDlg)
	enum { IDD = IDD_MOVIETESTER_DIALOG };
	CComboBox	m_pitch_combo;
	CComboBox	m_volume_comb;
	CString	m_filename;
	int		m_channel;
	int		m_volume;
	BOOL	m_encode;
	BOOL	m_mpeg4;
	BOOL	m_divx;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMovieTesterDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMovieTesterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPause();
	afx_msg void OnResume();
	afx_msg void OnStop();
	afx_msg void OnPlay();
	afx_msg void OnRelease();
	afx_msg void OnCommSend();
	afx_msg void OnClose();
	afx_msg void OnOpen();
	afx_msg void OnChangeChannel();
	afx_msg void OnUpdateDB();
	afx_msg void OnVolumeChange();
	afx_msg void OnChannel0();
	afx_msg void OnChannel1();
	afx_msg void OnChannel2();
	afx_msg void OnChannel3();
	afx_msg void OnSelchangeVolumeValue();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSelchangePitchValue();
	afx_msg void OnEditchangePitchValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVIETESTERDLG_H__9629D8DB_369C_4EF6_96A3_BE2A09F4D5D5__INCLUDED_)
