// ctool2.cpp : Defines the entry point for the console application.
//
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
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "winsock2.h"
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>


SOCKET ack=0;
#define SERVER_PORT 1980

void debg(char *crecpt)
{
	int ste=strlen(crecpt);
	while (ste>=0)
	{
		if (crecpt[ste]=='\\' || crecpt[ste]=='%')
			crecpt[ste]=0;
		ste--;
	}
}


UINT startserver(LPVOID pParam)
{
	SOCKET s=ack;
	int y=1;
	char command[30];
	int cmd=0;

	//Receive command (int):
		//1: receive file
		//2: upload file
	y=recv(s,command,30,0);
	if (y<=0)
		return 0; //Disconnection or TCP fail
	if (!strcmp(command,"put"))
	{ 
		//put file into client (receive)
		char fln[512];
		memset(&fln,0,512);
		if (recv(s,fln,512,0)<=0)
			return 0; //Disconnection or TCP fail
		debg(fln); //evites ../ bug
		printf("The server invoked upload command:\nFilename: %s\nReceiving...\n",fln);
		int fh;
		if( (fh = _open( fln, _O_WRONLY | _O_CREAT | _O_BINARY )) != -1 )
		{
			unsigned int iRead;
			char byt[4096];
			do
			{		  
				iRead = recv(s,byt,4096,0);
				_write(fh,&byt,iRead);
			} 
			while (iRead > 0);
		    _close( fh );
		}
		printf("\nClosed Command... Received.+\n");
	}
	if (!strcmp(command,"get"))
	{ 
		char fln[512];
		memset(&fln,0,512);
		if (recv(s,fln,512,0)<=0)
			return 0; //Disconnection or TCP fail
		debg(fln); //evites ../ bug
		printf("The server invoked download command:\nFilename: %s\n",fln);
           char buffer[4096];
		int fh;
		unsigned int nbytes = 4096, bytesread;
		if( (fh = _open(fln , _O_RDONLY | _O_BINARY )) == -1 )
		{
			send(s,"-",5,0);
			printf("File Not Founded\n");
		}
		else
		{
			printf("Sending...");
			send(s,"+",5,0);
			while( ( bytesread = _read( fh, buffer, nbytes ) ) > 0 )
				send(s,buffer,bytesread,0);
			_close( fh );
		}
		printf("\nClosed Command... Sended.+\n");
	}

	return 0;
}

int server()
{
	int ServerSize=sizeof(sockaddr_in);
	int  cliLen;
	struct sockaddr_in cliAddr, servAddr;
	
	WORD wVersionRequested;
	WSADATA	wsaData;
	SOCKET s, newSd;

	// ------------------------- Establecer variables
	
	wVersionRequested = MAKEWORD( 1,1);
	if (WSAStartup(wVersionRequested, &wsaData)!=0) 
	{
		return FALSE;
	}

	s=socket(AF_INET, SOCK_STREAM, 0);
	if (s==INVALID_SOCKET)
		return 0;

	//protocol
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERVER_PORT);

	if(bind(s, (struct sockaddr *) &servAddr, sizeof(servAddr))<0)
		return 1; //error! can't bind port

	listen(s,5); 
	while(1)
	{ 
		cliLen = sizeof(cliAddr);
		printf("listening on port 1980...\n");
		newSd = accept(s, (struct sockaddr *) &cliAddr, &cliLen);
		if(newSd<0) 
			return 2; //Cannot accept
		ack=newSd;
		//CWinThread* pThread = AfxBeginThread(startserver, GetSafeHwnd(), THREAD_PRIORITY_NORMAL);
		printf("Connected in: %s:1980\n",inet_ntoa(cliAddr.sin_addr));
		startserver(NULL);
		closesocket(newSd);
		printf("Closed connection to %s\n\n----------------\n",inet_ntoa(cliAddr.sin_addr));
	}
	closesocket(s);
	return -1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	printf("UNTT2 client tool console. 1.0\n");
	int q;
	while(1)
	{
		q=server(); // Create an server
		if (q==0)
			printf("can't initialize socket\n");
		if (q==1)
			printf("error! can't bind port\n");
		if (q==2)
			printf("Cannot accept\n");
		if (q==3)
			printf("Cannot receive data\n");
		if (q==4)
			printf("Closed!\n");
	}
	return 0;
}

