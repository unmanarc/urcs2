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
#include "Server.h"
#include "core_s.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Library for manage network as object.-

CUserver::CUserver() 
{ //Constructor
	
}
CUserver::~CUserver() 
{ //destructor
	
}

int CUserver::closeserver()
{
	return closesocket(d);
}

SOCKET CUserver::s_accept(char *ip)
{
	struct sockaddr_in cliAddr;
	int	cliLen = sizeof(cliAddr);
	SOCKET m;
	BOOL s=FALSE;
	m=accept(d, (struct sockaddr *) &cliAddr, &cliLen);
	if (m>0)
		strncpy(ip,inet_ntoa(cliAddr.sin_addr),17);
	else
		strcpy(ip,"000.000.000.000");
	return m;
	// Connection accepted, socket number returned.
}

int CUserver::startserver(unsigned int port)
{
	//Commencing any service on static port...
	//Review the port
	int ServerSize=sizeof(sockaddr_in);
	struct sockaddr_in servAddr;

	if (port<1 && port>65535) return 0; // Port out of range
	if ((d=socket(AF_INET, SOCK_STREAM, 0))==INVALID_SOCKET) return -2; //Cannot start sockets

	//protocol
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	if(bind(d, (struct sockaddr *) &servAddr, sizeof(servAddr))<0)	return -3; //error! can't bind port
	if (listen(d,5)==SOCKET_ERROR) return -3;
	return 1;
}
int CUserver::start_winsock()
{
	WORD wVersionRequested;
	WSADATA	wsaData;
	wVersionRequested = MAKEWORD( 1,1);
	if (WSAStartup(wVersionRequested, &wsaData)!=0) return -1; // Cannot start sockets
	return 1;
}
