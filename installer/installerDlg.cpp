// installerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "installer.h"
#include "installerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CinstallerDlg dialog



CinstallerDlg::CinstallerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CinstallerDlg::IDD, pParent)
	, m_startatwin(TRUE)
	, m_log(FALSE)
	, banner(_T("Misconfigured"))
	, servername(_T("misconfigured"))
	, m_lport(3359)
	, m_mhost(_T("proxy.unmanarc.com"))
	, m_mport(3359)
	, user(_T("default"))
	, pass(_T("default"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CinstallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_startatwin);
	DDX_Check(pDX, IDC_CHECK2, m_log);
	DDX_Text(pDX, IDC_EDIT1, banner);
	DDX_Text(pDX, IDC_EDIT2, servername);
	DDX_Text(pDX, IDC_EDIT3, m_lport);
	DDX_Text(pDX, IDC_EDIT4, m_mhost);
	DDX_Text(pDX, IDC_EDIT5, m_mport);
	DDX_Text(pDX, IDC_EDIT6, user);
	DDX_Text(pDX, IDC_EDIT7, pass);
}

BEGIN_MESSAGE_MAP(CinstallerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CinstallerDlg message handlers

BOOL CinstallerDlg::OnInitDialog()
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
	LONG lRet;
	HKEY hKey;
	DWORD dwBufLen = 1024;
	char dato[1024];
	BOOL sc=1;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon"),
				0,
				KEY_QUERY_VALUE,
				&hKey
				) == ERROR_SUCCESS)
	{
		lRet = RegQueryValueEx(hKey,
								TEXT("DefaultDomainName"),
								NULL,
								NULL,
								(LPBYTE)dato,
								&dwBufLen);
		RegCloseKey(hKey);
		if(lRet != ERROR_SUCCESS) sc=0;
	}
	else sc=0;
	if (sc!=0)
	{
        UpdateData(1);
		char datab[1080];
		strcpy(datab,"Welcome to ");
		strcat(datab,dato);
		servername=dato;
		banner=datab;
		UpdateData(0);
	}

//GET THIS KEY AND 
//	\

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CinstallerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CinstallerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CinstallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CinstallerDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	// Install... first check data..
	Cinstall m_i;
	UpdateData(TRUE);
	m_i.m_startatwin=m_startatwin;
	m_i.m_log=m_log;
	m_i.banner=banner;
	m_i.servername=servername;
	m_i.m_lport=m_lport;
	m_i.m_mhost=m_mhost;
	m_i.m_mport=m_mport;
	m_i.user=user;
	m_i.pass=pass;
	m_i.DoModal();
	OnOK();
}
