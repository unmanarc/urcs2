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
Cproxy::Cproxy(Cini *inim)
{ //Constructor
	ini=inim;
}
Cproxy::~Cproxy()
{ //destructor
	
}

SOCKET Cproxy::connectto(int prx)
{
//	Cini inim;
//	ini->LoadData();
	
	s_proxyinf ft=ini->GetProxyByNumber(prx);

	struct hostent *hentry;
	struct sockaddr_in Server_add;
//	WORD wVersionRequested;
//	WSADATA wsaData;
	// Start Winsock
//	wVersionRequested = MAKEWORD( 1, 0 );
//	if( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	//{
		SOCKET Sock = socket( AF_INET, SOCK_STREAM, 0 ); //open socket
		// Connect Server
		if ((hentry = gethostbyname(ft.proxy)) == NULL)
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
			Server_add.sin_port = htons(ft.port);
			Server_add.sin_family = AF_INET;

			// Connect Server
			if( connect( Sock, (struct sockaddr *) &Server_add, sizeof( Server_add ) ) == 0 )
			{
				//do negot with server.
				char init_d[4];
				strcpy(init_d,"ipx");
				send(Sock,init_d,3,0);
				// Compliant with URCS 2.1 protocol:
				/*
						- 1 Auth byte:
						101: be proxy in:
							- 1 byte with:
								0000:000X : Allow remote commands
								0000:00X0 : Allow Up-users
								0000:0X00 : Allow Upbounce
								0000:X000 : Log data
							- 256 bytes of server name
				*/
				char proxy_info[300];
				proxy_info[0]=(char)101;
				proxy_info[1]=0;
				if (ini->allow_uplink) proxy_info[1]+=0x1;
				if (ini->allow_upusers) proxy_info[1]+=0x2;
				if (ini->allow_upbounce) proxy_info[1]+=0x4;
				if (ini->log_data) proxy_info[1]+=0x8;
				strncpy(proxy_info+2,ini->server_name,256);
   				send(Sock,proxy_info,258,0);

				// A Server has my data... now, all information that i'll receive is a prx
				// Start server with proxy mode (non close admited)

				return Sock;
			}
			else
			{
				return 0;
			}
		}
	//}
//	else
//		return 0;
}

SOCKET Cproxy::connecttoproxy()
{
//	Cini inim;
//	ini->LoadData();
	s_proxyinf localproxy; 
	localproxy=ini->GetProxy(); //get windows proxy data.
	if (!localproxy.have_proxy) return 0; //not proxy defined.
	struct hostent *hentry;
	struct sockaddr_in Server_add;
	//WORD wVersionRequested;
	//WSADATA wsaData;
	// Start Winsock
	//wVersionRequested = MAKEWORD( 1, 0 );
	//if( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	//{
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
				_snprintf(negot,sizeof(negot),"CONNECT %s:%u HTTP/1.0\n",ini->mother_name,ini->port);
				send(Sock,negot,(int)strlen(negot),0);//negotiate with proxy... 
				//do negot with server.
				char init_d[4];
				strcpy(init_d,"ipx");
				send(Sock,init_d,3,0);
				send(Sock,ini->server_name,512,0);
				// A Server has my data... now, all information that i'll receive is a prx
				// Start server with proxy mode (non close admited)
				return Sock;
			}
			else
			{
				return 0;
			}
		}
	//}
	//else
//		return 0;
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
		
		if (GetBlock(&l,4,y)<=0) 
			return -3; //Server Closes.
		if (l>4096) //is command... 
		{
				//transmit command and do..
			char buftsnd[4096];
			memcpy(buftsnd,&l,4); //copy command
			if (send(x,buftsnd,4,0)<=0) 
				return -2; //error client closes connection.

			if (l==4099) //server exit command
				iscon=0;
			if (l==5002) //server to client ftransfer
			{
				char fnd[512];
				char intep[1];
				ftrs ft;
				if (recv2(y,fnd,512,0)<=0) 
					return -3;
				if (send(x,fnd,512,0)<=0) 
					return -2;
				if (recv2(y,intep,1,0)<=0) 
					return -3;
				if (send(x,intep,1,0)<=0) 
					return -2;
				if (intep[0]==1)
				{
					ft.bt=1;
					while(ft.bt>0)
					{
						if (recv2(x,(char *)&ft,sizeof(ft),0)<=0) 
							return -2;
						if (send(y,(char *)&ft,sizeof(ft),0)<=0) 
							return -3;
					}
				}

			}
			if (l==5001) //server to client ftransfer
			{
				char fnd[512];
				char intep[1];
				ftrs ft;
				if (recv2(y,fnd,512,0)<=0) 
					return -3;
				if (send(x,fnd,512,0)<=0) 
					return -2;
				if (recv2(x,intep,1,0)<=0) 
					return -2;
				if (send(y,intep,1,0)<=0) 
					return -3;
				if (intep[0]==0) break; //end's connection...
				ft.bt=1;
				while (ft.bt>0)
				{
					if (recv2(y,(char *)&ft,sizeof(ft),0)<=0) 
						return -3;
					if (send(x,(char *)&ft,sizeof(ft),0)<=0) 
						return -2;
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
				if (l2<0) 
					return -2; //client closes connection

				//reorder received data in buf3
				memcpy(buf3,&l2,4);
                strncpy(buf3+4,buf2,sizeof(buf3)-4);

				//send to server...
				if(send(y,buf3,l2+4,0)<=0) 
					return -3; //server closes connection
			}

			if (l==4100) //command for passwords
			{ //need to receive data from client and send to server...
                int l2=1;
				
				char buf2[4096];
				char buf3[4096];

				//Send Germen to Client.
				l=1;
				GetBlock(&l,4,y); //get 1 block lenght from server (germen)
				l=GetBlock(&buf,l,y); //get 1 block data from server (germen)
				if (l>=0)  //not corrupted
				{
					//send data to client
					char buftsnd[4096];
					memset(&buftsnd,0,4096);
					memcpy(buftsnd,&l,4);
					strncpy(buftsnd+4,buf,sizeof(buftsnd));
					if (send(x,buftsnd,l+4,0)<=0) 
						return -2; //error client closes connection.
				}
				else 
					return -3; //server closes


				memset(&buf2,0,4096);
				memset(&buf3,0,4096);
				GetBlock(&l2,4,x); //get 1 block lenght
				l2=GetBlock(&buf2,l2,x); //get 1 block data from client
				if (l2<0) 
					return -2; //client closes connection

				//reorder received data in buf3
				memcpy(buf3,&l2,4);
                strncpy(buf3+4,buf2,sizeof(buf3)-4);

				//send to server...
				if(send(y,buf3,l2+4,0)<=0) 
					return -3; //server closes connection
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
				memcpy(buftsnd,&l,4);
				strncpy(buftsnd+4,buf,sizeof(buftsnd)-4);
				if (send(x,buftsnd,l+4,0)<=0) 
					return -2; //error client closes connection.
			}
			else 
				return -3; //server closes
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
int Cproxy::attach(char *ip, unsigned int port, SOCKET client)
{
	x=client;
//	Cini inim;
//	ini->LoadData();

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
			return 0;
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
				//do negot with server.
//				char key[32];
				char buff[4096]; //buffer managment
				int i; //error managment
				memset(&buff,0,4096); //start buff
				i=send(Sock,"iux",3,0); //Send initialization command.
				if (i<=0) return -1; //if socket error, return -1 (socket error)

//				i=recv2(Sock,buff,32,0); //receive 32bytes data of key.
//				if(i!=32) return -2; //if not receive 32 bytes of protocol, return error -2 (incompatible protocol or socket error)
//				else strncpy(key,buff,32); //if well, set 128bit key
//				if (!strcmp(key,"--------------------------------")) return -3;//Server full
				y=Sock;
				return transfer();
			}
			else
			{
				return -4;
			}
		}
	//}
	//else
//		return 0;
}

//protected area:
/*int Cproxy::GetBlock(void *p, int len, SOCKET m)
{
	char tmpdata[4096];
	int tmpLen = recv2(m, tmpdata, len, 0); //maximum 4096 bytes
	if (tmpLen<=0) return -1;
	memcpy(p,tmpdata,tmpLen);
	return tmpLen;
}*/
int Cproxy::GetBlock(void *p, int len, SOCKET m)
{
	int tmplen=0;
	if ( (tmplen=recv2(m,(char *)p,len,0)) <= 0 ) return -1;
	else return tmplen;
}

