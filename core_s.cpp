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
#include "bouncing.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static Cservers *proxies;

Ccore::Ccore()
{ //Constructor
	proxies = new Cservers();
	data_g = new Cini();
	fnc = new Cfns(); //for use diverse functions.
	_log = new Clog(data_g,fnc);
	for (int t=0;t<SERVER_CONNECTIONS;t++) //free connections
	{
		cnts[t].busy=0;
		cnts[t].proxy=0;
	}
}
Ccore::~Ccore()
{ //destructor
	
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

int Ccore::start_instance(SOCKET d,BOOL asproxy, char *ip, int auth)
{
	//Commenging a new server
	SOCKET temp=d;
	int z=findcntfree(); //inmediatly find for free connection and busy
	Cproto protocol(data_g);
	if (z>=0) //if we have slots-.
	{
		cnts[z].busy=1; //inmediatly busy this connection
		cnts[z].socket=temp; //Assing socket to struct
		cnts[z].m_mem.release_mem();

		for (int i=0; i<FILE_SLOTS;i++) cnts[z].files[i]=0; //Release file handlers

		strncpy(cnts[z].ip_from,ip,256); //Assing IP addr from
		time(&cnts[z].since); //assing time of connection
		BOOL prx=1; // start proxy as TRUE

   		if (data_g->log_data) _log->logg(cnts[z].ip_from,"CORE ","Received connection.","none");

		prx=asproxy; //if proxy do more rounds
		if(auth==3)
		{
			Cbouncing bnc;
			if(data_g->allow_upbounce)  //Remote bounce accept.
			{
				if (data_g->log_data) _log->logg(cnts[z].ip_from,"CORE ","URCSBP Protocol Activated","none");
				bnc.proxy_accept(cnts[z].socket);
				bnc.proxy_manager(cnts[z].socket);
				return 0;
			}
			else
			{
				bnc.proxy_deny(cnts[z].socket);
				return 0;
			}
		}

		if(auth==2 && data_g->allow_uplink) // Remote command excecution
		{
			//List modules and load.
			HINSTANCE hisdz[MAX_MODULES];
			char bsqz[MAX_PATH+20];
			strncpy(bsqz,data_g->server_modules_directory,MAX_PATH);
			strncat(bsqz,"\\urcs*.dll",sizeof(bsqz)-strlen(bsqz));
		
			WIN32_FIND_DATA ffdz; 
			HANDLE shz = FindFirstFile(bsqz, &ffdz);
			for (int kz=0;kz<MAX_MODULES;kz++) hisdz[kz]=0;
			kz=0;
			if(INVALID_HANDLE_VALUE != shz)
			{
				do 
				{
					char pathfxz[2*MAX_PATH];
					_snprintf(pathfxz,sizeof(pathfxz),"%s\\%s",data_g->server_modules_directory,ffdz.cFileName);
					hisdz[kz]=LoadLibrary(pathfxz);
					if (hisdz[kz]!=NULL) kz++;
				} while (FindNextFile(shz, &ffdz) && kz!=MAX_MODULES);
				FindClose(shz);
			}

			
			//uplink commands
			protocol.continue_socket(temp,"00000000000000000000000000000000");
			strcpy(cnts[z].c_User,"support");
			char rcmd[4096]="";
			recv2(temp,rcmd,4096,0);
			Cintep intep; //make new interpreter
			intep.start_intep(cnts,z,protocol.getkey(),&protocol,fnc,data_g,_log);
			if (rcmd[0]=='R') 
			{
				_snprintf(cnts[z].cmdline,COMMAND_LINE,"PRX_UPLINK -r %s",rcmd+1);
				intep.run(cnts,proxies,z,hisdz); //run command line
			}
			_snprintf(cnts[z].cmdline,COMMAND_LINE,"%s",rcmd+1);
			fnc->filterstring(cnts[z].cmdline); //filter string for commands
			strncpy(cnts[z].cmdline,fnc->convert_vars(cnts[z].cmdline,cnts,z),COMMAND_LINE);

			intep.run(cnts,proxies,z,hisdz); //run command line

			//unload modules.
			int iz=0;	while (hisdz[iz]!=NULL)		FreeLibrary(hisdz[i++]);
			return 0;
		}
		else if (auth==2) return 0;

		bool cntw=FALSE; // Es validada la conexión?
		if (asproxy) protocol.continue_socket(temp,"00000000000000000000000000000000"); //Conexión saliente hacia un proxy
		else cntw=protocol.start_socket(temp,TRUE); //Conexión entrante de un proxy o un cliente

		if (protocol.isProxyIn() && cntw) 
		{
			// Es una conexión valida de un proxy entrante.
			// Se deben enviar Keep-Alives mientras este proxy exista y no este ocupado.
			///////////////////////////////////////
			//	     Setup Proxy Operation		 //
			///////////////////////////////////////
			int gd=proxies->findpipefree();
			proxies->setBusy(gd,true);
			
			char sname[256];
			protocol.getServerName(sname,256);
			proxies->setNameServer(gd,sname);
			proxies->setprx_logdata(gd,protocol.get_prx_logdata());
			proxies->setprx_upbounce(gd,protocol.get_prx_upbounce());
			proxies->setprx_uplink(gd,protocol.get_prx_uplink());
			proxies->setprx_upuser(gd,protocol.get_prx_upuser());

			proxies->setIp(gd,cnts[z].ip_from);
			proxies->setSocket(gd,cnts[z].socket);
			cnts[z].busy=0;
			char prxn[256];	proxies->getNameServer(gd,prxn,sizeof(prxn));
			if (data_g->log_data) _log->logg(proxies->lgetIp(gd),"CORE ","Proxy attached.",prxn);
			///////////////////////////////////////
			//	       Keep-Alive Operation		 //
			///////////////////////////////////////
			while (proxies->isBusy(gd))
			{
				if ( !proxies->isClientBusy(gd) )
				{
					char ka[80]="PING";
					proxies->setBlocked(gd,true);
					proxies->addSentBytes(gd,(unsigned long)send(proxies->getSocket(gd),ka,80,0));
					proxies->addRecvBytes(gd,(unsigned long)recv2(proxies->getSocket(gd),ka,80,0));
					proxies->setBlocked(gd,false);
					if (strcmp(ka,"PONG"))
					{
						proxies->freeSlot(gd); //Server Disconnected.
						if (data_g->log_data) _log->logg(proxies->lgetIp(gd),"CORE ","Proxy detached (No PING responce).",prxn);
					}
				}
				Sleep(35000);	//Ping's the server every 35 seconds.
			}
			return -1;
		}
		else if( protocol.isAuthIn() && cntw )
		{
			if (data_g->allow_be_upuser_server)	protocol.start_remote_auth_server();
			else protocol.start_remote_auth_server_false();
			return -21;
		}
		else if( cntw || asproxy ) // Conexión validada como valida (cliente/proxy entrante) o como proxy saliente.
		{
			//Connection started... we can send any data-. (sending banner)
			protocol.cls();

			protocol.drawRectangle(6,5,70,7,2,LIGHTGRAY,DARKGRAY);
			protocol.setbackground(DARKGRAY);
			protocol.setcolor(15);
			protocol.senddatacenter(BANNER,7);
			protocol.senddata("\n");
			
			if (data_g->log_data)
			{
				protocol.setcolor(RED);
				protocol.senddatacenter("Warning: This server logs your activities",8);
				protocol.setdefaultcolor();
				protocol.senddatacenter(data_g->server_banner,9);
			}
			else
			{
				protocol.setdefaultcolor();
				protocol.senddatacenter(data_g->server_banner,8);
			}
			int level; //privilege levels
			if (data_g->installed) level=0; //privilege level on logon
			else level=4; //privilege level on instalation

			protocol.setbackground(0);


			char urcslogin[512]="URCS Login";
			_snprintf(urcslogin,sizeof(urcslogin),"URCS Login @ %s",data_g->server_name);
			protocol.settitle(urcslogin);

			BOOL cont=1; //continue while not exit
			char germen[30];
			while(cont)
			{
				if(asproxy)
					int o=0;
				if (level==4)
				{
					//Logged on.
					Cintep intep; //make new interpreter
					intep.start_intep(cnts,z,protocol.getkey(),&protocol,fnc,data_g,_log); //start interpreter
					int misc=intep.firstinstall(cnts,z); //install program
					data_g->LoadData(); //load ini again
					if (data_g->installed) level=0; //privilege level on logon
					else if (misc==1) level=3; //Misconfigured shell
					else if (misc==2) {closesocket(cnts[z].socket); cont=0;} //Bye
					else level=4; //privilege level on instalation
				}
				if (level==3) 
				{
					char urcslg[512]="logged on.";
					if (strlen(cnts[z].c_User)+strlen(data_g->server_name)+strlen(cnts[z].ip_from)+8<512)
						_snprintf(urcslg,sizeof(urcslg),"%s@%s from: %s",cnts[z].c_User,data_g->server_name,cnts[z].ip_from);
					protocol.settitle(urcslg);
					strncpy(cnts[z].title,urcslg,512);

					//Logged on.
					if (data_g->log_data) _log->logg(cnts[z].ip_from,"CORE ","Logged on.",cnts[z].c_User);
					protocol.cls();
					// print banner
                   
					//protocol.setbackground(DARKGRAY);
					protocol.setcolor(LIGHTGREEN);
					protocol.sendline(SHELLBANNER1);
					protocol.sendline(SHELLBANNER2);
					protocol.sendline(SHELLBANNER3);
					protocol.sendline(SHELLBANNER4);
					protocol.setbackground(0);
					protocol.setdefaultcolor();

					//Load Libraries/modules here.
					HINSTANCE hisd[MAX_MODULES];

					//Modules information:
					
					//List modules and load.
					char bsq[MAX_PATH+20];
					strncpy(bsq,data_g->server_modules_directory,MAX_PATH);
					strncat(bsq,"\\urcs*.dll",sizeof(bsq)-strlen(bsq));
					
					WIN32_FIND_DATA ffd; 
					HANDLE sh = FindFirstFile(bsq, &ffd);
					for (int k=0;k<MAX_MODULES;k++) hisd[k]=0;
					k=0;
					if(INVALID_HANDLE_VALUE != sh)
					{
						protocol.sendline("Loading Modules:");
						do 
						{
							protocol.senddata(ffd.cFileName);
							char pathfx[2*MAX_PATH];
							_snprintf(pathfx,sizeof(pathfx),"%s\\%s",data_g->server_modules_directory,ffd.cFileName);
							hisd[k]=LoadLibrary(pathfx);
							if (hisd[k]==NULL) protocol.senddata(" - Could not load this module.");
							else
							{
								typedef int (*GTI)(char *info, int xm);
								typedef int (*GETVERSION)( void );
								
								GETVERSION getversion = (GETVERSION)GetProcAddress(hisd[k],"version");
								GTI gti = (GTI)GetProcAddress(hisd[k],"getmodinfo");
								if (gti!=NULL && getversion!=NULL)
								{
									typedef int (*STINTEP)(con_v mx[SERVER_CONNECTIONS], int local_user, char *newkey);
									STINTEP sintep = (STINTEP)GetProcAddress(hisd[k],"start_intep");
									sintep(cnts,z,protocol.getkey());

									char vr[COMMAND_LINE];
									char info[256];
									gti(info,256);
									_snprintf(vr,40," - Version %02d - ",getversion());
									strncat(vr,info,COMMAND_LINE-40);
									protocol.senddata(vr);
								}
								else protocol.senddata(" - FF - There isn't an URCS Module");

								k++;
							}
							protocol.senddata("\n");
						} while (FindNextFile(sh, &ffd) && k!=MAX_MODULES);
						FindClose(sh);
					}

					Cintep intep; //make new interpreter
					intep.start_intep(cnts,z,protocol.getkey(),&protocol,fnc,data_g,_log); //start interpreter
					// print command line take parameter and process them
                    int b=0;
					while (b>=0)
					{
						//print prompt
						_chdir(cnts[z].localdir); // change to dir of user
						intep.printprompt(cnts,z); //print prompt
						b=protocol.getdnline(cnts[z].cmdline,COMMAND_LINE); //get user command line
						int fs=fnc->filterstring(cnts[z].cmdline); //filter string for commands
						strncpy(cnts[z].cmdline,fnc->convert_vars(cnts[z].cmdline,cnts,z),COMMAND_LINE);
						if (b>=0)
						{
							b=intep.run(cnts,proxies,z,hisd); //run command line
							if (!intep.passed) protocol.senddata("\nCommand line error\n"); //when command cannot be excecuted show this
						}
					}
					//Liberar las librerias dinamicas:
					int i=0;
					while (hisd[i]!=NULL)
						FreeLibrary(hisd[i++]);
					if (b==-15) return -15;
					cont=0;
				}
				if (level==2) 
				{
					//compare data with database.
					if ((cnts[z].user_range=data_g->ValidateUser2(cnts[z].c_User,cnts[z].c_Pass,germen))>=0) 
					{ 
						//Validated with local database
						cnts[z].remote=false;
						protocol.setposxy(1,18);
						protocol.delline();
						protocol.setposxy(1,24);
						protocol.senddata("logged on.\n");
                        level++;			
					}
					else
					{
						// Validar usuario en servidor remoto.
						if (data_g->allow_upusers && (cnts[z].user_range=protocol.start_remote_auth_client(cnts[z].c_User,cnts[z].c_Pass,germen))>=0) 
						{
							// Valido con la base de datos remota.
							cnts[z].remote=true;
							protocol.setposxy(1,18);
							protocol.delline();
							protocol.setposxy(1,24);
							protocol.senddata("logged on (Remote Auth).\n");
					        level++;			
						}
						else
						{
							protocol.setposxy(1,18);
							protocol.delline();
							protocol.setposxy(1,24);
							protocol.senddata("Bad username or password.");
							level=0;
						}
					}
				}
				if (level==1) 
				{
					//request for password:
					protocol.setposxy(1,18);
					protocol.delline();
					protocol.setposxy(1,24);
					protocol.senddata("password: ");
					protocol.setcolor(15);
				//	if(protocol.getdnpass(cnts[z].c_Pass,512)<0)
				//		cont=0;
					if(protocol.comparehash(cnts[z].c_Pass,germen)<0)
						cont=0;
					protocol.setcolor(7);
					fnc->denter(cnts[z].c_Pass);
                    level++;
				}
				if (level==0) 
				{
					//request for user:
					protocol.setposxy(1,18);
					protocol.delline();
					protocol.setposxy(1,24);
					protocol.senddata("username: "); //request
					protocol.setcolor(15);
					if(protocol.getdnline(cnts[z].c_User,512)<0) cont=0; //request line
					protocol.setcolor(7);
					fnc->denter(cnts[z].c_User); //filter
					if(!strcmp(cnts[z].c_User,"exit")) cont=0; //command for terminate
                    level++;
				}
			}
			if(asproxy) 				//end of while... 
				protocol.sendclose();
		}
		if (data_g->log_data) _log->logg(cnts[z].ip_from,"CORE ","Closed Connection.",cnts[z].c_User);
		cnts[z].m_mem.release_mem();
		cnts[z].busy=0;
	}
	else
		protocol.deny_connection(temp);			//Cannot accept connection 
	return 0;
}
