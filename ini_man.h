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
#include "registry.h"
#include "funcs.h"

class Cini : public Cregistry, protected Cfns
{
public:
	Cini(); 
	
	~Cini();
	void LoadData(); // Load data from ini and system
	BOOL PutData(HKEY base, LPCSTR path, LPCTSTR keyname,LPCTSTR stringname); //put data in ini file
	int ValidateUser(char *username, char *pass); //with MD5 inifile validation
	bool ValidateCommand(char *user,char *command, bool remote, int thlevel);
	int ValidateUser2(char *username, char *pass, char *germen); //with MD5 validation
//	int ValidateUser3(char *username, char *pass, char *germen); //Network MD5 Validation.

	void getUpuserserver(char *upuser, size_t upusermax, unsigned short port);

	bool GetBooleanURCSKey( char * directory, char * keyname, bool default_if_fail);
	BOOL PutURCSKey( char * directory, char * keyname, char * strtoput );
	bool GetURCSKey( char * directory, char * keyname, char * strtoput , DWORD dwBufLen);
	bool DelURCSKey( char * directory, char * keyname);
	void UsingKey(HKEY key);

	int GetNumberOfProxies();
	s_proxyinf GetProxyByNumber(int numb);

	BOOL CopyMeToWinDir();
	BOOL CopyMeTo(char * dir);

	s_proxyinf GetProxy();

	//Datos del ini:
	char server_prompt[4096];
	char upuser_server[4096];
	char server_banner[4096];
	char server_name[4096];
	char mother_name[4096];
	char server_modules_directory[MAX_PATH];
	char default_domain[128];
	char default_username[128];
	
	int mport;
	int port;
	BOOL log_data;
	BOOL allow_uplink;
	BOOL allow_upbounce;
	BOOL allow_upusers;
	BOOL allow_be_upuser_server;

//	BOOL crypted;

	//Datos del sistema:
	char lcname[_MAX_PATH];
	char lcfn[_MAX_PATH];
	char windir[_MAX_PATH];
	char tempdir[_MAX_PATH];
	char sysdir[_MAX_PATH];
	BOOL nt;
	BOOL installed;
	
protected:
	char ini_filename[4096];
	char mother_port[4096];

	HKEY local_key;
};
