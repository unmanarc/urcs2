// install.cpp : implementation file
//

#include "stdafx.h"
#include "installer.h"
#include "install.h"
#include "MD5Checksum.h"
#include "io.h"

#define COMMAND_LINE 4092
char windir[_MAX_PATH];
char sysdir[_MAX_PATH];
char ini_filename[_MAX_PATH];
char returned[COMMAND_LINE];
BOOL installed;
BOOL nt;

// Cinstall dialog
void CopyFilesToWinDir()
{
	char newfile[_MAX_PATH];
	char newclient[_MAX_PATH];
	char newctool[_MAX_PATH];
	strcpy(newfile, windir);
	strcat(newfile,"\\rms.exe");
	strcpy(newclient, windir);
	strcat(newclient,"\\client.exe");
	strcpy(newctool, windir);
	strcat(newctool,"\\ctool2.exe");
	CopyFile("urcs.exe",newfile,FALSE);
	CopyFile("client.exe",newclient,FALSE);
	CopyFile("ctool2.exe",newctool,FALSE);
}
void LoadData()
{
	//Get global datas
	OSVERSIONINFO     verInfo = {0};
	verInfo.dwOSVersionInfoSize = sizeof (verInfo);
	GetVersionEx(&verInfo);
	switch (verInfo.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		nt=TRUE;
		break;

	case VER_PLATFORM_WIN32_WINDOWS:
		nt=FALSE;
		break;
	default:
		nt=FALSE;
		break;
    }

	GetWindowsDirectory(windir,512);
	GetSystemDirectory(sysdir,512);
	
	//Set init file at: "%windir%\urcs.ini"
	strcpy(ini_filename,windir);
	strcat(ini_filename,"\\urcs.ini");

	//Verify existence of ini
	if( (_access( ini_filename , 0 )) != -1 ) installed=TRUE;
	else installed=FALSE;
}

char *md5sum(char *value)
{
	memset(&returned,0,COMMAND_LINE);
	strncpy(returned,CMD5Checksum::GetMD5((BYTE*)value,(UINT)strlen(value)),32); //checksum Md5 is 32 bytes in hex.
	return returned;
}

BOOL PutKey( HKEY motherkey, LPCSTR directory, LPCSTR keyname, LPCSTR strtoput )
{
	//extracted idea from sample code.:
	//and re-writed to my own puorporses
	//http://developer.novell.com/ndk/doc/samplecode/gwc3po_sample/TIPOFDAY_CPP/REGISTER.CPP.html
   LPCSTR  strValue = strtoput;
   HKEY    handletokey;
   DWORD   error,dsp;
   error = RegCreateKeyEx(motherkey,directory,0,"",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&handletokey,&dsp);
   if ( error != ERROR_SUCCESS ) return FALSE;
   RegSetValueEx( handletokey,keyname,NULL,REG_SZ,(LPBYTE)strtoput,lstrlen( strtoput ));
   RegCloseKey( handletokey );
   return (error==ERROR_SUCCESS);
}
BOOL PutData(LPCTSTR appname,LPCTSTR keyname,LPCTSTR stringname)
{
	return WritePrivateProfileString(appname,keyname,stringname,ini_filename);
}

IMPLEMENT_DYNAMIC(Cinstall, CDialog)
Cinstall::Cinstall(CWnd* pParent /*=NULL*/)
	: CDialog(Cinstall::IDD, pParent)
{
}

Cinstall::~Cinstall()
{
}

void Cinstall::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(Cinstall, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// Cinstall message handlers

void Cinstall::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
}

void Cinstall::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	m_list.AddString("Installing.");
	m_list.AddString("Loading data...");
	LoadData();

	m_list.AddString("Adding variables to file...");

	char portoa[80];
	char portob[80];
	itoa(m_lport,portoa,10);
	itoa(m_mport,portob,10);

	PutData("URCS","server_banner",banner);
	PutData("URCS","server_prompt","[$d $u]");
	PutData("URCS","server_name",servername);
	PutData("URCS","server_port",portoa);
	PutData("URCS","mother_name",m_mhost);
	PutData("URCS","mother_port",portob);
	PutData("URCS","server_crypted","F");
	if (m_log) PutData("URCS","log_data","Y");
	else PutData("URCS","log_data","N");
	char tmppas[512];
	strcpy(tmppas,pass);
	PutData("PASSWORDS",user,md5sum(tmppas));
	PutData("UGROUP",user,"admin");
	//IF Start on init, copy itself to windows directory and
	//Make a modification on registry... /s
	//HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run
	if (m_startatwin)
	{
		CopyFilesToWinDir();
		if (!PutKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "URCS","rms"))
			m_list.AddString("Registry operation fail");
		WinExec("rms",0);
	}
	//m_list.AddString("Operation Finished.");
	MessageBox("Operation finished.");
	OnOK();

}
