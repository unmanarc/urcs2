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
#include "StdAfx.h"
#include ".\registry.h"

Cregistry::Cregistry(void)
{
}

Cregistry::~Cregistry(void)
{
}

BOOL Cregistry::PutKey( HKEY motherkey, LPCSTR directory, LPCSTR keyname, LPCSTR strtoput )
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
bool Cregistry::GetKey( HKEY motherkey, LPCSTR directory, LPCSTR keyname, LPCSTR stroutput,DWORD dwBufLen)
{
	LONG lRet;
	HKEY hKey;
	if(RegOpenKeyEx(motherkey,directory,0,KEY_QUERY_VALUE,&hKey) != ERROR_SUCCESS) return false;
	lRet = RegQueryValueEx(hKey,keyname,NULL,NULL,(LPBYTE)stroutput,&dwBufLen);
	RegCloseKey(hKey);
	if(lRet != ERROR_SUCCESS) return false;
	return true;
}

bool Cregistry::DelKey(HKEY motherkey,LPCSTR directory,LPCSTR Valuetodel)
{
	//RegDeleteValue 
	DWORD rc;
	HKEY todel=NULL;
	if (RegOpenKeyEx( motherkey,directory,0,KEY_WRITE,&todel)!= ERROR_SUCCESS) return false;
	rc = RegDeleteValue( todel, Valuetodel );
    if(rc != ERROR_SUCCESS && rc != ERROR_FILE_NOT_FOUND)	return false;
	RegCloseKey(todel);
	return true;
}