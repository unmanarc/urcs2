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
#pragma once

class Cregistry
{
public:
	Cregistry(void);

	bool GetKey( HKEY motherkey, LPCSTR directory, LPCSTR keyname, LPCSTR stroutput,DWORD dwBufLen);
	BOOL PutKey( HKEY base, LPCSTR path, LPCSTR valuename, LPCSTR value );
	bool DelKey(HKEY motherkey,LPCSTR directory,LPCSTR Valuetodel);

	~Cregistry(void);
};
