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
#include "ini_man.h"

class Cproxy
{
public:
	Cproxy(Cini *inim);
	int start_proxy_instance(SOCKET client, SOCKET server);
	SOCKET connectto(int prx);
	int attach(char *ip, unsigned int port, SOCKET client);
	SOCKET connecttoproxy();
	int transfer();
	~Cproxy();

protected:
	int GetBlock(void *p, int len,SOCKET m);
	SOCKET x;
	SOCKET y;
	Cini *ini;

};