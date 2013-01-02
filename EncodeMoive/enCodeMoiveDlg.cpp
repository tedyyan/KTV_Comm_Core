// enCodeMoiveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "enCodeMoive.h"
#include "enCodeMoiveDlg.h"
#define mmPart 1118192
#define mmPart_q 279548
#define mmCount 4

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CEnCodeMoiveDlg dialog

CEnCodeMoiveDlg::CEnCodeMoiveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEnCodeMoiveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnCodeMoiveDlg)
	m_dfname = _T("");
	m_sfname = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEnCodeMoiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnCodeMoiveDlg)
	DDX_Text(pDX, IDC_DFilename, m_dfname);
	DDX_Text(pDX, IDC_SFilename, m_sfname);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEnCodeMoiveDlg, CDialog)
	//{{AFX_MSG_MAP(CEnCodeMoiveDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SourceFile, OnSourceFile)
	ON_BN_CLICKED(IDC_DestinationFile, OnDestinationFile)
	ON_BN_CLICKED(IDC_EncodeFile, OnEncodeFile)
	ON_BN_CLICKED(IDC_Decode, OnDecode)
	ON_BN_CLICKED(IDC_encodeDir, OnEncodeDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnCodeMoiveDlg message handlers

BOOL CEnCodeMoiveDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEnCodeMoiveDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEnCodeMoiveDlg::OnPaint() 
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
HCURSOR CEnCodeMoiveDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CEnCodeMoiveDlg::OnSourceFile() 
{
	CFileDialog dlg(TRUE, "Source file", "*.*",
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"All Files(*.*)|*.*||"); //Text Files(*.txt)|*.txt|SQL Files(*.sql)|*.sql|
	if ( dlg.DoModal()!=IDOK ) return;
		//获取文件的绝对路径
	CString sFileName=dlg.GetPathName();
		//打开文件CStdioFile out;
	//out.Open(sFileName, CFile::modeRead);
	m_sfname = sFileName;
	UpdateData(false);

	
}

void CEnCodeMoiveDlg::OnDestinationFile() 
{
	CFileDialog dlg(false, "Destination File", "*.*",
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"All Files(*.*)|*.*||"); //Text Files(*.txt)|*.txt|SQL Files(*.sql)|*.sql|
	if ( dlg.DoModal()!=IDOK ) return;
		//获取文件的绝对路径
	CString sFileName=dlg.GetPathName();

		//打开文件CStdioFile out;
	//out.Open(sFileName, CFile::modeRead);
	m_dfname = sFileName;
	UpdateData(false);
}

void CEnCodeMoiveDlg::OnEncodeFile() 
{
	/*  Read the file */
	UpdateData(true);
    HANDLE hFile = CreateFile(m_sfname,
                              GENERIC_READ,
                              FILE_SHARE_READ,
                              NULL,
                              OPEN_EXISTING,
                              0,
                              NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        TRACE("\n error ");
        return ;
    }

    ULARGE_INTEGER uliSize;
    uliSize.LowPart = GetFileSize(hFile, &uliSize.HighPart);

	FILE	*f,*f2;
	BYTE	buff_8[mmPart_q];
	BYTE	buff_copy[mmPart_q];
	f2=fopen(m_dfname,"wb");
	f = fopen(m_sfname,"rb");
	
	if (NULL == f) return;	
	if (NULL == f2) return;



	//fpos_t pos = 0;
	//if(fsetpos(f,&pos)) return;
	

	TRACE("\n the file sum size:%d bytes\n",uliSize.LowPart);

	for (int j=0;j<mmCount;j++){
		int count = fread(buff_8,sizeof(BYTE),mmPart_q,f);
		if (count != mmPart_q) return;
		for (int i=0;i<mmPart_q;i++){
			unsigned char tt=buff_8[i];
			tt++;
			buff_copy[i]=tt;
		}
		int a=fwrite(buff_copy,sizeof(BYTE),mmPart_q,f2);
		TRACE("\n write the file size:%d bytes\n",a);
	}
	
	
	
	BYTE buff_1[1024];
	for (int i=mmPart;i<uliSize.LowPart;i=i+1024){
		 fread(buff_1,sizeof(BYTE),1024,f);
		 fwrite(buff_1,sizeof(BYTE),1024,f2);
	}
	TRACE("\n write the file size:%d bytes\n",uliSize.LowPart-mmPart);
	fclose(f);
	fclose(f2);

}



void CEnCodeMoiveDlg::OnDecode() 
{
	FILE	*f,*f2;
	BYTE	buff_8[mmPart_q];
	BYTE	buff_copy[mmPart_q];
	UpdateData(true);
	f2=fopen(m_dfname,"wb");
	f = fopen(m_sfname,"rb");
	
	if (NULL == f) return;	
	if (NULL == f2) return;

	/*  Read the file */
    HANDLE hFile = CreateFile(m_sfname,
                              GENERIC_READ,
                              FILE_SHARE_READ,
                              NULL,
                              OPEN_EXISTING,
                              0,
                              NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        TRACE("\n error ");
        return ;
    }

    ULARGE_INTEGER uliSize;
    uliSize.LowPart = GetFileSize(hFile, &uliSize.HighPart);


	TRACE("\n the file sum size:%d bytes\n",uliSize.LowPart);

	for (int j=0;j<mmCount;j++){
		int count = fread(buff_8,sizeof(BYTE),mmPart_q,f);
		if (count != mmPart_q) return;
		for (int i=0;i<mmPart_q;i++){
			unsigned char tt=buff_8[i];
			tt--;
			buff_copy[i]=tt;
		}
		int a=fwrite(buff_copy,sizeof(BYTE),mmPart_q,f2);
		TRACE("\n write the file size:%d bytes\n",a);
	}
	
	
	
	BYTE buff_1[1024];
	for (int i=mmPart;i<uliSize.LowPart;i=i+1024){
		 fread(buff_1,sizeof(BYTE),1024,f);
		 fwrite(buff_1,sizeof(BYTE),1024,f2);
	}
	TRACE("\n write the file size:%d bytes\n",uliSize.LowPart-mmPart);
	fclose(f);
	fclose(f2);
	
}

void CEnCodeMoiveDlg::BrowseFile(int CallNum, CString sourceDir,CString destDir)
{
	CallNum++;
	CFileFind ff;
	CString szDir = sourceDir;
	CString dzDir = destDir;
	if(szDir.Right(1) != "\\")
		szDir += "\\";
	if(dzDir.Right(1) != "\\")
		dzDir += "\\";
	
	szDir += "*.*";
	
	BOOL res = ff.FindFile(szDir);
	while(res)
	{
		res = ff.FindNextFile();
		if(ff.IsDirectory() && !ff.IsDots())//目录是文件夹
		{
			//如果是一个子目录，用递归继续往深一层找

			CString strPath = ff.GetFilePath(); //得到路径，做为递归调用的开始
			CString strTitle = ff.GetFileTitle();//得到目录名，做为树控的结点
			
			BrowseFile(CallNum,strPath,destDir);//递归调用
		}
		else if(!ff.IsDirectory() && !ff.IsDots())//到达最低层的文件
		{
			//显示当前访问的文件
			CString strPath;
			CString strTitle;
			strPath = ff.GetFilePath();
			strTitle = ff.GetFileTitle();
			ff.GetFileName();
			encode1(ff.GetFilePath(),dzDir+strTitle);
		}
	}
	ff.Close();//关闭
}

void CEnCodeMoiveDlg::encode1(CString sourceFile, CString destFile)
{
/*  Read the file */
	UpdateData(true);
    HANDLE hFile = CreateFile(sourceFile,
                              GENERIC_READ,
                              FILE_SHARE_READ,
                              NULL,
                              OPEN_EXISTING,
                              0,
                              NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        TRACE("\n error ");
        return ;
    }

    ULARGE_INTEGER uliSize;
    uliSize.LowPart = GetFileSize(hFile, &uliSize.HighPart);

	FILE	*f,*f2;
	BYTE	buff_8[mmPart_q];
	BYTE	buff_copy[mmPart_q];
	f2=fopen(destFile,"wb");
	f = fopen(sourceFile,"rb");
	
	if (NULL == f) return;	
	if (NULL == f2) return;



	//fpos_t pos = 0;
	//if(fsetpos(f,&pos)) return;
	

	TRACE("\n the file sum size:%d bytes\n",uliSize.LowPart);

	for (int j=0;j<mmCount;j++){
		int count = fread(buff_8,sizeof(BYTE),mmPart_q,f);
		if (count != mmPart_q) return;
		for (int i=0;i<mmPart_q;i++){
			unsigned char tt=buff_8[i];
			tt++;
			buff_copy[i]=tt;
		}
		int a=fwrite(buff_copy,sizeof(BYTE),mmPart_q,f2);
		TRACE("\n write the file size:%d bytes\n",a);
	}
	
	
	
	BYTE buff_1[1024];
	for (int i=mmPart;i<uliSize.LowPart;i=i+1024){
		 fread(buff_1,sizeof(BYTE),1024,f);
		 fwrite(buff_1,sizeof(BYTE),1024,f2);
	}
	TRACE("\n write the file size:%d bytes\n",uliSize.LowPart-mmPart);
	fclose(f);
	fclose(f2);
}

void CEnCodeMoiveDlg::OnEncodeDir() 
{
	UpdateData(true);
    CFile f,f2;
	f.Open(m_sfname,1);
	f2.Open(m_dfname,1);
	
	CString srcDir=f.GetFilePath().Left(f.GetFilePath().GetLength()-f.GetFileName().GetLength()-1);//f.GetFileTitle();
	AfxMessageBox(srcDir);
	CString destDir=f2.GetFilePath().Left(f2.GetFilePath().GetLength()-f2.GetFileName().GetLength()-1);//f.GetFileTitle();
	AfxMessageBox(destDir);
	f.Close();
	f2.Close();
	int i=10000;
	BrowseFile(i,srcDir,destDir);
}
