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
#include "proxy.h"
#include "ini_man.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef struct
{
	short bt;
	char data[4000];
}ftrs;


Cproxy::Cproxy()
{ //Constructor
}

Cproxy::~Cproxy()
{ //destructor
	
}
SOCKET Cproxy::connectto()
{
	Cini inim;
	inim.LoadData();

	struct hostent *hentry;
	struct sockaddr_in Server_add;
	WORD wVersionRequested;
	WSADATA wsaData;
	// Start Winsock
	wVersionRequested = MAKEWORD( 1, 0 );
	if( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	{

		SOCKET Sock = socket( AF_INET, SOCK_STREAM, 0 ); //open socket
		// Connect Server
		if ((hentry = gethostbyname(inim.mother_name)) == NULL)
		{
			//error: go out. Cannot resolve.
			return 0;
		}
		else
		{
			// Make Socket
			memset((char *)&Server_add, 0, sizeof(Server_add));
			memcpy(&Server_add.sin_addr.s_addr, *(hentry->h_addr_list),sizeof(Server_add.sin_addr.s_addr));
			//host are copied to Server_add
			Server_add.sin_port = htons(inim.port);
			Server_add.sin_family = AF_INET;

			// Connect Server
			if( connect( Sock, (struct sockaddr *) &Server_add, sizeof( Server_add ) ) == 0 )
			{
				//do negot with server.
				char init_d[4];
				strcpy(init_d,"ipx");
				send(Sock,init_d,3,0);
				send(Sock,inim.server_name,512,0);
				// A Server has my data... now, all information that i'll receive is a prx
				// Start server with proxy mode (non close admited)
				return Sock;
			}
			else
			{
				return 0;
			}
		}
	}
	else
		return 0;
}

SOCKET Cproxy::connecttoproxy()
{
	Cini inim;
	inim.LoadData();
	s_proxyinf localproxy; 
	localproxy=inim.GetProxy(); //get windows proxy data.
	if (!localproxy.have_proxy) return 0; //not proxy defined.
	struct hostent *hentry;
	struct sockaddr_in Server_add;
	WORD wVersionRequested;
	WSADATA wsaData;
	// Start Winsock
	wVersionRequested = MAKEWORD( 1, 0 );
	if( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	{
		SOCKET Sock = socket( AF_INET, SOCK_STREAM, 0 ); //open socket
		// Connect to proxy...
		if ((hentry = gethostbyname(localproxy.proxy)) == NULL)
		{
			//error: go out. Cannot resolve.
			return 0;
		}
		else
		{
			// Make Socket
			memset((char *)&Server_add, 0, sizeof(Server_add));
			memcpy(&Server_add.sin_addr.s_addr, *(hentry->h_addr_list),sizeof(Server_add.sin_addr.s_addr));
			//host are copied to Server_add
			Server_add.sin_port = htons(localproxy.port);
			Server_add.sin_family = AF_INET;
			// Connect Server
			if( connect( Sock, (struct sockaddr *) &Server_add, sizeof( Server_add ) ) == 0 )
			{
				char negot[1024];
				sprintf(negot,"CONNECT %s:%u HTTP/1.0\n",inim.mother_name,inim.port);
				send(Sock,negot,(int)strlen(negot),0);//negotiate with proxy... 
				//do negot with server.
				char init_d[4];
				strcpy(init_d,"ipx");
				send(Sock,init_d,3,0);
				send(Sock,inim.server_name,512,0);
				// A Server has my data... now, all information that i'll receive is a prx
				// Start server with proxy mode (non close admited)
				return Sock;
			}
			else
			{
				return 0;
			}
		}
	}
	else
		return 0;
}

int Cproxy::transfer()
{
	//transfer first initial data from/to client server
	BOOL iscon=1;
	while(iscon)
	{
        //get from server (MAX 4096 bytes)
		int l=1;
		char buf[4096];
		GetBlock(&l,4,y); //get 1 block lenght
		if (l>4096) //is command... 
		{
			//transmit command and do..
			char buftsnd[4096];
			memcpy(&buftsnd,&l,4); //copy command
			if (send(x,buftsnd,4,0)<=0) return -2; //error client closes connection.

			if (l==4099) //server exit command
				iscon=0;
			if (l==5002) //server to client ftransfer
			{
				char fnd[512];
				char intep[1];
				ftrs ft;
				if (recv(y,fnd,512,0)<=0) return -3;
				if (send(x,fnd,512,0)<=0) return -2;
				if (recv(y,intep,1,0)<=0) return -3;
				if (send(x,intep,1,0)<=0) return -2;
				if (intep[0]==1)
				{
					ft.bt=1;
					while(ft.bt>0)
					{
						if (recv(x,(char *)&ft,sizeof(ft),0)<=0) return -2;
						if (send(y,(char *)&ft,sizeof(ft),0)<=0) return -3;
					}
				}

			}
			if (l==5001) //server to client ftransfer
			{
				char fnd[512];
				char intep[1];
				ftrs ft;
				if (recv(y,fnd,512,0)<=0) return -3;
				if (send(x,fnd,512,0)<=0) return -2;
				if (recv(x,intep,1,0)<=0) return -2;
				if (send(y,intep,1,0)<=0) return -3;
				if (intep[0]==0) break; //end's connection...
				ft.bt=1;
				while (ft.bt>0)
				{
					if (recv(y,(char *)&ft,sizeof(ft),0)<=0) return -3;
					if (send(x,(char *)&ft,sizeof(ft),0)<=0) return -2;
				}
			}

			if (l==4097 || l==4098) //command for passwords
			{ //need to receive data from client and send to server...
                int l2=1;
				char buf2[4096];
				char buf3[4096];
				memset(&buf2,0,4096);
				memset(&buf3,0,4096);
				GetBlock(&l2,4,x); //get 1 block lenght
				l2=GetBlock(&buf2,l2,x); //get 1 block data from client
				if (l2<0) return -2; //client closes connection

				//reorder received data in buf3
				memcpy(&buf3,&l2,4);
                strcpy(buf3+4,buf2);

				//send to server...
				if(send(y,buf3,l2+4,0)<=0) return -3; //server closes connection
			}
		}
		else //is data
		{
			l=GetBlock(&buf,l,y); //get 1 block data
			if (l>=0)  //not corrupted
			{
				//send data to client
				char buftsnd[4096];
				memset(&buftsnd,0,4096);
				memcpy(&buftsnd,&l,4);
				strcpy(buftsnd+4,buf);
				if (send(x,buftsnd,l+4,0)<=0) return -2; //error client closes connection.
			}
			else return -3; //server closes
		}


	}
	return 0;
}
int Cproxy::start_proxy_instance(SOCKET client, SOCKET server)
{
	x=client;
	y=server;
	int f=send(y,"CONNECT",80,0);
	if (f<=0) return -1;
	return f;
}

BOOL Cproxy::pingproxy(SOCKET server)
{
	int f=send(server,"PING",80,0);
	if (f<=0) return FALSE;
	return TRUE;
}
//protected area:

int Cproxy::GetBlock(void *p, int len, SOCKET m)
{
	char tmpdata[4096];
	int tmpLen = recv(m, tmpdata, len, 0); //maximum 4096 bytes
	if (tmpLen<=0)
		return -1;
	memcpy(p,&tmpdata,tmpLen);
	return tmpLen;
}

