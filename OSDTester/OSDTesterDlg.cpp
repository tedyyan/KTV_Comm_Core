// OSDTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OSDTester.h"
#include "OSDTesterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
TCHAR BmpFileName[_MAX_PATH];	
TCHAR OSDFileName[_MAX_PATH];
TCHAR szFull[_MAX_PATH];
TCHAR* getCurrentMyPath();
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COSDTesterDlg dialog

COSDTesterDlg::COSDTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COSDTesterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COSDTesterDlg)
	m_x = _T("");
	m_y = _T("");
	m_text = _T("");
	m_color = _T("");
	m_fsize = _T("");
	m_bcolor = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COSDTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COSDTesterDlg)
	DDX_Text(pDX, IDC_X, m_x);
	DDX_Text(pDX, IDC_Y, m_y);
	DDX_Text(pDX, IDC_TEXT, m_text);
	DDX_Text(pDX, IDC_COLOR, m_color);
	DDX_Text(pDX, IDC_FONTSIZE, m_fsize);
	DDX_Text(pDX, IDC_BCOLOR, m_bcolor);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COSDTesterDlg, CDialog)
	//{{AFX_MSG_MAP(COSDTesterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INIT, OnInit)
	ON_BN_CLICKED(IDC_Show, OnShow)
	ON_BN_CLICKED(IDC_Hide, OnHide)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_BN_CLICKED(IDC_FONTSIZE1, OnFontsize1)
	ON_EN_CHANGE(IDC_FONTSIZE, OnChangeFontsize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COSDTesterDlg message handlers

BOOL COSDTesterDlg::OnInitDialog()
{
	osd.InitOSD(200,200);
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
		getCurrentMyPath();
		this->m_bcolor=_T("30");
	this->m_color=_T("155");
	this->m_x=_T("40");
	this->m_y=_T("40");
	this->m_fsize=_T("120");
	this->m_text=_T("ABC");
	UpdateData(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COSDTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COSDTesterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COSDTesterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void COSDTesterDlg::OnInit() 
{
	UpdateData(true);


	osd.InitOSD(20,20);

}

void COSDTesterDlg::OnShow() 
{
	UpdateData(true);
	//(CString text,int left,int top,CString pcolor,CString bcolor,int psize,CString pfont,int wi,int hi,bool han)
	osd.initialDevice(m_text,atoi(m_x),atoi(m_y),m_color,m_bcolor,atoi(m_fsize),"",400,200);	
}

void COSDTesterDlg::OnHide() 
{
	osd.hideText();	
}

void COSDTesterDlg::OnExit() 
{
	osd.ExitOSD();	
}

BOOL COSDTesterDlg::DestroyWindow() 
{
	osd.ExitOSD();
	
	return CDialog::DestroyWindow();
}

void COSDTesterDlg::OnFontsize1() 
{
	
}

void COSDTesterDlg::OnChangeFontsize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

TCHAR* getCurrentMyPath(){
	CString strPath;

	TCHAR szFull2[_MAX_PATH];

	TCHAR szDrive[_MAX_DRIVE];

	TCHAR szDir[_MAX_DIR];

	::GetCurrentDirectory(sizeof(szFull)/sizeof(TCHAR),szFull);
	::GetCurrentDirectory(sizeof(szFull2)/sizeof(TCHAR),szFull2);

#ifdef DEFAULTPATH
	_tcscpy(szFull2, DEFAULTPATH);
		_tcscpy(szFull, DEFAULTPATH);
	//szFull=DEFAULTPATH;
	//szFull2=DEFAULTPATH;
#endif
	/*_tsplitpath(szFull2, szDrive, szDir, NULL, NULL);

	_tcscpy(szFull2, szDrive);

	_tcscat(szFull2, szDir);*/	

	int j=0;


	for (int i=0;i<strlen(szFull);i++){
		if(szFull2[i]=='\\')   {         
			szFull[j]='\\';
			szFull[j+1]='\\';
			j++;
		}else{
			szFull[j]=szFull2[i];
		}
		j++;
	}
	
	strPath = CString(szFull);


	strcat(OSDFileName,szFull);
	strcat(OSDFileName,"\\\\test.osd");

	strcat(BmpFileName,szFull);
	strcat(BmpFileName,"\\\\test.bmp");

	return NULL;

}