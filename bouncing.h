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

class Cbouncing 
{
public:
	Cbouncing(void);

	int listener(unsigned short localPort,SOCKET proxyUsed,char *remoteEndPoint,unsigned short remotePort);//Accept/Initialize tha proxy
    int proxy_manager(SOCKET proxyUsed);
	void proxy_accept(SOCKET proxyUsed);
	void proxy_deny(SOCKET proxyUsed);

	~Cbouncing(void);
protected:
	char gremoteEndPoint[256];
	unsigned short gremotePort;


};
