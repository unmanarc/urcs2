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
#include "core_s.h"
#include "protocol.h"
#include "funcs.h"
#include "intep.h"
#include <direct.h>
#include "proxy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


Ccore::Ccore()
{ //Constructor
	for (int fnd=0; fnd<SERVER_SLOTS;fnd++) //free server slots
	{
		svrs[fnd].busy=FALSE;
		svrs[fnd].cbsy=FALSE;
		svrs[fnd].f=0;
		strcpy(svrs[fnd].ip,"0.0.0.0");
		strcpy(svrs[fnd].nameserver,"none");
	}
	for (int t=0;t<SERVER_CONNECTIONS;t++) //free connections
	{
		cnts[t].busy=0;
		cnts[t].proxy=0;
	}

}
Ccore::~Ccore()
{ //destructor
	
}

int Ccore::findpipefree()
{
	Cproxy prx;
	for (int fnd=0; fnd<SERVER_SLOTS;fnd++)
	{
		//releasing disconnected proxys
		if (svrs[fnd].busy && !svrs[fnd].cbsy)
		{
			if (!prx.pingproxy(svrs[fnd].f))
			{ //release
				svrs[fnd].busy=0;
				svrs[fnd].cbsy=0;
				closesocket(svrs[fnd].f);
			}
		}
	}
	for (int fnd=0; fnd<SERVER_SLOTS;fnd++)
		if (!svrs[fnd].busy) return fnd;
	return -1;
}

int Ccore::findcntfree()
{
	int fnd=0;
	while (fnd<SERVER_CONNECTIONS)
	{
		if (cnts[fnd].busy==FALSE)
			return fnd;
		fnd++;
	}
	return -1;
}

int Ccore::start_instance(SOCKET d,BOOL asproxy, char *ip)
{
	//Commenging a new server
	SOCKET temp=d;
	int z=findcntfree(); //inmediatly find for free connection and busy
	Cproto protocol;
	if (z>=0) //if we have slots-.
	{
		Cfns fnc; //for use diverse functions.
		cnts[z].busy=1; //inmediatly busy this connection
		cnts[z].socket=temp; //Assing socket to struct
		cnts[z].m_mem.release_mem();
		for (int i=0; i<FILE_SLOTS;i++) cnts[z].files[i]=0; //Release file handlers

		strcpy(cnts[z].ip_from,ip); //Assing IP addr from
		time(&cnts[z].since); //assing time of connection
		BOOL prx=1; // start proxy as TRUE
		
		prx=asproxy; //if proxy do more rounds
		BOOL cntw=FALSE;
		if (asproxy) 
			protocol.continue_socket(temp,"00000000000000000000000000000000");
		else
			cntw=protocol.start_socket(temp,TRUE);
		if(cntw || asproxy) // Start connection with client?
		{
			//review if proxy
			if (asproxy)
				while(protocol.getproxyline()<0); //wait for proxyline
			if (protocol.prx) //is proxy...
			{
				int gd=findpipefree(); //find for free proxy...
				svrs[gd].busy=1;
				strncpy(svrs[gd].nameserver,protocol.nameserver,512); //assing nameserver
				strcpy(svrs[gd].ip,cnts[z].ip_from);
				svrs[gd].f=cnts[z].socket;
				cnts[z].busy=0;
				return -1;
			}
				//Connection started... we can send any data-. (sending banner)
			protocol.senddata("URCS - Unmanarc Remote Control Server 1.0.3 build 3 - MDP\n");
			protocol.senddata(data_g.server_banner);
			protocol.senddata("\n");
			int level; //privilege levels
			if (data_g.installed) level=0; //privilege level on logon
			else level=4; //privilege level on instalation
			
			BOOL cont=1; //continue while not exit
			while(cont)
			{
				if(asproxy)
					int o=0;
				if (level==4)
				{
					//Logged on.
					Cintep intep; //make new interpreter
					intep.start_intep(cnts,z,protocol.getkey()); //start interpreter
					intep.firstinstall(cnts,z); //install program
					data_g.LoadData(); //load ini again
					if (data_g.installed) level=0; //privilege level on logon
					else level=4; //privilege level on instalation
				}
				if (level==3) 
				{
					//Logged on.
					protocol.cls();
					Cintep intep; //make new interpreter
					intep.start_intep(cnts,z,protocol.getkey());
                       // print command line take parameter and process them
                    int b=0;
					while (b>=0)
					{
						//print prompt
						_chdir(cnts[z].localdir); // change to dir of user
						intep.printprompt(cnts,z); //print prompt
						b=protocol.getdnline(cnts[z].cmdline,COMMAND_LINE); //get user command line
						fnc.filterstring(cnts[z].cmdline); //filter string for commands
						strncpy(cnts[z].cmdline,fnc.convert_vars(cnts[z].cmdline,cnts,z),COMMAND_LINE);
						if (b>=0) b=intep.run(cnts,svrs,z); //run command line
						if (!intep.passed) protocol.senddata("\nCommand line error\n"); //when command cannot be excecuted show this
					}
					cont=0;
				}
				if (level==2) 
				{
					//compare data with database.
					if (data_g.ValidateUser(cnts[z].c_User,cnts[z].c_Pass)>=0) 
					{ //Validated with database
						protocol.senddata("\nlogged on.\n");
						cnts[z].user_range=data_g.ValidateUser(cnts[z].c_User,cnts[z].c_Pass);
                        level++;			
					}
					else
					{
						protocol.senddata("\nBad username or password.\n");
						level=0;
					}
				}
				if (level==1) 
				{
					//request for password:
					protocol.senddata("password:");
					if(protocol.getdnpass(cnts[z].c_Pass,512)<0)
						cont=0;
					fnc.denter(cnts[z].c_Pass);
                    level++;
				}
				if (level==0) 
				{
					//request for user:
					protocol.senddata("username:"); //request
					if(protocol.getdnline(cnts[z].c_User,512)<0) cont=0; //request line
					fnc.denter(cnts[z].c_User); //filter
					if(!strcmp(cnts[z].c_User,"exit")) cont=0; //command for terminate
                    level++;
				}
			}
			if(asproxy) 				//end of while... 
				protocol.sendclose();
		}
		cnts[z].m_mem.release_mem();
		cnts[z].busy=0;
	}
	else
		protocol.deny_connection(temp);			//Cannot accept connection 
	return 0;
}
