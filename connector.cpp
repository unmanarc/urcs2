#include "StdAfx.h"
#include "connector.h"

Cconnector::Cconnector(void)
{
}

Cconnector::~Cconnector(void)
{
}

void Cconnector::connectto(char *ip,unsigned int port)
{
	struct hostent *hentry;
	struct sockaddr_in Server_add;
//	WORD wVersionRequested;
//	WSADATA wsaData;
	// Start Winsock
//	wVersionRequested = MAKEWORD( 1, 0 );
//	if( WSAStartup( wVersionRequested, &wsaData ) == 0 )
//	{

		SOCKET Sock = socket( AF_INET, SOCK_STREAM, 0 ); //open socket
		// Connect Server
		if ((hentry = gethostbyname(ip)) == NULL)
		{
			//error: go out. Cannot resolve.
			return;
		}
		else
		{
			// Make Socket
			memset((char *)&Server_add, 0, sizeof(Server_add));
			memcpy(&Server_add.sin_addr.s_addr, *(hentry->h_addr_list),sizeof(Server_add.sin_addr.s_addr));
			//host are copied to Server_add
			Server_add.sin_port = htons(port);
			Server_add.sin_family = AF_INET;

			// Connect Server
			if( connect( Sock, (struct sockaddr *) &Server_add, sizeof( Server_add ) ) == 0 )
			{
				closesocket(Sock);
			}
			else
				return;
		}
//	}
//	else
//		return;
}