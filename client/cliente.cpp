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
#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include <io.h>
#include <stdlib.h>
#include <stdio.h>


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
typedef struct
{
	short bt;
	char data[4000];
}ftrs;

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
	return 1; //socket started-.
}

int CUclient::senddata(char *data)
{
	int dlenght=strlen(data); //
	char bftsnd[4096]; //buffer to send
	if ( dlenght>4092 ) return -1;		//data is too large...
	memcpy(&bftsnd,&dlenght,4);
	//progressive encode of data.
	for (int g=0;g<dlenght;g++) data[g]=255-data[g];
	strcpy(bftsnd+4,data);
	int tr=send(a,bftsnd,dlenght+4,0);
    return tr;
}

int CUclient::sendfile()
{
	char intep[1];
	char rmf[512];
	if (recv(a,rmf,512,0)<=0) return -3;
	if (recv(a,intep,1,0)<=0) return -3;
	if (intep[0]==1)
	{
        //remote host accept creation of files... openning.
		ftrs ft;
		int fh;
		unsigned int nbytes = sizeof(ft.data), bytesread;
		if( (fh = _open(rmf , _O_RDONLY | _O_BINARY )) != -1 ) 
		{
			while( ( bytesread = _read( fh, ft.data, nbytes ) ) > 0 )
			{
				ft.bt=(short)bytesread;
				if (ft.bt>0) 
				{
					if (send(a,(char *)&ft,sizeof(ftrs),0)<=0) return -3;
				}
			}
			_close( fh );
			ft.bt=0;
			if (send(a,(char *)&ft,sizeof(ftrs),0)<=0) return -3;
			return 1;
		}
		ft.bt=0;
		if (send(a,(char *)&ft,sizeof(ftrs),0)<=0) return -3;
	}
	return 1;
}
int CUclient::recvfile()
{
	ftrs ft;
	char intep[1];
	char rmf[512];
	if (recv(a,rmf,512,0)<=0) return -3;
	
	//commencing  openning file
	DWORD tot=0;
	int fh;
	if( (fh = _open(rmf, _O_WRONLY | _O_CREAT | _O_BINARY )) != -1 )
	{
		intep[0]=1;
		if (send(a,intep,1,0)<=0) return -3; //ok opened
		do
		{
			if (recv(a,(char *)&ft,sizeof(ft),0)<=0) return -3;
			tot=tot+ft.bt;
			_write(fh,&ft.data,ft.bt);
		}
		while (ft.bt > 0);
		int jh=_close( fh );

		if (tot==0) 
			DeleteFile(rmf);

		return 1;
	}
	else 
	{
		intep[0]=0;
		if (send(a,intep,1,0)<=0) return -3;
		return -2; //cannot open localfile
	}
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
		for (int g=0;g<header;g++) data[g]=255-data[g];
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
