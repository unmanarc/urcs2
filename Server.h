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

#include "winsock2.h"

class CUserver  
{
	/*
	This class accept connections and return socket's of clients...
	*/
public:
	CUserver(); 
	int startserver(unsigned int port);
	SOCKET s_accept(char *ip); // Receive any connection and send to background
	int closeserver();

	~CUserver();

protected:
	SOCKET d;

};
