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
		strcpy(ip,inet_ntoa(cliAddr.sin_addr));
	else
		strcpy(ip,"000.000.000.000");
	return m;
	// Connection accepted, socket number returned.
}

int CUserver::startserver(unsigned int port)
{
	//Commencing any service on static port...
	//Review the port
	if (port<1 && port>65535)
	{
		return 0; // Port out of range
	}

	int ServerSize=sizeof(sockaddr_in);
	struct sockaddr_in servAddr;
	
	WORD wVersionRequested;
	WSADATA	wsaData;

	// ------------------------- Establecer variables
	
	wVersionRequested = MAKEWORD( 1,1);
	if (WSAStartup(wVersionRequested, &wsaData)!=0) 
	{
		return -1; // Cannot start sockets
	}

	d=socket(AF_INET, SOCK_STREAM, 0);
	if (d==INVALID_SOCKET)
		return -2;


	//protocol
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	//bind port
	if(bind(d, (struct sockaddr *) &servAddr, sizeof(servAddr))<0)
		return -3; //error! can't bind port

	listen(d,5);
	return 1;
}
