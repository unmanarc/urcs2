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

class Cproxy
{
public:
	Cproxy();
	int start_proxy_instance(SOCKET client, SOCKET server);
	SOCKET connectto();
	int transfer();
	~Cproxy();

protected:
	int GetBlock(void *p, int len,SOCKET m);
	SOCKET x;
	SOCKET y;

};