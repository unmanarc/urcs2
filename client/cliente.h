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

class CUclient  
{
	/*
	This class accept connections and return socket's of clients...
	*/
public:
	CUclient(); 
	int connectto(char *ip, unsigned int port);
	int receive(char *data);
	int senddata(char *data);
	int closesock();
	int start_client();
	int GetBlock(void *p, int len);
	~CUclient();

protected:
	SOCKET a;
	char key[32];
	BOOL crypted;


};
