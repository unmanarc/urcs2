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
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsock2.h"
#include "ini_man.h"
#include "logger.h"
#include "servers.h"

class Ccore
{
public:
	Ccore(); 
	//Core functions
	int start_instance(SOCKET d, BOOL asproxy, char *ip, int auth); //Start new instance for server

	// Functions to manage Connections.
	int findcntfree();
	int findpipefree();

	Cini * data_g; // Globals data. (initialized)

	con_v cnts[SERVER_CONNECTIONS]; //Create structure for connections (users)
	//ers_svr * svrs[SERVER_SLOTS];  //Create structure for servers attached.

	~Ccore();

protected:
	Clog * _log;
	Cfns * fnc; //for use diverse functions.

	//functions to 

};
