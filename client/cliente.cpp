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
#include "cliente.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
Unmanarc implementation of client managment...
Special thanks to: 

TheCPUWizard: for the idea of algorithm to transmit data with nagle implementing a new header.
Bistok: for OOP help
DanM: for help about socket's functions / hostnames
*/

// Library for manage network as object.-

CUclient::CUclient()
{ //Constructor
}
CUclient::~CUclient() 
{ //destructor
	
}

void chrcpy(char *a, char *b, int c)
{
	for (int p=0;p<c;p++)
        a[p]=b[p];
}

int CUclient::start_client()
{
	//Do first negot... sending client data.
	char buff[4096]; //buffer managment
	int i; //error managment

	memset(&buff,0,4096); //start buff

	i=send(a,"iux",3,0); //Send initialization command.

	if (i<=0) return -1; //if socket error, return -1 (socket error)
	i=recv(a,buff,32,0); //receive 32bytes data of key.
	
	if(i!=32) return -2; //if not receive 32 bytes of protocol, return error -2 (incompatible protocol or socket error)
	else strncpy(key,buff,32); //if well, set 128bit key

	if (!strcmp(key,"00000000000000000000000000000000")) crypted=0;//no enc.
	else 
	{
		if (!strcmp(key,"--------------------------------")) return -3; //Server full
		else crypted=1; //yes, is crypted.
	}
	return 1; //socket started-.
}

int CUclient::senddata(char *data)
{
	int dlenght=strlen(data); //
	char bftsnd[4096]; //buffer to send
	if ( dlenght>4092 ) return -1;		//data is too large...
	memcpy(&bftsnd,&dlenght,4);
	strcpy(bftsnd+4,data);
	int tr=send(a,bftsnd,dlenght+4,0);
    return tr;
}

int CUclient::receive(char *data)
{
	int header; //header
	int mtr=GetBlock(&header, 4); //get header
	if (mtr<=0) return -1;
	if (header>4092) //is command, not needed to receive data
		strcpy(data,"");
	else
	{
		int b=GetBlock(data, header); //Get block that contains the data
		if (b<0) return -1; //if an error ocurrs return error
		else 
		{
			data[b]=0; //null-terminate the string
		}
	}
	return header;
}

int CUclient::GetBlock(void *p, int len)
{
	char tmpdata[4096];
	int tmpLen = recv(a, tmpdata, len, 0); //maximum 4096 bytes
	if (tmpLen<=0)
		return -1;
	memcpy(p,&tmpdata,tmpLen);
	return tmpLen;
}

int CUclient::closesock()
{
	return closesocket(a); //Close opened connection.
}
int CUclient::connectto(char *ip, unsigned int port)
{
	struct sockaddr_in Server_add;
	struct hostent *hentry;

	WORD wVersionRequested;
	WSADATA wsaData;
	// Start Winsock
	wVersionRequested = MAKEWORD( 1, 0 );
	if( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	{
		a = socket( AF_INET, SOCK_STREAM, 0 );
		// Connect Server
		if ((hentry = gethostbyname(ip)) == NULL)
		{
			//error: go out. Cannot resolve.
			return -1;
		}
		else
		{
			memset((char *)&Server_add, 0, sizeof(Server_add));
			memcpy(&Server_add.sin_addr.s_addr, *(hentry->h_addr_list),sizeof(Server_add.sin_addr.s_addr));
			//host are copied to Server_add
			Server_add.sin_port = htons(port);
			Server_add.sin_family = AF_INET;

			//ready.
			Server_add.sin_family      = AF_INET;
			Server_add.sin_port        = htons( port );


			if( connect( a, (struct sockaddr *) &Server_add, sizeof( Server_add ) ) == 0 )
				return 1;
			else
				return -1;
		}
	}
	return -1;
}
