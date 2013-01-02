// MovieTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MovieTester.h"
#include "MovieTesterDlg.h"

TCHAR dbPath[17];

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
bool flag=false;
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


struct PlayParam{
	ClipDll *clipctl;
	CString songFile;
	bool encode;
	int mpeg4type;
} playp;

/////////////////////////////////////////////////////////////////////////////
// CMovieTesterDlg dialog

CMovieTesterDlg::CMovieTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMovieTesterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMovieTesterDlg)
	m_filename = _T("");
	m_channel = 0;
	m_volume = 0;
	m_encode = FALSE;
	m_mpeg4 = FALSE;
	m_divx = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CMovieTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMovieTesterDlg)
	DDX_Control(pDX, IDC_PITCH_VALUE, m_pitch_combo);
	DDX_Control(pDX, IDC_VOLUME_VALUE, m_volume_comb);
	DDX_Text(pDX, IDC_EDIT1, m_filename);
	DDX_Text(pDX, IDC_channel, m_channel);
	DDX_Text(pDX, IDC_VOLUME, m_volume);
	DDX_Check(pDX, IDC_ENCODEFLAG, m_encode);
	DDX_Check(pDX, IDC_CHECK1, m_mpeg4);
	DDX_Check(pDX, IDC_divx, m_divx);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMovieTesterDlg, CDialog)
	//{{AFX_MSG_MAP(CMovieTesterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Pause, OnPause)
	ON_BN_CLICKED(IDC_Resume, OnResume)
	ON_BN_CLICKED(IDC_Stop, OnStop)
	ON_BN_CLICKED(IDC_Play, OnPlay)
	ON_BN_CLICKED(IDC_Release, OnRelease)
	ON_BN_CLICKED(IDC_CommSend, OnCommSend)
	ON_BN_CLICKED(IDC_Close, OnClose)
	ON_BN_CLICKED(IDC_Open, OnOpen)
	ON_BN_CLICKED(IDC_BUTTON1, OnChangeChannel)
	ON_BN_CLICKED(IDC_BUTTON2, OnUpdateDB)
	ON_BN_CLICKED(IDC_BUTTON3, OnVolumeChange)
	ON_BN_CLICKED(IDC_Channel0, OnChannel0)
	ON_BN_CLICKED(IDC_Channel1, OnChannel1)
	ON_BN_CLICKED(IDC_Channel2, OnChannel2)
	ON_BN_CLICKED(IDC_Channel3, OnChannel3)
	ON_CBN_SELCHANGE(IDC_VOLUME_VALUE, OnSelchangeVolumeValue)
	ON_CBN_SELCHANGE(IDC_PITCH_VALUE, OnSelchangePitchValue)
	ON_CBN_EDITCHANGE(IDC_PITCH_VALUE, OnEditchangePitchValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMovieTesterDlg message handlers

BOOL CMovieTesterDlg::OnInitDialog()
{
		
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
	
	// TODO: Add extra initialization here
	//clipctl.InitCard();
	CMoiveControlApp::InitCard();
	m_filename="71638";
	m_volume_comb.InsertString(0,"0");
	m_volume_comb.InsertString(1,"-50");
	m_volume_comb.InsertString(2,"-100");
	m_volume_comb.InsertString(3,"-150");
	m_volume_comb.InsertString(4,"-200");
	m_volume_comb.InsertString(5,"-250");
	m_volume_comb.InsertString(6,"-300");
	m_volume_comb.InsertString(7,"-350");
	m_volume_comb.InsertString(8,"-400");
	m_volume_comb.InsertString(9,"-450");
	m_volume_comb.InsertString(10,"-500");
	m_volume_comb.InsertString(11,"-550");
	m_volume_comb.InsertString(12,"-600");
	m_volume_comb.InsertString(13,"-650");
	m_volume_comb.InsertString(13,"-700");
	m_volume_comb.InsertString(15,"-750");
	m_volume_comb.InsertString(16,"-800");
	m_volume_comb.InsertString(17,"-850");
	m_volume_comb.InsertString(18,"-900");
	m_volume_comb.InsertString(19,"-950");
	m_volume_comb.InsertString(20,"-1000");
	m_volume_comb.UpdateData(FALSE);
	
	m_pitch_combo.InsertString(0,"850");
	m_pitch_combo.InsertString(1,"880");
	m_pitch_combo.InsertString(2,"910");
	m_pitch_combo.InsertString(3,"940");
	m_pitch_combo.InsertString(4,"970");
	m_pitch_combo.InsertString(5,"1000");
	m_pitch_combo.InsertString(6,"1030");
	m_pitch_combo.InsertString(7,"1060");
	m_pitch_combo.InsertString(8,"1090");
	m_pitch_combo.InsertString(9,"1120");	
	m_pitch_combo.UpdateData(FALSE);

	UpdateData(false);

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMovieTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMovieTesterDlg::OnPaint() 
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
HCURSOR CMovieTesterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

char test[100];
void CMovieTesterDlg::OnPause() 
{
	clipctl.Pause();
	//strcat(test,"aaa");
	//AfxMessageBox(test);
	
}

void CMovieTesterDlg::OnResume() 
{

strcat(test,"No implement");
	AfxMessageBox(test);
}

void CMovieTesterDlg::OnStop() 
{
	clipctl.Stop();
	
}

UINT PlayThread(LPVOID pParam){
	
	TRACE("-------------PlayThread \n");
	PlayParam* pp=(PlayParam*)pParam;
	pp->clipctl->InitCard();
	boolean b=pp->encode;
	int mpeg4type=pp->mpeg4type;
	int tt=pp->clipctl->PlayFile(pp->songFile,b,-500,-500,1,2,mpeg4type,1);
	if (tt==-1){
		flag=true;
	}else if(tt==-2){
		TRACE("\nHardware error\n");
		return -1;
	}else {
		TRACE("\n play ok");
	}
	pp->clipctl->setCallbackObject(&flag);

	while(!flag){
		if (flag) break;
		//TRACE("\n in loop");
		Sleep(500);	
	}

	pp->clipctl->ReleaseMovie();

	TRACE("\n-------------Movie end ");
	return 1;
}


void CMovieTesterDlg::OnPlay() 
{

	UpdateData(true);
	playp.encode=m_encode;
	playp.clipctl=&clipctl;
	if (m_mpeg4){
		playp.mpeg4type=2;
	}else if (m_divx){
		playp.mpeg4type=3;
	}else{
		playp.mpeg4type=0;
	}
	
	playp.songFile=m_filename;
	flag=false;
	this->playthread=AfxBeginThread(PlayThread,&playp);

}


BOOL CMovieTesterDlg::DestroyWindow() 
{

	clipctl.ReleaseMovie();
	clipctl.Shutdown();
	return CDialog::DestroyWindow();
}

void CMovieTesterDlg::OnRelease() 
{

	flag=true;
	if (this->playthread!=NULL){
		this->playthread->m_bAutoDelete=false;

		waitforThreadStop(this->playthread,13000);
		this->playthread->m_bAutoDelete=true;
		delete this->playthread;
		this->playthread=NULL;
		
		AfxMessageBox("end");
	}
	
	
}

void CMovieTesterDlg::OnCommSend() 
{
	comm.songNext();
}

void CMovieTesterDlg::OnClose() 
{
	comm.Close();
}

void CMovieTesterDlg::OnOpen() 
{
	comm.open();
}

void CMovieTesterDlg::OnChangeChannel() 
{
	UpdateData(true);
	clipctl.OpenChannel(m_channel);
}

void CMovieTesterDlg::OnUpdateDB() 
{
	
	
}

void CMovieTesterDlg::OnVolumeChange() 
{
	UpdateData(true);

	clipctl.setNormalVolume(m_volume);
}

void CMovieTesterDlg::OnChannel0() 
{
	clipctl.OpenChannel(0);
}

void CMovieTesterDlg::OnChannel1() 
{
	clipctl.OpenChannel(1);
}

void CMovieTesterDlg::OnChannel2() 
{
	clipctl.OpenChannel(2);
}

void CMovieTesterDlg::OnChannel3() 
{
	clipctl.OpenChannel(3);
}

void CMovieTesterDlg::OnSelchangeVolumeValue() 
{
	UpdateData(true);
	clipctl.setNormalVolume(m_volume_comb.GetCurSel()*(-50));
}

void CMovieTesterDlg::OnDropFiles(HDROP hDropInfo) {
	char *lpszFileName=new char[512];

	int nFileCount;
	nFileCount=::DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,512);
	//::AfxMessageBox(itoa(nFileCount,cFileCount,10));
	if (nFileCount==1){
		UINT nChars=::DragQueryFile(hDropInfo,0,&lpszFileName[0],512);
		CString str(&lpszFileName[0],nChars); 
		m_filename=str;
		//::AfxMessageBox(str);
		
		UpdateData(false);
	}
	delete []lpszFileName;

}

bool CMovieTesterDlg::waitforThreadStop(CWinThread *mythread, int maxTime)
{
	DWORD dwStatus;
	int time=0;
	
	
	for (;;){
		::GetExitCodeThread(mythread->m_hThread, &dwStatus);
		if (dwStatus == STILL_ACTIVE){
			Sleep(100);
			time=time+100;
			if (time>maxTime){
				return false;
			}
		}else{
			break;
		}
	}
	return true;
}

void CMovieTesterDlg::OnSelchangePitchValue() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	clipctl.setPitchValue(1000+(m_pitch_combo.GetCurSel()-5)*30);
}

void CMovieTesterDlg::OnEditchangePitchValue() 
{
	// TODO: Add your control notification handler code here
	
}
