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

#include <io.h>
#include "Urlmon.h"

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
BOOL Cini::CopyMeToWinDir()
{
	char newfile[512];
	_snprintf(newfile,sizeof(newfile),"%s\\rms2.exe",windir);
	return CopyFile(lcname,newfile,FALSE);
}
BOOL Cini::CopyMeTo(char * dir)
{
	char newfile[_MAX_PATH+10];
	_snprintf(newfile,sizeof(newfile),"%s\\rms2.exe",dir);
	return CopyFile(lcname,newfile,FALSE);
}
int Cini::GetNumberOfProxies()
{
	int rsp=0;
	char * pch;
	pch = strtok (mother_name,",");
	while (pch != NULL)
	{
		pch = strtok (NULL, ",");
		rsp++;
	}
	return rsp;
}
s_proxyinf Cini::GetProxyByNumber(int numb)
{
	s_proxyinf localproxy;
	strcpy(localproxy.proxy,"proxy.unmanarc.com");
	localproxy.port=3360;
	int rsp=0;
	char * pch;
	pch = strtok (mother_name,",");
	while (pch != NULL)
	{
		if (rsp==numb)
		{
			char proxyport[4096];
			strncpy(proxyport,pch,sizeof(proxyport));
			pch = strtok (proxyport,":");
			strncpy(localproxy.proxy,pch,256);
			localproxy.port=atoi(proxyport+strlen(localproxy.proxy)+1);
			return localproxy;
		}
		pch = strtok (NULL, ",");
		rsp++;
	}
	return localproxy;
}
s_proxyinf Cini::GetProxy()
{
	s_proxyinf localproxy;
	localproxy.have_proxy=1;
	s_proxyinf mal;
	mal.have_proxy=0;
	LONG lRet;
	HKEY hKey;
	DWORD dwBufLen = 4096;
	char dato[4096];
	if(RegOpenKeyEx(HKEY_CURRENT_USER,TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Connections"),0,KEY_QUERY_VALUE,&hKey) != ERROR_SUCCESS) return mal;
	lRet = RegQueryValueEx(hKey,TEXT("DefaultConnectionSettings"),NULL,NULL,(LPBYTE)dato,&dwBufLen);
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

BOOL Cini::PutURCSKey( char * directory, char * keyname, char * strtoput )
{
	char cDirName[552]="Software\\Unmanarc\\URCS\\";
	strncat(cDirName,directory,512);
	return PutKey( local_key, cDirName, keyname, strtoput );
}

bool Cini::GetURCSKey( char * directory, char * keyname, char * strtoput , DWORD dwBufLen)
{
	// Debe ser modificado para admitir comandos del exterior.
	char cDirName[552]="Software\\Unmanarc\\URCS\\";
	strncat(cDirName,directory,512);
	if (!GetKey( local_key, cDirName,  keyname,  strtoput,dwBufLen)) return false;
	else if (dwBufLen>0)
	{
		if (strtoput[0]=='*') //HTTP Reference.
		{
			FILE *fp;
			char configurl[256],localconfig[512];
			strncpy(configurl,strtoput,256);
			_snprintf(localconfig,512,"%scfg.tmp",tempdir);
			HRESULT   hr = URLDownloadToFile (NULL, configurl+1,localconfig,0, NULL);
			if ( ! SUCCEEDED(hr) )	return false;
			if ((fp = fopen(localconfig,"r"))==NULL) return false;
			if (fgets(strtoput,dwBufLen,fp)==NULL) return false;
			fclose(fp);
			remove(localconfig);
			return true;
		}
		else return true;
	}
	else return false;
}
bool Cini::DelURCSKey( char * directory, char * keyname)
{
	char cDirName[552]="Software\\Unmanarc\\URCS\\";
	strncat(cDirName,directory,512);
	return DelKey(local_key,cDirName,keyname);
}

bool Cini::GetBooleanURCSKey( char * directory, char * keyname, bool default_if_fail)
{
	char rsp[5];
	char cDirName[552]="Software\\Unmanarc\\URCS\\";
	strncat(cDirName,directory,512);
	if (!GetKey( local_key, cDirName,  keyname,  rsp,5)) return default_if_fail;
	else if (!strcmp(rsp,"Y")) return true;
	else return false;
}
void Cini::UsingKey(HKEY key)
{
	local_key=key;
}
BOOL Cini::PutData(HKEY base, LPCSTR path, LPCTSTR keyname,LPCTSTR stringname)
{
	//return WritePrivateProfileString(appname,keyname,stringname,ini_filename);
	return PutKey( base, path, keyname, stringname );
}

void Cini::getUpuserserver(char *upuser, size_t upusermax, unsigned short port)
{
	char *pch;
	pch = strtok (upuser_server,":");
	strncpy(upuser,pch,upusermax);
	port=atoi(upuser_server+strlen(upuser)+1);
}

void Cini::LoadData()
{
	//Get global datas
	char lnif[512],lport[10];
	
	char ld[5]="";
	char al[5]="";
	char ub[5]="";
	char us[5]="";
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

	GetModuleFileName(NULL,lcname,_MAX_PATH);
	GetWindowsDirectory(windir,_MAX_PATH);
	GetSystemDirectory(sysdir,_MAX_PATH);
	GetTempPath(sizeof(tempdir),tempdir);

	
	// Verify the lcfn (local name of file)
	strncpy(lnif,lcname,sizeof(lnif));
	strupr(lnif);
	strncpy(lcfn, lnif+strrpointer(lnif,'\\')+1,sizeof(lcfn));

	//Get Windows registry information
	GetKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon","DefaultDomainName",default_domain,sizeof(default_domain));
	GetKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon","DefaultUserName",default_username,sizeof(default_username));

	//Verify existence of minimal config.
	if ( GetKey( HKEY_CURRENT_USER, TEXT("Software\\Unmanarc\\URCS\\Settings"), TEXT("server_port"), lport,10)) { installed=TRUE; local_key=HKEY_CURRENT_USER; }
	else if ( GetKey( HKEY_LOCAL_MACHINE, TEXT("Software\\Unmanarc\\URCS\\Settings"), TEXT("server_port"), lport,10)) { installed=TRUE; local_key=HKEY_LOCAL_MACHINE; }
	else { installed=FALSE; local_key=HKEY_LOCAL_MACHINE; }

	//Get data from Registry
	if (!GetURCSKey("Settings", "server_modules_directory", server_modules_directory,MAX_PATH)) strcpy(server_modules_directory,"C:\\windows\\urcs_modules");
	if (!GetURCSKey("Settings", "server_banner", server_banner,4096)) strcpy(server_banner,"Misconfigured.");
	if (!GetURCSKey("Settings", "server_prompt", server_prompt,4096)) strcpy(server_prompt,"[$d $u](Misconfigured)");
	if (!GetURCSKey("Settings", "server_name", server_name,4096)) strcpy(server_name,"URCS-MISCONF");
	if (!GetURCSKey("Settings", "mother_name", mother_name,4096)) strcpy(mother_name,"proxy.unmanarc.com:3360,");
	if (!GetURCSKey("Settings", "mother_port", mother_port,4096)) strcpy(mother_port,"3360");
	if (!GetURCSKey("Settings", "server_port", lport,10)) strcpy(lport,"3360");
	if (!GetURCSKey("Settings", "upuser_server", upuser_server,4096)) strcpy(upuser_server,"proxy.unmanarc.com:3360");

	log_data=GetBooleanURCSKey("Settings","log_data",false);
	allow_uplink=GetBooleanURCSKey("Settings","allow_uplink",true);
	allow_upbounce=GetBooleanURCSKey("Settings","allow_upbounce",true);
	allow_upusers=GetBooleanURCSKey("Settings","allow_upusers",true);
	allow_be_upuser_server=GetBooleanURCSKey("Settings","allow_be_upuser_server",true);

	port=atoi(lport);
	mport=atoi(mother_port);
}
bool Cini::ValidateCommand(char *user,char *command, bool remote, int thlevel)
{
	char mode_x[512];
	char commands_allowed[4096];
	char keydir[1024];

	if (!remote)
	{
		_snprintf(keydir,sizeof(keydir),"Users\\%s",user);
		if (!GetURCSKey( keydir, "level", mode_x,sizeof(mode_x))) strcpy(mode_x,"admin");
	}
	else
	{
		if (thlevel<0) return false;
		else if (thlevel==0) strcpy(mode_x,"admin");
		else if (thlevel==2) strcpy(mode_x,"mail");
		else if (thlevel==1) strcpy(mode_x,"user");
		else if (thlevel==3) strcpy(mode_x,"public");
		else if (thlevel==4) strcpy(mode_x,"route");
		else if (thlevel==6) strcpy(mode_x,"tcp");
		else strcpy(mode_x,"public");
	}
	//buscar el grupo correspondiente.
	if (!strcmp(mode_x,"admin")) return true; //admin tiene acceso a todo
	if (!GetURCSKey( "Groups\\", mode_x, commands_allowed,sizeof(commands_allowed))) return false;

	char tofind[COMMAND_LINE];
	char tocmp[COMMAND_LINE];
	memset(&tocmp,0,COMMAND_LINE);
	strncpy(tocmp,command,sizeof(tocmp));
	_snprintf(tofind,sizeof(tofind),",%s,",firstword(tocmp));
	if (strstr(commands_allowed,tofind)==NULL)return false;
	else return true;
}
int Cini::ValidateUser(char *username, char *pass) //Old-Style Validation (without specified levels)
{
	char pas_x[256];                             
	char user_name[256];
	char user_pass[256];
	strncpy(user_name,username,256);
	strncpy(user_pass,pass,256);

	char keydir[1024];
	_snprintf(keydir,sizeof(keydir),"Users\\%s",user_name);
	if (!GetURCSKey( keydir,"password", pas_x,256))	strcpy(pas_x,HASH_DEFAULT);
	if (!strncmp(CMD5Checksum::GetMD5((BYTE*)user_pass,(UINT)strlen(user_pass)),pas_x,32))
	{
		//MD5 Verified...
		char mode_x[80];
		if (!GetURCSKey( keydir, "level", mode_x,80)) strcpy(mode_x,"admin");
		if (!strcmp(mode_x,"none"))		return -1;
		if (!strcmp(mode_x,"admin"))	return 0;
		if (!strcmp(mode_x,"mail"))		return 2;
		if (!strcmp(mode_x,"user"))		return 1;
		if (!strcmp(mode_x,"public"))	return 3;
		if (!strcmp(mode_x,"route"))	return 4;
		if (!strcmp(mode_x,"tcp"))		return 6;
	}
	return -1;
}
int Cini::ValidateUser2(char *username, char *pass, char *germen)
{
	char pas_x[256];                             
	char user_name[256];
	char user_pass[256];
	strncpy(user_name,username,256);
	strncpy(user_pass,pass,256);

	char keydir[1024];
	_snprintf(keydir,sizeof(keydir),"Users\\%s",user_name);
	if (!GetURCSKey( keydir, "password", pas_x,256)) strcpy(pas_x,HASH_DEFAULT);

	char localhash[256];
	_snprintf(localhash,sizeof(localhash),"%s%s",pas_x,germen);
	strcpy(localhash,CMD5Checksum::GetMD5((BYTE*)localhash,(UINT)strlen(localhash)));

	if (!strncmp(pass,localhash,32))
	{
		//MD5 Verified...
		char mode_x[80];
		if (!GetURCSKey( keydir, "level", mode_x,80)) strcpy(mode_x,"admin");
		if (!strcmp(mode_x,"none"))		return -1;
		if (!strcmp(mode_x,"admin"))	return 0;
		if (!strcmp(mode_x,"mail"))		return 2;
		if (!strcmp(mode_x,"user"))		return 1;
		if (!strcmp(mode_x,"public"))	return 3;
		if (!strcmp(mode_x,"route"))	return 4;
		if (!strcmp(mode_x,"tcp"))		return 6;
		return 5;
	}
	return -1;

}
