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
#include "file_transfer.h"

#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include <io.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


Ctransf::Ctransf()
{ //Constructor
}
Ctransf::~Ctransf()
{
}
int Ctransf::uploadto(char *host, char *localfile, char *remotefile)
{
	struct sockaddr_in Server_add;
	struct hostent *hentry;

//	WORD wVersionRequested;
//	WSADATA wsaData;

	// Start Winsock
//	wVersionRequested = MAKEWORD( 1, 0 );
//	if( WSAStartup( wVersionRequested, &wsaData ) == 0 )
//	{
		// Make Socket
		SOCKET Sock = socket( AF_INET, SOCK_STREAM, 0 );
		if ((hentry = gethostbyname(host)) == NULL)	return -4;
		else
		{
			// Connect Server

			memset((char *)&Server_add, 0, sizeof(Server_add));
			memcpy(&Server_add.sin_addr.s_addr, *(hentry->h_addr_list),sizeof(Server_add.sin_addr.s_addr));

			Server_add.sin_family      = AF_INET;
			Server_add.sin_port        = htons( 1980 );

			if( connect( Sock, (struct sockaddr *) &Server_add, sizeof( Server_add ) ) == 0 )
			{
				char rmf[512];
				strncpy(rmf,remotefile,512);
				send(Sock,"put",30,0);
				send(Sock,rmf,512,0);
				//Commencing opening file...
				char buffer[4096];
				int fh;
				unsigned int nbytes = 4096, bytesread;
				char lcfile[512];
				strncpy(lcfile,localfile,512);
				if( (fh = _open(lcfile , _O_RDONLY | _O_BINARY )) == -1 ) return -1;
				else
				{
					while( ( bytesread = _read( fh, buffer, nbytes ) ) > 0 )
						send(Sock,buffer,bytesread,0);
					_close( fh );
					closesocket(Sock);
					return 1;
				}
			}
			else return -2;
		//Connection Closed:
		}
//	}
	return -3;
}

int Ctransf::downloadfrom(char *host,char *remotefile,char *localfile)
{
	struct sockaddr_in Server_add;
	struct hostent *hentry;

	//WORD wVersionRequested;
	//WSADATA wsaData;

	// Start Winsock
//	wVersionRequested = MAKEWORD( 1, 0 );
//	if( WSAStartup( wVersionRequested, &wsaData ) == 0 )
//	{
		// Make Socket
		SOCKET Sock = socket( AF_INET, SOCK_STREAM, 0 );
		if ((hentry = gethostbyname(host)) == NULL)	return -4;
		else
		{
			// Connect Server
			memset((char *)&Server_add, 0, sizeof(Server_add));
			memcpy(&Server_add.sin_addr.s_addr, *(hentry->h_addr_list),sizeof(Server_add.sin_addr.s_addr));

			Server_add.sin_family      = AF_INET;
			Server_add.sin_port        = htons( 1980 );

			if( connect( Sock, (struct sockaddr *) &Server_add, sizeof( Server_add ) ) == 0 )
			{
				char integ[5];
				char rmf[512];
				strncpy(rmf,remotefile,512);

				send(Sock,"get",30,0);
				send(Sock,rmf,512,0);
				recv2(Sock,integ,5,0);

				if (!strcmp(integ,"+"))
				{
					//Commencing writing file...
					int fh;
					char lcfile[512];
					strncpy(lcfile,localfile,512);
					if( (fh = _open(lcfile, _O_WRONLY | _O_CREAT | _O_BINARY )) != -1 )
					{
						unsigned int iRead;
						char byt[4096];
						do
						{
							iRead = recv2(Sock,byt,4096,0);
							_write(fh,&byt,iRead);
						}
						while (iRead > 0);
						int jh=_close( fh );
						return 1;
					}
				}
				else return -1;
			}
			else return -2;
		//Connection Closed:
		}
	//}
	return -3;
}
