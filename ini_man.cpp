/*
All CopyRights Reserved.
Created by unmanarc.com. emailme to: unma@unmanarc.com
this source are under GNU/GPL (General Public Licence)
http://urcs.unmanarc.com/
http://urcs.sourceforge.net/

by unmanarc.
thanks to all.
enjoy ;)
*/
#include "stdafx.h"

#include "MD5Checksum.h"
#include "ini_man.h"

#include "funcs.h"
#include <io.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


Cini::Cini()
{ //Constructor
	
}
Cini::~Cini()
{
}
void Cini::CopyMeToWinDir()
{
	char newfile[512];
	strcpy(newfile, windir);
	strcat(newfile,"\\rms.exe");
	CopyFile(lcname,newfile,FALSE);
}

s_proxyinf Cini::GetProxy()
{
	s_proxyinf localproxy;
	localproxy.have_proxy=1;
	s_proxyinf mal;
	mal.have_proxy=0;
	LONG lRet;
	HKEY hKey;
	DWORD dwBufLen = 1024;
	char dato[1024];
	if(RegOpenKeyEx(HKEY_CURRENT_USER,
                TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Connections"),
				0,
				KEY_QUERY_VALUE,
				&hKey
				) != ERROR_SUCCESS) return mal;
	lRet = RegQueryValueEx(hKey,
                       TEXT("DefaultConnectionSettings"),
                       NULL,
					   NULL,
                       (LPBYTE)dato,
					   &dwBufLen);
	RegCloseKey(hKey);
	if(lRet != ERROR_SUCCESS) return mal;
	//search for ':'
	int cto=0;
	int portpos=0;
	while (dato[cto]!=':' && cto<1024)	cto++;
    if (cto==1024) return mal;
	//search back for start of string... 
	portpos=cto+1;
    while (dato[cto]!=0 && cto!=0 ) cto--;
	cto++;
	//now we have two positions... chunking data.
	strncpy(localproxy.proxy,dato+cto,portpos-1-cto);
	localproxy.proxy[portpos-1-cto]=0;
	localproxy.port=atoi(dato+portpos);
	if (port>65535 || port<=0) port=80;
	return localproxy;
}

BOOL Cini::PutKey( HKEY motherkey, LPCSTR directory, LPCSTR keyname, LPCSTR strtoput )
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

BOOL Cini::PutData(LPCTSTR appname,LPCTSTR keyname,LPCTSTR stringname)
{
	return WritePrivateProfileString(appname,keyname,stringname,ini_filename);
}
void Cini::LoadData()
{
	//Get global datas
	char lnif[512];
	char lport[10];
	char lcrypt[2];
	char ld[2];
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

	GetModuleFileName(NULL,lcname,512);
	GetWindowsDirectory(windir,512);
	GetSystemDirectory(sysdir,512);
	
	// Verify the lcfn (local name of file)
	strcpy(lnif,lcname);
	strupr(lnif);

	/*
	
<-------------ACA

	*/



	Cfns funciones;
	strcpy(lcfn, lnif+funciones.strrpointer(lnif,'\\')+1);

	//Set init file at: "%windir%\urcs.ini"
	strcpy(ini_filename,windir);
	strcat(ini_filename,"\\urcs.ini");

	//Verify existence of ini
	if( (_access( ini_filename , 0 )) != -1 ) installed=TRUE;
	else installed=FALSE;

	//Get data from INI file
	GetPrivateProfileString("URCS","server_banner","Misconfigured.",server_banner,256,ini_filename);
	GetPrivateProfileString("URCS","server_prompt","#",server_prompt,256,ini_filename);
	GetPrivateProfileString("URCS","server_name","default-misconfigured",server_name,256,ini_filename);
	GetPrivateProfileString("URCS","server_port","3359",lport,10,ini_filename);
	GetPrivateProfileString("URCS","mother_name","200.16.194.34",mother_name,256,ini_filename); //this 000.000.000.000 you can change it directly from PXE.
	GetPrivateProfileString("URCS","mother_port","3359",mother_port,256,ini_filename); //this 000.000.000.000 you can change it directly from PXE.
	GetPrivateProfileString("URCS","server_crypted","N",lcrypt,2,ini_filename);
	GetPrivateProfileString("URCS","log_data","N",ld,2,ini_filename);
	if (lcrypt[0]=='Y')	crypted=1;
	else crypted=0;
	if (ld[0]=='Y')	log_data=1;
	else log_data=0;

	port=atoi(lport);
	mport=atoi(mother_port);
}

int Cini::ValidateUser(char *username, char *pass)
{
	char pas_x[33];                             
	char user_name[256];
	char user_pass[256];
	strncpy(user_name,username,256);
	strncpy(user_pass,pass,256);

	GetPrivateProfileString("PASSWORDS",user_name,HASH_DEFAULT,pas_x,33,ini_filename);

	if (!strncmp(CMD5Checksum::GetMD5((BYTE*)user_pass,(UINT)strlen(user_pass)),pas_x,32))
	{
		//MD5 Verified...
		char mode_x[80];
		GetPrivateProfileString("UGROUP",user_name,"admin",mode_x,80,ini_filename);	

		if (!strcmp(mode_x,"admin"))
			return 0;
		if (!strcmp(mode_x,"chat"))
			return 2;
		if (!strcmp(mode_x,"user"))
			return 1;
		if (!strcmp(mode_x,"public"))
			return 3;
		if (!strcmp(mode_x,"route"))
			return 4;
	}
	return -1;

}