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
#include "protocol.h"

/*
Unmanarc implementation of client managment...
Special thanks to: 

TheCPUWizard: for the idea of algorithm to transmit data with nagle implementing a new header.
Bistok: for OOP help
DanM: for help about socket's functions / hostnames
*/

BOOL bye=0;
Ccore newcore; //Create global core object for managment of connection
char ipl[256];

typedef struct THREADSTRUCT		//structure for passing to the controlling function
{
	SOCKET n;
	int auth;
} THREADSTRUCT;

typedef struct SOCKETPARSER		//structure for passing to the controlling function
{
	SOCKET n;
} SOCKETPARSER;
typedef struct INTPARSER		//structure for passing to the controlling function
{
	int n;
} INTPARSER;
UINT newprxinstance(LPVOID pParam)
{
	THREADSTRUCT*	ts = (THREADSTRUCT*)pParam;
	char ipm[256];
	strncpy(ipm,ipl,sizeof(ipm));
	int i=newcore.start_instance(ts->n,1,"255.255.255.255",ts->auth); 
	if (i==0) closesocket(ts->n); //When finish close the connection
	else if (i==-15) //exit signal
	{
		bye=1;
		Cconnector cn;
		cn.connectto("127.0.0.1",newcore.data_g->port);
	}
	delete ts;
	return 0; // Process finish, return 0
}
UINT eprx(LPVOID pParam)  // EXTERNAL PROXY.
{
	INTPARSER*	ts = (INTPARSER*)pParam;
	//Connect to the proxy.
	Cproxy mpr(newcore.data_g);
	while (1) //infinite loop for connect to server
	{
		Sleep(1000); //Wait 1 second 
		int i=-1; //indicator.
		SOCKET sk=mpr.connectto(ts->n); //Connect to socket and prepair it for instance's
		if(sk==0)
			Sleep(60000); //wait 1 minute for reconnect if don't success.
		else 
		{
			Cproto prot(newcore.data_g);
			prot.continue_socket(sk, "");
			int rew=-2;
			while (rew==-2)
				rew=prot.getproxyline();
			if (rew>=0)
			{
				//when receive connection, parse to core and start new listenner.
				THREADSTRUCT *_param = new THREADSTRUCT;
				_param->n = sk;
				_param->auth = rew;
				AfxBeginThread (newprxinstance, _param); //Start Another thread for my client... and go to background.
			}
		}
	}
	delete ts;
	return 0;
}
UINT newinstance(LPVOID pParam)
{
	SOCKETPARSER*	ts = (SOCKETPARSER*)pParam;
	char ipm[256];
	strncpy(ipm,ipl,sizeof(ipm));
	int i=newcore.start_instance(ts->n,0,ipm,1); // Start the process to manage connection
	if (i==0) closesocket(ts->n); //When finish close the connection
	if (i==-15) //exit signal
	{
		bye=1;
		Cconnector cn;
		cn.connectto("127.0.0.1",newcore.data_g->port);
	}
	delete ts;
	return 0; // Process finish, return 0
}
int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR    lpCmdLine,int       nCmdShow)
{
	char commandline[512];
	strncpy(commandline,lpCmdLine,512);
	strupr(commandline);
	if (strstr(commandline,"DELAY")!=NULL) Sleep(5000); // Wait 5 seconds for start new server
	CUserver server; //Start new server instance
	server.start_winsock(); //Start WinSocket.
	newcore.data_g->LoadData();
	//Get ini
	/* auto-install...	
	//installed...
	// this code is for auto-startup (if you need to prevent for reboot before remote installation)
	// by default is disabled.
	newcore.data_g->PutKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "URCS","rms");
	newcore.data_g->CopyMeToWinDir(); 
	*/
	int we=newcore.data_g->GetNumberOfProxies();
	for (int prx_n=0;prx_n<we;prx_n++) 
	{
		INTPARSER *_paramx = new INTPARSER;
		_paramx->n=prx_n;
		AfxBeginThread(eprx,_paramx); //Start proxy out connection
	}
	if (server.startserver(newcore.data_g->port)>0)
	{
		strcpy(ipl,"");
		while (1)
		{
			SOCKET n=server.s_accept(ipl); //Wait for client connection and parse as global
			if (bye) return 0;
			SOCKETPARSER *_param = new SOCKETPARSER;
			_param->n=n;
			AfxBeginThread(newinstance, _param); //Start Another thread for my client... and go to background.
		}
	}
	else Sleep(INFINITE); //If can't start server, Sleep Infinite for wait for outgoing proxy.
	return 0;
}