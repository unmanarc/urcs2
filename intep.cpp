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
#include "MD5Checksum.h"
#include "intep.h"
#include "mem_man.h"
#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include "Tlhelp32.h"
#include <direct.h>
#include "stdio.h"
#include <io.h>
#include "proxy.h"
#include "file_transfer.h"
#include "malloc.h"
#include "scripting.h"
#include "compiler.h"
#include "Urlmon.h"
#include "bouncing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


Cintep::Cintep()
{ //Constructor
	bye=FALSE;
//	data_g->LoadData();
}
Cintep::~Cintep()
{
}
void Cintep::ExecAndGetOutput( LPCSTR szCommand )
{
   SECURITY_ATTRIBUTES rSA;
   rSA.nLength=              sizeof(SECURITY_ATTRIBUTES);
   rSA.bInheritHandle=       TRUE;
   rSA.lpSecurityDescriptor= 0;

   HANDLE hReadPipe, hWritePipe;
   CreatePipe( &hReadPipe, &hWritePipe, &rSA, 250000 );
  
   PROCESS_INFORMATION rPI;
   STARTUPINFO rSI;
   memset( &rSI, 0, sizeof(STARTUPINFO));
   rSI.cb=          sizeof(STARTUPINFO);
   rSI.dwFlags=     STARTF_USESHOWWINDOW |STARTF_USESTDHANDLES;
   rSI.wShowWindow= SW_HIDE; //SW_MINIMIZE;
   rSI.hStdOutput=  hWritePipe;
   rSI.hStdError=   hWritePipe;

//   CString sCmd; sCmd.Format( "%s", (LPCSTR)szCommand );
   

   BOOL fRet=CreateProcess(NULL,(LPSTR)szCommand, NULL,NULL,TRUE,0,0,0, &rSI, &rPI );
   if ( !fRet ) {
        //return( "" );
	  // nproto->sendline("Error @CreateProcess.fRet:");
	   nproto->sendline("Cannot start this proccess.");

   }
   //------------------------- wait for the program to end
   WaitForSingleObject( rPI.hProcess, INFINITE);

   //------------------------- read the pipe
   char dest[1000];
   while ( ReadFromPipeNoWait( hReadPipe, dest, sizeof(dest) ) > 0 ) 
   {
	   nproto->senddata(dest);
   }

   CloseHandle( hReadPipe  );
   CloseHandle( hWritePipe );
   CloseHandle( rPI.hThread); 
   CloseHandle( rPI.hProcess);
   nproto->sendline("");

}
int Cintep::ReadFromPipeNoWait( HANDLE hPipe, char* pDest, int nMax )
{
   DWORD nBytesRead= 0;
   DWORD nAvailBytes;
   char cTmp;
   memset( pDest, 0, nMax );

   PeekNamedPipe( hPipe, &cTmp, 1, NULL, &nAvailBytes, NULL );
   if ( nAvailBytes == 0 ) {
        return( nBytesRead );
   }
   BOOL fNoErr= ReadFile( hPipe, pDest, nMax-1, &nBytesRead, NULL);

   if ( !fNoErr ) 
   {  // error
        nBytesRead= 0;
//        strError = "Error reading pipe";
   }
   return( nBytesRead );
}
int Cintep::firstinstall(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	
	//////////////////////////////////////////////////////////////////
	////////   Inicializacin de Variables de instalacin     ////////
	//////////////////////////////////////////////////////////////////

	HKEY install_location=HKEY_CURRENT_USER;
	char username[128]="default";
	char password[128]="default";
	char use_default_user_struct[5]="Y";
	char allow_upusers[5]="Y";
	char allow_be_upuser_server[5]="Y";
	char allow_upbounce[5]="Y";
	char allow_uplink[5]="Y";
	char log_data[5]="N";
	char mother_name[2048]="proxy.unmanarc.com:3360,";
	char server_banner[256];
	char server_modules_directory[_MAX_PATH+20]="";
	char server_install_directory[_MAX_PATH]="";
	char server_name[25]="";
	char server_port[10]="3360";
	char server_prompt[128]="[$d $u]";
	char upuser_server[128]="proxy.unmanarc.com:3360";
	
	int install_method=0; 
	// 0: HKEY_CURRENT_USER
	// 1: HKEY_LOCAL_MACHINE
	// 2: Service
	// 3: No startup
	
	_snprintf(server_modules_directory,sizeof(server_modules_directory),"%s\\urcs_modules",data_g->windir);
	_snprintf(server_banner,sizeof(server_banner),"Welcome to %s",data_g->default_domain);
	_snprintf(server_install_directory,sizeof(server_install_directory),"%s",data_g->windir);
	_snprintf(server_name,sizeof(server_name),"%s",data_g->default_domain);
	_snprintf(username,sizeof(username),"%s",data_g->default_username);
	
	bool installed=false,cannot_write_to_reg=false,cannot_copyfile=false;
	while (!installed)
	{
		bool mispass=false;
		bool isconfigured=false;
		while (!isconfigured)
		{
			//////////////////////////////////////////////////////////////////
			////////   Pantalla de configuracin principal            ////////
			//////////////////////////////////////////////////////////////////
			nproto->setbackground(BLUE);
			nproto->drawRectangle(1,1,79,24,1,LIGHTBLUE,BLUE);
		
			nproto->setposxy(5,2);
			nproto->setcolor(WHITE);
			nproto->senddata(INSTALL_URCS_01);
			char line[30];
			memset(&line,0,30);
			memset(&line,196,29);
			nproto->setposxy(5,3);
			nproto->senddata(line);

			nproto->setcolor(LIGHTGRAY);
			nproto->setposxy(10,5);
			nproto->senddata("1. Configure global server variables");
			nproto->setposxy(10,6);
			nproto->senddata("2. Configure server install method");
			nproto->setposxy(10,7);
			nproto->senddata("3. Configure server authentification module");
			nproto->setposxy(10,8);
			nproto->senddata("4. Configure server proxy module");
			
			nproto->setposxy(10,10);
			nproto->senddata(line);
	
			nproto->setposxy(10,12);
			nproto->senddata("5. Save configuration and install");
			nproto->setposxy(10,13);
			nproto->senddata("6. Go to misconfigured Shell");
			nproto->setposxy(10,14);
			nproto->senddata("7. Load Defaults");
			nproto->setposxy(10,15);
			nproto->senddata("8. Leave/Disconnect (no install)");

			if (mispass)
			{
				nproto->setcolor(LIGHTRED);
				nproto->setposxy(2,21);
				nproto->senddata("Error: you must specify user and password (Server Auth Mod.)");
				mispass=false;
				nproto->setcolor(LIGHTGRAY);
			}

			nproto->setposxy(2,23);
			nproto->senddata("Make your choice: ");
			char choice[5];
			nproto->getdnline(choice,5);
			if		(choice[0]=='1') 
			{
				bool ch_1_leave=false;
				while(!ch_1_leave)
				{
					//Control de las variables globales de sistema
					
					//	char log_data[5]="N";
					//	char server_banner[256];
					//	char server_name[25]="";
					//	char server_port[10]="3360";
					//	char server_prompt[128]="[$d $u]";
	
					//CRear un rectangulo (60x20)
	
					nproto->setbackground(DARKGRAY);
					nproto->setcolor(LIGHTGRAY);
					nproto->drawRectangle(14,4,60,19,1,LIGHTGRAY,DARKGRAY);
		
					nproto->setcolor(WHITE);
					nproto->setposxy(16,5);
					nproto->senddata(INSTALL_URCS_02);
					nproto->setposxy(16,6);
					nproto->senddata(line);
					nproto->setposxy(20,7);
					nproto->senddata("1. Log server data: [");
					if (log_data[0]=='Y') nproto->senddata("Y");
					else nproto->senddata("N");
					nproto->senddata("]");
					nproto->setposxy(20,8);
					nproto->senddata("2. Server banner: ");
					nproto->senddata(server_banner);

					nproto->setposxy(20,9);
					nproto->senddata("3. Server name: ");
					nproto->senddata(server_name);

					nproto->setposxy(20,10);
					nproto->senddata("4. Server port: ");
					nproto->senddata(server_port);

					nproto->setposxy(20,11);
					nproto->senddata("5. Server prompt: ");
					nproto->senddata(server_prompt);

					nproto->setposxy(20,12);

					nproto->senddata("6. return");

					nproto->setposxy(15,21);
					nproto->senddata("What do you want to change? ");
					nproto->getdnline(choice,5);
					if (choice[0]=='1') 
					{
						if (log_data[0]=='Y') log_data[0]='N';
						else log_data[0]='Y';
					}
					else if (choice[0]=='2') 
					{
						nproto->setposxy(15,21);
						nproto->senddata("                                                         ");
						nproto->setposxy(15,21);
						nproto->senddata("New Value: ");
						nproto->getdnline(server_banner,sizeof(server_banner));
						fns->denter(server_banner);
					}
					else if (choice[0]=='3') 
					{
						nproto->setposxy(15,21);
						nproto->senddata("                                                         ");
						nproto->setposxy(15,21);
						nproto->senddata("New Value: ");
						nproto->getdnline(server_name,sizeof(server_name));
						fns->denter(server_name);
					}
					else if (choice[0]=='4') 
					{
						nproto->setposxy(15,21);
						nproto->senddata("                                                         ");
						nproto->setposxy(15,21);
						nproto->senddata("New Value: ");
						nproto->getdnline(server_port,sizeof(server_port));
						unsigned short port=atoi(server_port);
						itoa(port,server_port,10);
					}
					else if (choice[0]=='5') 
					{
						nproto->setposxy(15,21);
						nproto->senddata("                                                         ");
						nproto->setposxy(15,21);
						nproto->senddata("New Value: ");
						nproto->getdnline(server_prompt,sizeof(server_prompt));
						fns->denter(server_prompt);
					}
					else if (choice[0]=='6') { ch_1_leave=true; }
				}
			}
			else if (choice[0]=='2') 
			{
				bool ch_2_leave=false;
				while(!ch_2_leave)
				{
					//Control de las variables de instalacion

					// HKEY install_location=HKEY_CURRENT_USER;
					// char server_modules_directory[_MAX_PATH+20]="";
					// char server_install_directory[_MAX_PATH]="";
					//	int install_method=0; 
					// 0: HKEY_CURRENT_USER
					// 1: HKEY_LOCAL_MACHINE
					// 2: Service

					//CRear un rectangulo (60x20)
	
					nproto->setbackground(DARKGRAY);
					nproto->setcolor(LIGHTGRAY);
					nproto->drawRectangle(14,4,60,19,1,LIGHTGRAY,DARKGRAY);
		
					nproto->setcolor(WHITE);
					nproto->setposxy(16,5);
					nproto->senddata(INSTALL_URCS_03);
					nproto->setposxy(16,6);
					nproto->senddata(line);
					nproto->setposxy(20,7);
					nproto->senddata("1. Config at Current User or All Users: ");
					if (install_location==HKEY_CURRENT_USER) nproto->senddata("Current User");
					else if (install_location==HKEY_LOCAL_MACHINE) nproto->senddata("All Users");

					nproto->setposxy(20,8);
					nproto->senddata("2. Installation directory: ");
					nproto->senddata(server_install_directory);

					nproto->setposxy(20,9);
					nproto->senddata("3. Modules Directory: ");
					nproto->senddata(server_modules_directory);

					nproto->setposxy(20,10);
					nproto->senddata("4. Startup Method: ");
					if (install_method==0) nproto->senddata("HKEY_CURRENT_USER");
					else if (install_method==1) nproto->senddata("HKEY_LOCAL_MACHINE");
					else if (install_method==2) nproto->senddata("Service");
					else if (install_method==3) nproto->senddata("No Startup");

					nproto->setposxy(20,11);
					nproto->senddata("5. return");

					nproto->setposxy(15,21);
					nproto->senddata("What do you want to change? ");
					nproto->getdnline(choice,5);
					if (choice[0]=='1') 
					{
						if (install_location==HKEY_CURRENT_USER) install_location=HKEY_LOCAL_MACHINE;
						else install_location=HKEY_CURRENT_USER;
					}
					else if (choice[0]=='2') 
					{
						nproto->setposxy(15,21);
						nproto->senddata("                                                         ");
						nproto->setposxy(15,21);
						nproto->senddata("New Value: ");
						nproto->getdnline(server_install_directory,sizeof(server_install_directory));
						fns->denter(server_install_directory);
					}
					else if (choice[0]=='3') 
					{
						nproto->setposxy(15,21);
						nproto->senddata("                                                         ");
						nproto->setposxy(15,21);
						nproto->senddata("New Value: ");
						nproto->getdnline(server_modules_directory,sizeof(server_modules_directory));
						fns->denter(server_modules_directory);
					}
					else if (choice[0]=='4') 
					{
						if (install_method==0) install_method=1;
						else if (install_method==1) install_method=2;
						else if (install_method==2) install_method=3;
						else if (install_method==3) install_method=0;
					}
					else if (choice[0]=='5') { ch_2_leave=true; }
				}
			
			}
			else if (choice[0]=='3') 
			{
				bool ch_3_leave=false;
				while(!ch_3_leave)
				{
					//Control de las variables de instalacion

			
					//char username[128]="default";
					//char password[128]="default";
					//char use_default_user_struct[5]="Y";
					//char allow_upusers[5]="Y";


					//CRear un rectangulo (60x20)
	
					nproto->setbackground(DARKGRAY);
					nproto->setcolor(LIGHTGRAY);
					nproto->drawRectangle(14,4,60,19,1,LIGHTGRAY,DARKGRAY);
		
					nproto->setcolor(WHITE);
					nproto->setposxy(16,5);
					nproto->senddata(INSTALL_URCS_04);
					nproto->setposxy(16,6);
					nproto->senddata(line);
					nproto->setposxy(20,7);
					nproto->senddata("1. Use default user structure: ");
					if (use_default_user_struct[0]=='Y') nproto->senddata("[Y]");
					else nproto->senddata("[N]");

					nproto->setposxy(20,8);
					nproto->senddata("2. Allow remote authentification: ");
					if (allow_upusers[0]=='Y') nproto->senddata("[Y]");
					else nproto->senddata("[N]");

					nproto->setposxy(20,9);
					nproto->senddata("3. Be a remote authentification server: ");
					if (allow_be_upuser_server[0]=='Y') nproto->senddata("[Y]");
					else nproto->senddata("[N]");
				
					nproto->setposxy(20,10);
					nproto->senddata("4. Remote-Auth server: ");
					nproto->senddata(upuser_server);

					nproto->setposxy(20,11);
					nproto->senddata("5. Username: ");
					nproto->senddata(username);

					nproto->setposxy(20,12);
					nproto->senddata("6. Password: ");
					for (size_t gt=0;gt<strlen(password);gt++) nproto->senddata("*");

					nproto->setposxy(20,13);
					nproto->senddata("7. return");

					nproto->setposxy(15,21);
					nproto->senddata("What do you want to change? ");
					nproto->getdnline(choice,5);
					if (choice[0]=='2') 
					{
						if (allow_upusers[0]=='Y') allow_upusers[0]='N';
						else allow_upusers[0]='Y';
					}
					else if (choice[0]=='1') 
					{
						if (use_default_user_struct[0]=='Y') use_default_user_struct[0]='N';
						else use_default_user_struct[0]='Y';
					}
					else if (choice[0]=='3') 
					{
						if (allow_be_upuser_server[0]=='Y') allow_be_upuser_server[0]='N';
						else allow_be_upuser_server[0]='Y';
					}
					else if (choice[0]=='4') 
					{
						nproto->setposxy(15,21);
						nproto->senddata("                                                         ");
						nproto->setposxy(15,21);
						nproto->senddata("New Value: ");
						nproto->getdnline(upuser_server,sizeof(upuser_server));
						fns->denter(upuser_server);
					}
					else if (choice[0]=='5') 
					{
						nproto->setposxy(15,21);
						nproto->senddata("                                                         ");
						nproto->setposxy(15,21);
						nproto->senddata("New Value: ");
						nproto->getdnline(username,sizeof(username));
						fns->denter(username);

					}
					else if (choice[0]=='6') 
					{
						nproto->setposxy(15,21);
						nproto->senddata("                                                         ");
						nproto->setposxy(15,21);
						nproto->senddata("New Password: ");
						nproto->getdnpass(password,sizeof(password));
						fns->denter(password);

					}
					else if (choice[0]=='7') { ch_3_leave=true; }
				}
			}
			else if (choice[0]=='4') 
			{
				bool ch_4_leave=false;
				while(!ch_4_leave)
				{
					//Control de las variables de instalacion
					//char allow_upbounce[5]="Y";
					//char allow_uplink[5]="Y";
					//char mother_name[2048]="proxy.unmanarc.com:3360,";

					//CRear un rectangulo (60x20)
	
					nproto->setbackground(DARKGRAY);
					nproto->setcolor(LIGHTGRAY);
					nproto->drawRectangle(14,4,60,19,1,LIGHTGRAY,DARKGRAY);
		
					nproto->setcolor(WHITE);
					nproto->setposxy(16,5);
					nproto->senddata(INSTALL_URCS_05);
					nproto->setposxy(16,6);
					nproto->senddata(line);
					nproto->setposxy(20,7);
					nproto->senddata("1. Allow Remote Command Execution: ");
					if (allow_uplink[0]=='Y') nproto->senddata("[Y]");
					else nproto->senddata("[N]");

					nproto->setposxy(20,8);
					nproto->senddata("2. Allow Remote Socket Control: ");
					if (allow_upbounce[0]=='Y') nproto->senddata("[Y]");
					else nproto->senddata("[N]");

					nproto->setposxy(20,9);
					nproto->senddata("3. Uplinks: ");
					nproto->senddata(mother_name);

					nproto->setposxy(20,10);
					nproto->senddata("4. return");

					nproto->setposxy(15,21);
					nproto->senddata("What do you want to change? ");
					nproto->getdnline(choice,5);
					if (choice[0]=='2') 
					{
						if (allow_upbounce[0]=='Y') allow_upbounce[0]='N';
						else allow_upbounce[0]='Y';
					}
					else if (choice[0]=='1') 
					{
						if (allow_uplink[0]=='Y') allow_uplink[0]='N';
						else allow_uplink[0]='Y';
					}
					else if (choice[0]=='3') 
					{
						nproto->setposxy(15,21);
						nproto->senddata("                                                         ");
						nproto->setposxy(15,21);
						nproto->senddata("New Value: ");
						nproto->getdnline(mother_name,sizeof(mother_name));
						fns->denter(mother_name);
					}
					else if (choice[0]=='4') { ch_4_leave=true; }
				}

			}
			else if (choice[0]=='5') 
			{
				if (!strcmp(password,"default")) mispass=true;
				else isconfigured=true; 
			}
			else if (choice[0]=='6') { nproto->setbackground(0); return 1; } //Misconfig shell
			else if (choice[0]=='7') 
			{  
				install_location=HKEY_CURRENT_USER;
				strcpy(password,"default");
				strcpy(use_default_user_struct,"Y");
				strcpy(allow_upusers,"Y");
				strcpy(allow_upbounce,"Y");
				
				strcpy(allow_be_upuser_server,"Y");
				strcpy(upuser_server,"proxy.unmanarc.com:3360");

				strcpy(allow_uplink,"Y");
				strcpy(log_data,"N");
				strcpy(mother_name,"proxy.unmanarc.com:3360,");
				strcpy(server_port,"3360");
				strcpy(server_prompt,"[$d $u]");
				install_method=0; 
				_snprintf(server_modules_directory,sizeof(server_modules_directory),"%s\\urcs_modules",data_g->windir);
				_snprintf(server_banner,sizeof(server_banner),"Welcome to %s",data_g->default_domain);
				_snprintf(server_install_directory,sizeof(server_install_directory),"%s",data_g->windir);
				_snprintf(server_name,sizeof(server_name),"%s",data_g->default_domain);
				_snprintf(username,sizeof(username),"%s",data_g->default_username);
			} //Load Defaults
			else if (choice[0]=='8') { nproto->setbackground(0); return 2; } //Disconnect
		}
			
		nproto->setbackground(BLUE);
		nproto->cls();
		nproto->drawRectangle(1,1,79,24,1,LIGHTBLUE,BLUE);
		nproto->setposxy(5,2);
		nproto->setcolor(WHITE);
		nproto->senddata(INSTALL_URCS_06);
		char line[30];
		memset(&line,0,30);
		memset(&line,196,29);
		nproto->setposxy(5,3);
		nproto->senddata(line);

		nproto->setcolor(LIGHTGRAY);
		nproto->setposxy(10,5);
		nproto->senddata("[ ] Writting configuration to the registry");
		nproto->setposxy(10,6);
		nproto->senddata("[ ] Copying file to installation directory");
		nproto->setposxy(10,7);
		nproto->senddata("[ ] Setting up startup mode");
				
		//////////////////////////////////////////////////////////////////
		////////   Escritura de la instalacin en el registro     ////////
		//////////////////////////////////////////////////////////////////
		
		data_g->UsingKey(install_location);
		if (!data_g->PutURCSKey( "Groups", "public",GRP_PUBLIC)) { cannot_write_to_reg=true; continue; }
		data_g->PutURCSKey( "Groups", "route",GRP_ROUTE); 
		data_g->PutURCSKey( "Groups", "user",GRP_USER); 
		data_g->PutURCSKey( "Groups", "tcp",GRP_TCP); 
		if (use_default_user_struct[0]=='Y')
		{
			//Public User:
			data_g->PutURCSKey("Users\\public", "password","4c9184f37cff01bcdc32dc486ec36961");
			data_g->PutURCSKey("Users\\public", "level","public");
			//Route User:
			data_g->PutURCSKey("Users\\route", "password","9c46408a3bc655c68505c57a11d6c4ee");
			data_g->PutURCSKey("Users\\route", "level","route");
		}

		char keydir[1024]; 	_snprintf(keydir,sizeof(keydir),"Users\\%s",username);
		data_g->PutURCSKey(keydir,"password",fns->md5sum(password));
		data_g->PutURCSKey(keydir,"level","admin");
		data_g->PutURCSKey("Settings", "server_modules_directory",server_modules_directory); 
		data_g->PutURCSKey("Settings", "server_banner",server_banner); 
		data_g->PutURCSKey("Settings", "server_prompt",server_prompt);
		data_g->PutURCSKey("Settings", "server_name",server_name);
		data_g->PutURCSKey("Settings", "server_port",server_port);
		data_g->PutURCSKey("Settings", "mother_name",mother_name);
		data_g->PutURCSKey("Settings", "log_data",log_data);
		data_g->PutURCSKey("Settings", "allow_uplink",allow_uplink);
		data_g->PutURCSKey("Settings", "allow_upbounce",allow_upbounce);
		data_g->PutURCSKey("Settings", "allow_upusers",allow_upusers);
		data_g->PutURCSKey("Settings", "allow_be_upuser_server",allow_be_upuser_server);
		data_g->PutURCSKey("Settings", "upuser_server",upuser_server);

		nproto->setposxy(11,5);
		nproto->senddata("Y");

		nproto->setposxy(11,6);
		//copymetodir...
		if (data_g->CopyMeTo(server_install_directory))
		{
			Cregistry reg;
			char install_filename[_MAX_PATH+10];
			_snprintf(install_filename,sizeof(install_filename),"%s\\rms2.exe",server_install_directory);
			nproto->senddata("Y");
			nproto->setposxy(11,7);
			//Tratar de instalar.
			if (install_method==3)
			{
				nproto->senddata("N");
			}
			else if (install_method==0)
			{
				// 0: HKEY_CURRENT_USER
				if (!reg.PutKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "URCS2",install_filename)) nproto->senddata("N");
				else nproto->senddata("Y");
			}
			else if (install_method==1)
			{
				// 1: HKEY_LOCAL_MACHINE
				if (!reg.PutKey(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "URCS2",install_filename)) nproto->senddata("N");
				else nproto->senddata("Y");
			}
			else if (install_method==2)
			{
				// 2: Service
				char item_a[_MAX_PATH+12];
				char item_b[_MAX_PATH+12];
				_snprintf(item_a,sizeof(item_a),"%s\\SRVANY.EXE",server_install_directory);
				_snprintf(item_b,sizeof(item_b),"%sINSTSRV.EXE",data_g->tempdir);

				nproto->setposxy(2,23);
				nproto->senddata("Downloading instsrv.exe...");
				nproto->setposxy(11,7);

				HRESULT   hr = URLDownloadToFile ( NULL,"http://www.electrasoft.com/srvany/instsrv.exe",item_b,0, NULL);
				if ( ! SUCCEEDED(hr) )	nproto->senddata("N");
				else
				{
					nproto->setposxy(2,23);
					nproto->senddata("Downloading srvany.exe... ");
					nproto->setposxy(11,7);

					hr = URLDownloadToFile ( NULL,"http://www.electrasoft.com/srvany/srvany.exe",item_a,0, NULL);
					if ( ! SUCCEEDED(hr) ) 	nproto->senddata("N");
					else 
					{
                        //Instalar como un servicio.
						char installServCmd[_MAX_PATH+256];
						_snprintf(installServCmd,sizeof(installServCmd),"%s URCS2 %s",item_b,item_a);
						WinExec(installServCmd,0);
						//Modificar el key del registro para que srvany ejecute URCS.
						if (!reg.PutKey(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\URCS2\\Parameters", "Application",install_filename)) nproto->senddata("N");
						else nproto->senddata("Y");
					}
				}
			}
		}
		else
		{
			nproto->senddata("N");
			nproto->setposxy(11,7);
			nproto->senddata("N");
		}

		

		//startup install


		char linea[5];
		nproto->setposxy(2,23);
		nproto->senddata("Press any key to continue...");

		nproto->getdnline(linea,5);
		installed=true;
		
	}
	data_g->LoadData();
	nproto->setbackground(0);
	nproto->cls();
	return 0;

}
void Cintep::prg_install(con_v mx[SERVER_CONNECTIONS], int xlogon) //installing system
{
	passed=TRUE;
	firstinstall(mx,xlogon);
}


void Cintep::prg_banner(con_v mx[SERVER_CONNECTIONS], int xlogon) //change banner
{
	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,""))
		nproto->senddata("Usage:\nBanner [-h] Welcome to...\n");
	else
	{
		data_g->PutURCSKey("Settings", "server_banner",mx[xlogon].cmdline); 
		data_g->LoadData();
		nproto->senddata("Banner changed to: ");
		nproto->senddata(mx[xlogon].cmdline);
		nproto->senddata("\n");
	}
}
void Cintep::prg_sname(con_v mx[SERVER_CONNECTIONS], int xlogon) //change server_name
{
	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,""))
		nproto->senddata("Usage:\nsname [-h] coputer-01...\n");
	else
	{
		data_g->PutURCSKey("Settings", "server_name",mx[xlogon].cmdline); 
		data_g->LoadData();
		nproto->senddata("Server name changed to: ");
		nproto->senddata(mx[xlogon].cmdline);
		nproto->senddata("\n");
	}	
}
void Cintep::prg_sport(con_v mx[SERVER_CONNECTIONS], int xlogon) //change server_port
{
	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,""))
		nproto->senddata("Usage:\nsport [-h] 3360\nwhen 3360 are the server port\n");
	else
	{

		char sPort[80];
		int iPort=atoi(mx[xlogon].cmdline);
		if (iPort>65535 || iPort < 1)
		{
			nproto->senddata("Invalid port ranges (1-65535)...\nURCS works at 3360 by default\n");
		}
		else
		{
			itoa(iPort,sPort,10);
			data_g->PutURCSKey("Settings","server_port",sPort);
			data_g->LoadData();
			nproto->senddata("Server port changed to:");
			nproto->senddata(sPort);
			nproto->senddata("\nPlease do \"restart\" to do effect\n");
		}
	}	
}

void Cintep::prg_mname(con_v mx[SERVER_CONNECTIONS], int xlogon) //change mother name
{
	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,""))
	{
		nproto->sendline("Usage:");
		nproto->sendline("1. mname 192.168.0.1:3360,");
		nproto->sendline("2. mname 192.168.0.1:3360,192.168.0.2:3360,");
		nproto->sendline("for:");
		nproto->sendline("1. set 1 mother name");
		nproto->sendline("2. set 2 or more mothers names");
		nproto->sendline("usage: ip:port, followed by nexts ip:port,");
	}
	else
	{
		data_g->PutURCSKey("Settings","mother_name",mx[xlogon].cmdline); 
		data_g->LoadData();
		nproto->senddata("Mother name changed to: ");
		nproto->senddata(mx[xlogon].cmdline);
		nproto->senddata("\n");
	}
}
void Cintep::prg_restart(con_v mx[SERVER_CONNECTIONS], int xlogon) //restart server
{
	passed=TRUE;
	nproto->senddata("Warning: All server operations will be closed now....\nrestarting...\n");
	//Start new server with 5 seconds of delay
	char newsvr[_MAX_PATH+20];
	_snprintf(newsvr,sizeof(newsvr),"%s delay",data_g->lcname);
    
	STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process. 
    if( !CreateProcess( NULL, // No module name (use command line). 
        newsvr, // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        0,                // No creation flags. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
    ) 
    {
		nproto->senddata("CreateProcess failed.\n" );
    }
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
	//closing server...
	bye=1;
}
void Cintep::prg_restart_mother(con_v mx[SERVER_CONNECTIONS], int xlogon) //restart connection with mother
{
	passed=TRUE;
}
void Cintep::prg_upgrade(con_v mx[SERVER_CONNECTIONS], int xlogon) //upgrade server with mother server version
{
	passed=TRUE;
}
void Cintep::prg_upload(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//upload file to IP
	//upload -d [destip] -f [localfile] remotefile
	passed=TRUE;

	char respd[COMMAND_LINE];
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	strncpy(respd,fns->getchararg(mx[xlogon].cmdline,"d"),COMMAND_LINE);
	fns->getzeroarg(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline,""))
		nproto->senddata("UPLOAD INTERNAL UTILITY\nupload [-d destination_file] local_file\n");
	else
	{
		if (!strcmp(respd,""))
		{
			//get first \ or /
			size_t pkb=strlen(mx[xlogon].cmdline);
			while (pkb>0 && mx[xlogon].cmdline[pkb]!='\\' && mx[xlogon].cmdline[pkb]!='/') pkb--;
			strncpy(respd,mx[xlogon].cmdline+pkb,sizeof(respd));
		}
		// All data well...
		//Making new connection to: respd
		nproto->senddata("Uploading file...");
		int g=nproto->recvfile(mx[xlogon].cmdline,respd);
		if (g>0) nproto->senddata("Uploaded\n");
		else nproto->senddata("Unknown error\n");
	}
}
void Cintep::prg_reg_getkey(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//getkey -l HKEY -d Directory -k key
	passed=TRUE;
	char respl[256];
	char respd[256];
	char respk[128];

	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	strncpy(respl,fns->getchararg(mx[xlogon].cmdline,"l"),sizeof(respl));
	strncpy(respd,fns->getchararg(mx[xlogon].cmdline,"d"),sizeof(respd));
	strncpy(respd,fns->convert(respd,mx,xlogon),sizeof(respd));

	strncpy(respk,fns->getchararg(mx[xlogon].cmdline,"k"),sizeof(respk));
	strncpy(respk,fns->convert(respk,mx,xlogon),sizeof(respk));

	fns->getzeroarg(mx[xlogon].cmdline);

	if (bresph || !strcmp(respl,"") || !strcmp(respd,"") || !strcmp(respk,""))
	{		
		nproto->sendline("REG_GETKEY reads ASCII key from windows registry");
		nproto->sendline("options:");
		nproto->sendline(" -h	help");
		nproto->sendline(" -l	HKEY (HKEY_LOCAL_MACHINE,etc)");
		nproto->sendline(" -d	Directory.");
		nproto->sendline(" -k	keyname");
		nproto->sendline("example:");
		nproto->sendline("#REG_GETKEY -l HKEY_CURRENT_USERS -d Software\\Microsoft\\Windows\\CurrentVersion\\Run -k URCS2");
	}
	else
	{
		HKEY keyts;
		if (!strcmp(respl,"HKEY_LOCAL_MACHINE")) keyts=HKEY_LOCAL_MACHINE;
		else if (!strcmp(respl,"HKEY_CLASSES_ROOT")) keyts=HKEY_CLASSES_ROOT;
		else if (!strcmp(respl,"HKEY_CURRENT_USER")) keyts=HKEY_CURRENT_USER;
		else if (!strcmp(respl,"HKEY_USERS")) keyts=HKEY_USERS;
		else if (!strcmp(respl,"HKEY_PERFORMANCE_DATA")) keyts=HKEY_PERFORMANCE_DATA;
		else if (!strcmp(respl,"HKEY_PERFORMANCE_TEXT")) keyts=HKEY_PERFORMANCE_TEXT;
		else if (!strcmp(respl,"HKEY_PERFORMANCE_NLSTEXT")) keyts=HKEY_PERFORMANCE_NLSTEXT;
		else if (!strcmp(respl,"HKEY_CURRENT_CONFIG")) keyts=HKEY_CURRENT_CONFIG;
		else if (!strcmp(respl,"HKEY_DYN_DATA")) keyts=HKEY_DYN_DATA;
		else keyts=HKEY_LOCAL_MACHINE;
		char rsp[COMMAND_LINE];
		if (data_g->GetKey(keyts,respd,respk,rsp,COMMAND_LINE))
		{
			nproto->senddata("Data: ");
			nproto->sendline(rsp);
		}
		else nproto->sendline("ERROR: Cannot access this key");
	}
}
void Cintep::prg_reg_putkey(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//putkey -l HKEY -d Directory -k key -v value
	passed=TRUE;
	char respl[256];
	char respd[256];
	char respk[128];
	char respv[1024];

	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	strncpy(respl,fns->getchararg(mx[xlogon].cmdline,"l"),sizeof(respl));
	strncpy(respd,fns->getchararg(mx[xlogon].cmdline,"d"),sizeof(respd));
	strncpy(respd,fns->convert(respd,mx,xlogon),sizeof(respd));
	strncpy(respk,fns->getchararg(mx[xlogon].cmdline,"k"),sizeof(respk));
	strncpy(respk,fns->convert(respk,mx,xlogon),sizeof(respk));
	strncpy(respv,fns->getchararg(mx[xlogon].cmdline,"v"),sizeof(respv));
	strncpy(respv,fns->convert(respv,mx,xlogon),sizeof(respv));
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(respl,"") || !strcmp(respd,"") || !strcmp(respk,""))
	{		
		nproto->sendline("REG_PUTKEY write ASCII key to windows registry");
		nproto->sendline("options:");
		nproto->sendline(" -h	help");
		nproto->sendline(" -l	HKEY (HKEY_LOCAL_MACHINE,etc)");
		nproto->sendline(" -d	Directory.");
		nproto->sendline(" -k	keyname");
		nproto->sendline(" -v	value");
		nproto->sendline("example:");
		nproto->sendline("#REG_PUTKEY -l HKEY_CURRENT_USERS -d Software\\Microsoft\\Windows\\CurrentVersion\\Run -k URCS2 -v c:\\windows\\rms2.exe");
	}
	else
	{
		HKEY keyts;
		if (!strcmp(respl,"HKEY_LOCAL_MACHINE")) keyts=HKEY_LOCAL_MACHINE;
		else if (!strcmp(respl,"HKEY_CLASSES_ROOT")) keyts=HKEY_CLASSES_ROOT;
		else if (!strcmp(respl,"HKEY_CURRENT_USER")) keyts=HKEY_CURRENT_USER;
		else if (!strcmp(respl,"HKEY_USERS")) keyts=HKEY_USERS;
		else if (!strcmp(respl,"HKEY_PERFORMANCE_DATA")) keyts=HKEY_PERFORMANCE_DATA;
		else if (!strcmp(respl,"HKEY_PERFORMANCE_TEXT")) keyts=HKEY_PERFORMANCE_TEXT;
		else if (!strcmp(respl,"HKEY_PERFORMANCE_NLSTEXT")) keyts=HKEY_PERFORMANCE_NLSTEXT;
		else if (!strcmp(respl,"HKEY_CURRENT_CONFIG")) keyts=HKEY_CURRENT_CONFIG;
		else if (!strcmp(respl,"HKEY_DYN_DATA")) keyts=HKEY_DYN_DATA;
		else keyts=HKEY_LOCAL_MACHINE;
		nproto->setAction("Writing registry key");
		if (data_g->PutKey(keyts,respd,respk,respv)) nproto->setActionStatus(true);
		else nproto->setActionStatus(false);
	}
}
void Cintep::prg_reg_delkey(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//delkey -l HKEY -d Directory -k key
	passed=TRUE;
	char respl[256];
	char respd[256];
	char respk[128];

	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	strncpy(respl,fns->getchararg(mx[xlogon].cmdline,"l"),sizeof(respl));
	strncpy(respd,fns->getchararg(mx[xlogon].cmdline,"d"),sizeof(respd));
	strncpy(respd,fns->convert(respd,mx,xlogon),sizeof(respd));
	strncpy(respk,fns->getchararg(mx[xlogon].cmdline,"k"),sizeof(respk));
	strncpy(respk,fns->convert(respk,mx,xlogon),sizeof(respk));
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(respl,"") || !strcmp(respd,"") || !strcmp(respk,""))
	{		
		nproto->sendline("REG_DELKEY deletes a key from windows registry");
		nproto->sendline("options:");
		nproto->sendline(" -h	help");
		nproto->sendline(" -l	HKEY (HKEY_LOCAL_MACHINE,etc)");
		nproto->sendline(" -d	Directory.");
		nproto->sendline(" -k	keyname");
		nproto->sendline("example:");
		nproto->sendline("#REG_DELKEY -l HKEY_CURRENT_USERS -d Software\\Microsoft\\Windows\\CurrentVersion\\Run -k URCS2");
	}
	else
	{
		HKEY keyts;
		if (!strcmp(respl,"HKEY_LOCAL_MACHINE")) keyts=HKEY_LOCAL_MACHINE;
		else if (!strcmp(respl,"HKEY_CLASSES_ROOT")) keyts=HKEY_CLASSES_ROOT;
		else if (!strcmp(respl,"HKEY_CURRENT_USER")) keyts=HKEY_CURRENT_USER;
		else if (!strcmp(respl,"HKEY_USERS")) keyts=HKEY_USERS;
		else if (!strcmp(respl,"HKEY_PERFORMANCE_DATA")) keyts=HKEY_PERFORMANCE_DATA;
		else if (!strcmp(respl,"HKEY_PERFORMANCE_TEXT")) keyts=HKEY_PERFORMANCE_TEXT;
		else if (!strcmp(respl,"HKEY_PERFORMANCE_NLSTEXT")) keyts=HKEY_PERFORMANCE_NLSTEXT;
		else if (!strcmp(respl,"HKEY_CURRENT_CONFIG")) keyts=HKEY_CURRENT_CONFIG;
		else if (!strcmp(respl,"HKEY_DYN_DATA")) keyts=HKEY_DYN_DATA;
		else keyts=HKEY_LOCAL_MACHINE;

		nproto->setAction("Removing registry key");
		if (data_g->DelKey(keyts,respd,respk)) nproto->setActionStatus(true);
		else nproto->setActionStatus(false);
	}

}

void Cintep::prg_download(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;

	char respd[COMMAND_LINE];
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	strncpy(respd,fns->getchararg(mx[xlogon].cmdline,"d"),COMMAND_LINE);
	fns->getzeroarg(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline,""))
		nproto->senddata("DOWNLOAD INTERNAL UTILITY\ndownload [-d local_file] serverfile\nIf not file specified.. the name are same that serverfile\n");
	else
	{
		if (!strcmp(respd,""))
		{
			//get first \ or /
			size_t pkb=strlen(mx[xlogon].cmdline);
			while (pkb>0 && mx[xlogon].cmdline[pkb]!='\\' && mx[xlogon].cmdline[pkb]!='/') pkb--;
			strncpy(respd,mx[xlogon].cmdline+pkb,sizeof(respd));
		}
		// All data well...
		//Making new connection to: respd
		nproto->senddata("Downloading file...");
		int g=nproto->sendfile(respd,mx[xlogon].cmdline);
		if (g>0) nproto->senddata("Downloaded\n");
		else nproto->senddata("Unknown error\n");
	}

}
void Cintep::prg_downloadfrom(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	
	char respd[COMMAND_LINE];
	char respf[COMMAND_LINE];

	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");

	strncpy(respd,fns->getchararg(mx[xlogon].cmdline,"d"),COMMAND_LINE);
	strncpy(respf,fns->getchararg(mx[xlogon].cmdline,"f"),COMMAND_LINE);

	fns->getzeroarg(mx[xlogon].cmdline);

	if (bresph || !strcmp(respd,"") || !strcmp(respf,""))
		nproto->senddata("DOWNLOADFROM INTERNAL UTILITY\n-d destination IP\n-h help\n-f RemoteFile\n\rand local file\n\rExample:  downloadfrom -d 216.72.226.8 -f readme.txt rd.txt\n");
	else
	{
		// All data well...
        //Making new connection to: respd
		Ctransf ft;
		nproto->senddata("Downloading file...");
		int g=ft.downloadfrom(respd,respf,mx[xlogon].cmdline);
		if (g>0) nproto->senddata("downloaded\n");
		else
		{
			if (g==-4) nproto->senddata("Host not founded\n");
			if (g==-1) nproto->senddata("file not found\n");
			if (g==-2) nproto->senddata("Unknown error\n");
			if (g==-3) nproto->senddata("Cannot Start sockets\n");
		}

	}
}
void Cintep::prg_uploadto(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//upload file to IP
	//upload -d [destip] -f [localfile] remotefile
	passed=TRUE;

	char respd[COMMAND_LINE];
	char respf[COMMAND_LINE];
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");

	strncpy(respd,fns->getchararg(mx[xlogon].cmdline,"d"),COMMAND_LINE);
	strncpy(respf,fns->getchararg(mx[xlogon].cmdline,"f"),COMMAND_LINE);

	fns->getzeroarg(mx[xlogon].cmdline);

	if (bresph || !strcmp(respd,"") || !strcmp(respf,""))
		nproto->senddata("UPLOADTO INTERNAL UTILITY\n-d destination IP\n-h help\n-f LocalFile\nand remote file\n\rExample_:  uploadto -d 216.72.226.8 -f c:\\autoexec.bat autoexec.bat.bak\n");
	else
	{
		// All data well...
        //Making new connection to: respd
		Ctransf ft;
		nproto->senddata("Uploading file...");
		int g=ft.uploadto(respd,respf,mx[xlogon].cmdline);
		if (g>0) nproto->senddata("uploaded\n");
		else
		{
			if (g==-4) nproto->senddata("Host not founded\n");
			if (g==-1) nproto->senddata("Local file not found\n");
			if (g==-2) nproto->senddata("Unknown error\n");
			if (g==-3) nproto->senddata("Cannot Start sockets\n");
		}
	}
}
void Cintep::prg_prx_uplink(con_v mx[SERVER_CONNECTIONS],Cservers * svrs, int xlogon)
{
	passed=TRUE;
	char lcmd[COMMAND_LINE];
	strncpy(lcmd,mx[xlogon].cmdline,COMMAND_LINE);
	char resps[128];
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");	//Help
	bool brespr=fns->getboolarg(mx[xlogon].cmdline,"r"); //Recursivo
	strncpy(resps,fns->getchararg(mx[xlogon].cmdline,"s"),128);
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,""))
	{
		nproto->sendline("PRX_UPLINK Sends command to all servers attached to this server");
		nproto->sendline("options:");
		nproto->sendline(" -h	help");
		nproto->sendline(" -s	specific proxy ID");
		nproto->sendline(" -r	Recursively flag actived.");
		nproto->sendline("example:");
		nproto->sendline("#PRX_UPLINK -r msgbox hallo!!");
	}
	else
	{
		char exterm[COMMAND_LINE];
		nproto->sendline("Sending commands to:");
		for(int n=0;n<svrs->getServerSlots();n++)
		{
			if (svrs->isBusy(n) && !svrs->isClientBusy(n))
			{
				if ( !strcmp(resps,"") || atoi(resps)==n ) 
				{
					char prxn[256];	svrs->getNameServer(n,prxn,sizeof(prxn));
					if (brespr)	_snprintf(exterm,sizeof(exterm),"%s (%s) -> Sending Recursively Signal.",svrs->lgetIp(n),prxn);
					else _snprintf(exterm,sizeof(exterm),"%s (%s) -> Sending Signal.",svrs->lgetIp(n),prxn);
					nproto->sendline(exterm);
					//svrs[n]->cbsy=1;
					svrs->setClientBusy(n,true);
					send(svrs->getSocket(n),"SERVAUTH",80,0);
					char cmdx[4096];
					if (brespr) _snprintf(cmdx,sizeof(cmdx),"R%s",mx[xlogon].cmdline);
					else		_snprintf(cmdx,sizeof(cmdx),"K%s",mx[xlogon].cmdline);
					send(svrs->getSocket(n),cmdx,4096,0);
					svrs->freeSlot(n);
				}
			}
		}
		nproto->sendline("Finished...");
	}

}
void Cintep::prg_prx_detail(con_v mx[SERVER_CONNECTIONS],Cservers * svrs, int xlogon)
{
	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");	//Help
	fns->getzeroarg(mx[xlogon].cmdline); // ID

	if (bresph || !strcmp(mx[xlogon].cmdline,""))
	{
		nproto->sendline("PRX_DETAIL Show Detailed data about specified server");
		nproto->sendline("prxdetail -h [serverid]");
		nproto->sendline("options:");
		nproto->sendline(" -h	help");
		nproto->sendline("example:");
		nproto->sendline("#PRX_DETAIL 1");
	}
	else
	{
		int server=atoi(mx[xlogon].cmdline);
		if (svrs->isBusy(server))
		{
			char detail[50];
			nproto->sendline("Proxy detailed information:");
			nproto->sendline("---------------------------");

			nproto->senddata("ID                        : ");
			_snprintf(detail,sizeof(detail),"%d",server);
			nproto->sendline(detail);

			nproto->senddata("Name                      : ");
			svrs->getNameServer(server,detail,sizeof(detail));
			nproto->sendline(detail);

			nproto->senddata("Accept Mother Commands    : ");
			_snprintf(detail,sizeof(detail),"%u",svrs->getprx_uplink(server));
			nproto->sendline(detail);

			nproto->senddata("Accept TCP Redirection    : ");
			_snprintf(detail,sizeof(detail),"%u",svrs->getprx_upbounce(server));
			nproto->sendline(detail);

			nproto->senddata("Have Remote Auth          : ");
			_snprintf(detail,sizeof(detail),"%u",svrs->getprx_upuser(server));
			nproto->sendline(detail);

			nproto->senddata("Log Activities            : ");
			_snprintf(detail,sizeof(detail),"%u",svrs->getprx_logdata(server));
			nproto->sendline(detail);

			nproto->senddata("busy                      : ");
			_snprintf(detail,sizeof(detail),"%u",svrs->isClientBusy(server));
			nproto->sendline(detail);

			nproto->senddata("IP Address                : ");
			svrs->getIp(server,detail,sizeof(detail));
			nproto->sendline(detail);

			nproto->senddata("Socket                    : ");
			_snprintf(detail,sizeof(detail),"%u",svrs->getSocket(server));
			nproto->sendline(detail);

            nproto->senddata("Connected since           : ");
			time_t tx=svrs->getConnectionTime(server);
			_snprintf(detail,sizeof(detail),"%s",ctime(&tx));
			nproto->senddata(detail);

/*			nproto->senddata("Bytes Transmited          : ");
			_snprintf(detail,sizeof(detail),"%u",svrs->getSentBytes(server));
			nproto->sendline(detail);

			nproto->senddata("Bytes Received            : ");
			_snprintf(detail,sizeof(detail),"%u",svrs->getRecvBytes(server));
			nproto->sendline(detail);*/
			
		}
		else
		{
			nproto->sendline("No servers founded.");
		}
	}
}
void Cintep::prg_prx_who(con_v mx[SERVER_CONNECTIONS],Cservers * svrs, int xlogon)
{
	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	bool brespa=fns->getboolarg(mx[xlogon].cmdline,"a");
	bool brespr=fns->getboolarg(mx[xlogon].cmdline,"r");
	if (bresph)
	{
		nproto->sendline("PRX_WHO List all proxys attached to this server");
		nproto->sendline("options:");
		nproto->sendline(" -h	help");
		nproto->sendline(" -a	list all proxy's including connected proxys");
		nproto->sendline("example:");
		nproto->sendline("#prx_who -a");
	}
	else
	{
		char exterm[COMMAND_LINE];
		mx[xlogon].cpu.i_a=nproto->setcolor(LIGHTBLUE);
		//nproto->sendline("   ID  SOCK                IP                      Name of server");
		nproto->sendline("ID Server Name                                IP Address       BNC RLG RCM LOG");
		nproto->sendline("-- -----------                                ---------------- --- --- --- ---");

		mx[xlogon].cpu.i_a=nproto->setdefaultcolor();
		for(int n=0;n<svrs->getServerSlots();n++)
		{
			if (svrs->isBusy(n))
			{
				if (!svrs->isClientBusy(n))
				{
					//proxy is alive. showing data.
					char server_name[42];
					svrs->getNameServer(n,server_name,42);
					_snprintf(exterm,sizeof(exterm),"%02u %-42s %-17s %0u   %0u   %0u   %0u\n",n,server_name,svrs->lgetIp(n),svrs->getprx_upbounce(n),svrs->getprx_upuser(n),svrs->getprx_uplink(n),svrs->getprx_logdata(n));
					nproto->senddata(exterm);
				}
				else if (brespa)
				{
					//user are connected to proxy.
					//proxy is alive. showing data.
					char server_name[80];
					svrs->getNameServer(n,server_name,80);
					_snprintf(exterm,sizeof(exterm),"%02u %-42s %-17s %0u   %0u   %0u   %0u\n",n,server_name,svrs->lgetIp(n),svrs->getprx_upbounce(n),svrs->getprx_upuser(n),svrs->getprx_uplink(n),svrs->getprx_logdata(n));
					//					_snprintf(exterm,"%5u %5u %17s %35s\n",n,svrs->getSocket(n),svrs->lgetIp(n),server_name);
					mx[xlogon].cpu.i_a=nproto->setcolor(LIGHTRED);
					nproto->senddata(exterm);
					mx[xlogon].cpu.i_a=nproto->setdefaultcolor();
				}
			}
		}
		//nproto->senddata("* denotes that proxy is in use.\n");
	}

}
void Cintep::prg_prx_close(con_v mx[SERVER_CONNECTIONS],Cservers * svrs, int xlogon)
{
	passed=TRUE;
	char exlusive[COMMAND_LINE];
	int mew=0;
	fns->denter(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline+strlen("PRX_CLOSE "),""))
	{
		nproto->senddata("USAGE: prx_close [prx_ID]");
		return;
	}
	else
		strncpy(exlusive,mx[xlogon].cmdline+strlen("PRX_CLOSE "),COMMAND_LINE);

	//on exlusive 
	mew=atoi(exlusive);
	//mew is an PRX_ID
	if(!svrs->isBusy(mew))
	{
		nproto->senddata("This pointer does not has an asociation\n");
		return;
	}
	if (svrs->isClientBusy(mew))
	{
		nproto->senddata("Another client is connected to this server.\nThis operation will stop any communication\n");
	}
	svrs->freeSlot(mew);
	nproto->senddata("Closed Connection :S we hope that client are alive\n");
}


void Cintep::prg_prx_bounce(con_v mx[SERVER_CONNECTIONS],Cservers * svrs, int xlogon)
{
	//prx_bounce -h -i ProxyID -l LocalPort -p DestPort EndPointHostname
	passed=TRUE;
	char lcmd[COMMAND_LINE];
	strncpy(lcmd,mx[xlogon].cmdline,COMMAND_LINE);
	char respl[128],respp[128],respi[128];
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");	//Help
	
	strncpy(respl,fns->getchararg(mx[xlogon].cmdline,"l"),128);
	strncpy(respp,fns->getchararg(mx[xlogon].cmdline,"p"),128);
	strncpy(respi,fns->getchararg(mx[xlogon].cmdline,"i"),128);

	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,"") || !strcmp(respi,"") || !strcmp(respp,"") || !strcmp(respl,""))
	{
		nproto->sendline("PRX_BOUNCE Establish a TCP tunnel between 2 URCS's");
		nproto->sendline("options:");
		nproto->sendline(" -h	help");
		nproto->sendline(" -l	Local listen port");
		nproto->sendline(" -i	Proxy ID used as tunnel");
		nproto->sendline(" -p	Remote port");
		

		nproto->sendline("example:");
		nproto->sendline("#PRX_BOUNCE -i 1 -l 8080 -p 80 unmanarc.com");
	}
	else
	{
		unsigned short lport, rport;
		int proxyServerID=atoi(respi);
		nproto->setAction("Starting TCP Redirection");
		if (svrs->isClientBusy(proxyServerID))
		{
			nproto->setActionStatus(0);
			nproto->sendline("Another client is connected to this server.");
			return;
		}
		svrs->setClientBusy(proxyServerID,true);
		
		SOCKET proxyUsed=svrs->getSocket(proxyServerID);
		lport=atoi(respl);
		rport=atoi(respp);

		while (svrs->isBusy(proxyServerID) && svrs->isBlocked(proxyServerID)) {}//No empezar el transfer hasta que el proxy sea desbloqueado por completo
		Cbouncing bnc;
		int rst=bnc.listener(lport,proxyUsed,mx[xlogon].cmdline,rport);		

		if		(rst==1 ) { nproto->setActionStatus(1); nproto->sendline("Proxy Created Successfully"); }
		else if (rst==-1) { nproto->setActionStatus(0); nproto->sendline("Error: Cannot open socket for listen here."); }
		else if (rst==-2) { nproto->setActionStatus(0); nproto->sendline("Error: This proxy does not allow TCP redirection."); }
		else if (rst==-3) { nproto->setActionStatus(0); nproto->sendline("Error: Proxy error."); }
		else { nproto->setActionStatus(0); nproto->sendline("Error: Unknown error."); }

		//	svrs->freeSlot(proxyServerID);
	}

}




void Cintep::prg_prx_attach(con_v mx[SERVER_CONNECTIONS],Cservers * svrs, int xlogon)
{
	//prx_attach -h -p port hostname
	passed=TRUE;

	char respp[COMMAND_LINE];
	char resph[12];

	BOOL bresph=FALSE;
	BOOL brespp=FALSE;

	strncpy(respp,fns->deparg(mx[xlogon].cmdline,"-p",FALSE),COMMAND_LINE);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=TRUE;
	if (strcmp(respp,"-p"))
		brespp=TRUE;

	if (bresph==TRUE || !strcmp(mx[xlogon].cmdline+11,""))
		nproto->senddata("PRX_ATTACH utility connect to another URCS\n-p port\n-h this help\nExample:  prx_attach -p 3360 proxy.unmanarc.com\n");
	else
	{
		Cproxy prd(data_g);
		unsigned int prt;
		if (brespp) prt=atoi(respp);
		else prt=3360;
		int i=prd.attach(mx[xlogon].cmdline+11,prt,mx[xlogon].socket);
	}
	nproto->settitle(mx[xlogon].title);
}
void Cintep::prg_prx_connect(con_v mx[SERVER_CONNECTIONS],Cservers * svrs, int xlogon)
{
	passed=TRUE;
	//USAGE: prx_connect prx_id
	char exlusive[COMMAND_LINE];
	int mew=0;
	fns->denter(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline+strlen("PRX_CONNECT "),""))
	{
		nproto->senddata("USAGE: prx_connect [prx_ID]\n");
		return;
	}
	else
		strncpy(exlusive,mx[xlogon].cmdline+strlen("PRX_CONNECT "),COMMAND_LINE);

	//on exlusive 
	mew=atoi(exlusive);
	//mew is an PRX_ID
	if(!svrs->isBusy(mew))
	{
		nproto->senddata("This pointer does not has an asociation\n");
		return;
	}
	if (svrs->isClientBusy(mew))
	{
		nproto->senddata("Another client is connected to this server.\n");
		return;
	}
	//Send Activation Seq.
	svrs->setClientBusy(mew,true);
	Cproxy prd(data_g);
	prd.start_proxy_instance(mx[xlogon].socket,svrs->getSocket(mew));

	//Activated Sequence... All server data will be redirected to
	//client.
	while (svrs->isBusy(mew) && svrs->isBlocked(mew)) {}//No empezar el transfer hasta que el proxy sea desbloqueado por completo
	int j=prd.transfer(); //do proxy...
	if (j<=0) svrs->freeSlot(mew); //server closes connection
	nproto->settitle(mx[xlogon].title);
}

void Cintep::prg_net_download(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//net_download -h -d DestFile URL
	passed=TRUE;
	char lcmd[COMMAND_LINE];
	strncpy(lcmd,mx[xlogon].cmdline,COMMAND_LINE);
	char respd[_MAX_PATH];
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");	//Help
	strncpy(respd,fns->getchararg(mx[xlogon].cmdline,"d"),_MAX_PATH);
	strncpy(respd,fns->convert(respd,mx,xlogon),_MAX_PATH);
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,"") || !strcmp(respd,""))
	{
		nproto->sendline("NET_DOWNLOAD Downloads a file from an URL");
		nproto->sendline("options:");
		nproto->sendline(" -h	help");
		nproto->sendline(" -d	Destination File");
		nproto->sendline("example:");
		nproto->sendline("#NET_DOWNLOAD -d index.html http://www.unmanarc.com/index.asp");
		nproto->sendline("Note: Destination filename accept formatted data (hello$sguy.php)");
	}
	else
	{
		nproto->senddata("Donwload ");
		nproto->sendline(mx[xlogon].cmdline);
		nproto->setAction("Donwloading...");
		HRESULT   hr = URLDownloadToFile ( NULL,mx[xlogon].cmdline,respd,0, NULL);
		if ( ! SUCCEEDED(hr) )	nproto->setActionStatus(false);
		else nproto->setActionStatus(true);
	}
}

void Cintep::prg_net_lookup(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	fns->denter(mx[xlogon].cmdline);
	if (!strcmp(mx[xlogon].cmdline+10,""))
		nproto->senddata("NetLookup 1.0\nUSAGE: net_lookup hostname\n");
	else
	{
		struct sockaddr_in Server_add;
		struct hostent *hentry;
		if ((hentry = gethostbyname(mx[xlogon].cmdline+11)) == NULL)
			nproto->senddata("Cannot resolve hostname\n");
		else
		{
			//data resolved... 
			char dottedip[80];
			memset((char *)&Server_add, 0, sizeof(Server_add));
			memcpy(&Server_add.sin_addr.s_addr, *(hentry->h_addr_list),sizeof(Server_add.sin_addr.s_addr));
			//host are copied to Server_add
			_snprintf(dottedip,sizeof(dottedip),"IP: %s\n",inet_ntoa(Server_add.sin_addr));
			nproto->senddata(dottedip);
		}
	}
}


void Cintep::prg_net_sendto(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//net_sendto [-h] [-s] [-p ticket] [-i ip] [-t port] [-l long] message.
	passed=TRUE;
	
	char exlusive[COMMAND_LINE];
	char respp[12];
	char respl[12];
	char respt[12];
	char respi[45];
	char resph[12];
	char resps[12];

	BOOL bresph=0;
	BOOL bresps=0;

	strncpy(resps,fns->deparg(mx[xlogon].cmdline,"-s",FALSE),12);
	strncpy(respp,fns->deparg(mx[xlogon].cmdline,"-p",FALSE),12);
	strncpy(respl,fns->deparg(mx[xlogon].cmdline,"-l",FALSE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);
	strncpy(respt,fns->deparg(mx[xlogon].cmdline,"-t",FALSE),12);
	strncpy(respi,fns->deparg(mx[xlogon].cmdline,"-i",FALSE),45);

    if (strcmp(resph,"-h"))		bresph=1;
	if (strcmp(resps,"-s"))		bresps=1;

	fns->denter(mx[xlogon].cmdline);


	if (bresph || !strcmp(mx[xlogon].cmdline+strlen("NET_SENDTO "),"")	|| !strcmp(respi,"-i") || !strcmp(respt,"-t") || !strcmp(respp,"-p"))
		nproto->senddata("net_sendto [-h] [-s] [-p ticket] [-i ip] [-t port] [-l long] [data].\n-s silent\n-i ip of host\n-t port to send\n-l use this long\n\r-p Ticket (Socket Number)\n[Data] Formmated data such as: Hola$sComo$sEstas\n");
	else
	{
		strncpy(exlusive,mx[xlogon].cmdline+strlen("NET_SENDTO "),COMMAND_LINE);
		//Do operations...
		SOCKET p=atoi(respp);
		char mfs[COMMAND_LINE];
		int r=-1;
		size_t lng=0;
		memset(&mfs,0,COMMAND_LINE);
		strncpy(mfs,fns->convert(exlusive,mx, xlogon),COMMAND_LINE);
		if (strcmp(respl,"-l"))
			lng=atoi(respl);
		else
			lng=strlen(mfs);
		//create struct...
		struct sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_port = htons(atoi(respt));
		server.sin_addr.s_addr = inet_addr( respi );
		r=sendto(p,mfs,(int)lng,0,(const struct sockaddr *)&server,sizeof(struct sockaddr_in));
//		r=send(p,mfs,(int)lng,0);
		if (r<=0)
		{
			nproto->senddata("Can't send data\n");
			mx[xlogon].cpu.b_a=0;
		}
		else
		{
			if (!bresps)
			{
				char ds[80];
				_snprintf(ds,sizeof(ds),"%u bytes sendedto succesfull to socket %u\n",r,p);
				nproto->senddata(ds);
				mx[xlogon].cpu.b_a=1;
			}
			else
			{
				mx[xlogon].cpu.b_a=1;
			}
		}
	}
}

void Cintep::prg_net_recvfrom(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//net_receivefrom [-h] [-p ticket] [-i ip] [-t port] [-l long]
	passed=TRUE;

	char respp[12];
	char respr[12];
	char respl[12];
	char resph[12];
	char respt[12];
	char respi[45];

	BOOL bresph=0;
	BOOL brespp=0;
	BOOL brespr=0;

	strncpy(respp,fns->deparg(mx[xlogon].cmdline,"-p",FALSE),12);
	strncpy(respl,fns->deparg(mx[xlogon].cmdline,"-l",FALSE),12);
	strncpy(respr,fns->deparg(mx[xlogon].cmdline,"-r",TRUE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);
	strncpy(respt,fns->deparg(mx[xlogon].cmdline,"-t",FALSE),12);
	strncpy(respi,fns->deparg(mx[xlogon].cmdline,"-i",FALSE),45);

	if (strcmp(respr,"-r"))
		brespr=1;
	if (strcmp(resph,"-h"))
		bresph=1;
	if (!strcmp(respp,"-p"))
		brespp=1;
	fns->denter(mx[xlogon].cmdline);
	if (bresph || brespp || !strcmp(respi,"-i") || !strcmp(respt,"-t"))
		nproto->senddata("net_receivefrom [-r] [-l long] [-i ip] [-t port] -p ticket\n-l use this long (default is 4096)\n-p Ticket (Socket Number)\n-r Show in hexadecimal\n");
	else
	{
		//Do operations...
		SOCKET p=atoi(respp);
		char mfs[4096];
		int r=-1;
		int lng=0;
		memset(&mfs,0,4096);
		if (strcmp(respl,"-l"))
			lng=atoi(respl);
		else
			lng=4096;

		struct sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_port = htons(atoi(respt));
		server.sin_addr.s_addr = inet_addr( respi );
		int ln=sizeof(server);
		r=recvfrom(p,mfs,lng,0,(struct sockaddr *)&server,&ln);
		//r=recv2(p,mfs,lng,0);
		if (r<=0)
			nproto->senddata("Can't Receive data\n");
		else
		{
			char ds[100]="";
			_snprintf(ds,sizeof(ds),"%u bytes received succesfully from socket %u:\n",r,p);
			nproto->senddata(ds);
			if (!brespr)
				nproto->senddata(mfs);
			else
				nproto->senddatahex(mfs,r);
			nproto->senddata("\n\r");
		}
	}

}

void Cintep::prg_net_send(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	
	char exlusive[COMMAND_LINE];
	char respp[12];
	char respl[12];
	char resph[12];

	BOOL bresph=0;

	strncpy(respp,fns->deparg(mx[xlogon].cmdline,"-p",FALSE),12);
	strncpy(respl,fns->deparg(mx[xlogon].cmdline,"-l",FALSE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns->denter(mx[xlogon].cmdline);


	if (bresph || !strcmp(mx[xlogon].cmdline+strlen("NET_SEND "),"") || !strcmp(respp,"-p"))
		nproto->senddata("net_send [-l long] -p ticket [data]\n-l use this long\n\r-p Ticket (Socket Number)\n[Data] Formmated data such as: Hola$sComo$sEstas\n");
	else
	{
		strncpy(exlusive,mx[xlogon].cmdline+strlen("NET_SEND "),COMMAND_LINE);
		//Do operations...
		SOCKET p=atoi(respp);
		char mfs[COMMAND_LINE];
		int r=-1;
		size_t lng=0;
		memset(&mfs,0,COMMAND_LINE);
		strncpy(mfs,fns->convert(exlusive,mx, xlogon),COMMAND_LINE);
		if (strcmp(respl,"-l"))
			lng=atoi(respl);
		else
			lng=strlen(mfs);
		r=send(p,mfs,(int)lng,0);
		if (r<=0)
			nproto->senddata("Can't send data\n");
		else
		{
			char ds[80];
			_snprintf(ds,sizeof(ds),"%u bytes sended succesfull to socket %u\n",r,p);
			nproto->senddata(ds);
		}
	}
}

void Cintep::prg_net_receive(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;

	char respp[12];
	char respr[12];
	char respl[12];
	char resph[12];

	BOOL bresph=0;
	BOOL brespp=0;
	BOOL brespr=0;

	strncpy(respp,fns->deparg(mx[xlogon].cmdline,"-p",FALSE),12);
	strncpy(respl,fns->deparg(mx[xlogon].cmdline,"-l",FALSE),12);

	strncpy(respr,fns->deparg(mx[xlogon].cmdline,"-r",TRUE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(respr,"-r"))
		brespr=1;
	if (strcmp(resph,"-h"))
		bresph=1;
	if (!strcmp(respp,"-p"))
		brespp=1;
	fns->denter(mx[xlogon].cmdline);

	if (bresph || brespp)
		nproto->senddata("net_receive [-r] [-l long] -p ticket\n-l use this long (default is 4096)\n-p Ticket (Socket Number)\n-r Show in hexadecimal\n");
	else
	{
		//Do operations...
		SOCKET p=atoi(respp);
		char mfs[4096];
		int r=-1;
		int lng=0;
		memset(&mfs,0,4096);
		if (strcmp(respl,"-l"))
			lng=atoi(respl);
		else
			lng=4096;
		r=recv2(p,mfs,lng,0);
		if (r<=0)
			nproto->senddata("Can't Receive data\n");
		else
		{
			char ds[100]="";
			_snprintf(ds,sizeof(ds),"%u bytes received succesfully from socket %u:\n",r,p);
			nproto->senddata(ds);
			if (!brespr)
				nproto->senddata(mfs);
			else
				nproto->senddatahex(mfs,r);
			nproto->senddata("\n\r");
		}
	}
}

void Cintep::prg_net_set_timeout(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	// net_set_timeout -t timeout socket
	//setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));

	char respt[12];
	char resph[12];
	BOOL bresph=0;

	strncpy(respt,fns->deparg(mx[xlogon].cmdline,"-t",FALSE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns->denter(mx[xlogon].cmdline);

	if (bresph)
		nproto->senddata("USAGE: net_set_timeout [-t timeout] socket\ntimeout in milliseconds\n");
	else
	{
		if (!strcmp(mx[xlogon].cmdline+16,""))
		{
			nproto->senddata("Need socket\n");
			return;
		}
		if (!strcmp(respt,"-t"))
		{
			nproto->senddata("Need timeout\n");
			return;
		}
		//do...
		int tmout=atoi(respt);
		SOCKET j=atoi(mx[xlogon].cmdline+16);
		setsockopt(j,SOL_SOCKET,SO_RCVTIMEO,(char *)&tmout,sizeof(tmout));
		nproto->senddata("changed\n");
	}
}
void Cintep::prg_net_opensocket(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;

	char exlusive[COMMAND_LINE];
	char respp[12];
	char respu[12];
	char resph[12];

	BOOL bresph=0;
	BOOL brespu=0;

	strncpy(respp,fns->deparg(mx[xlogon].cmdline,"-p",FALSE),12);
	strncpy(respu,fns->deparg(mx[xlogon].cmdline,"-u",TRUE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	if (strcmp(respu,"-u"))
		brespu=1;

	fns->denter(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline+strlen("NET_OPENSOCKET "),""))
		nproto->senddata("net_opensocket [-p port] [-u] [ip]\n\r-p Port Number 1-65535\n-u UDP mode\n[ip] IP Address such as: 127.0.0.1\n\r");
	else
	{
		strncpy(exlusive,mx[xlogon].cmdline+strlen("NET_OPENSOCKET "),COMMAND_LINE);
		//Do operations...
		int port=23;
		port=atoi(respp);
        
		if (!brespu)
		{
			struct sockaddr_in Server_add;
			//WORD wVersionRequested;
			//WSADATA wsaData;
			// Start Winsock
			//wVersionRequested = MAKEWORD( 1, 0 );
			//if( WSAStartup( wVersionRequested, &wsaData ) == 0 )
			//{
				// Make Socket
				Server_add.sin_family      = AF_INET;
				Server_add.sin_port        = htons( port );
				Server_add.sin_addr.s_addr = inet_addr( exlusive );
				SOCKET Sock = socket( AF_INET, SOCK_STREAM, 0 );
				// Connect Server
				if( connect( Sock, (struct sockaddr *) &Server_add, sizeof( Server_add ) ) == 0 )
				{
					char socke[80];
					itoa((int)Sock,socke,10);
					nproto->senddata("Socket Number: ");
					nproto->senddata(socke);
					nproto->senddata("\n\r");
				}
				else
				{
					nproto->senddata("Host is unavailable.\n");
				}
			//}
			//else
			//{
			//	nproto->senddata("Cant initialize socket.\n");
			//}
		}
		else
		{
			//Make UDP Connection
			struct sockaddr_in Server_add;
		//	WORD wVersionRequested;
	//		WSADATA wsaData;
			// Start Winsock
	//		wVersionRequested = MAKEWORD( 1, 0 );
	//		if( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	//		{
				// Make Socket
				Server_add.sin_family      = AF_INET;
				Server_add.sin_port        = htons( port );
				Server_add.sin_addr.s_addr = inet_addr( exlusive );
				SOCKET Sock = socket( AF_INET, SOCK_DGRAM, 0 );

				char socke[80];
				itoa((int)Sock,socke,10);
				nproto->senddata("Socket Number: ");
				nproto->senddata(socke);
				nproto->senddata("\nWarning: We don't verify the status of host.\nPacket's will be sended in siege mode.\n");
	//		}
	//		else
	//		{
	//			nproto->senddata("Cant initialize sockets\n");
	//		}
		}
	}

}
void Cintep::prg_net_closesocket(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	nproto->senddata("Closesocket program\n\r");
	int sk=atoi(mx[xlogon].cmdline+15);

	/*
		WSANOTINITIALISED A successful WSAStartup call must occur before using this function. 
		WSAENETDOWN The network subsystem has failed. 
		WSAENOTSOCK The descriptor is not a socket. 
		WSAEINPROGRESS A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function. 
		WSAEINTR The (blocking) Windows Socket 1.1 call was canceled through WSACancelBlockingCall. 
		WSAEWOULDBLOCK The socket is marked as nonblocking and SO_LINGER is set to a nonzero time-out value. 
	*/
	BOOL okq=1;
	int rspw=closesocket(sk);
	if (rspw==WSANOTINITIALISED)
	{
		nproto->senddata("error closing... A successful WSAStartup call must occur before using this function.\n");
		okq=0;
	}
	if (rspw==WSAENETDOWN)
	{
		nproto->senddata("The network subsystem has failed.\n");
		okq=0;
	}
	if (rspw==WSAENOTSOCK)
	{
		nproto->senddata("The descriptor is not a socket.\n");
		okq=0;
	}
	if (rspw==WSAEINTR)
	{
		nproto->senddata("The (blocking) Windows Socket 1.1 call was canceled through WSACancelBlockingCall.\n");
		okq=0;
	}
	if (rspw==WSAEWOULDBLOCK)
	{
		nproto->senddata("The socket is marked as nonblocking and SO_LINGER is set to a nonzero time-out value.\n");
		okq=0;
	}
	if (rspw==WSAEINPROGRESS)
	{
		nproto->senddata("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.\n");
		okq=0;
	}
	if(okq)
	{
		if (rspw>=0)
			nproto->senddata("Socket Closed.\n");
		else
			nproto->senddata("Unknown error\n");
	}

}

void Cintep::prg_prompt(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,""))
	{
		nproto->sendline("PROMPT Internal Program v1.1");
		nproto->sendline("prompt [-h] prompt");
		nproto->sendline(" -h	show this help");
		nproto->sendline(" prmpt	new prompt to show (can be formatted)");
		nproto->sendline("example: prompt <$d:$u>");
	}
	else
	{
		//Change prompt of the program...
		data_g->PutURCSKey("Settings","server_prompt",mx[xlogon].cmdline);
		data_g->LoadData();
		nproto->sendline("Prompt changed...");
	}	
}


void Cintep::prg_kill(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//Kill a process..
	//Command: KILL

	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,""))
	{
		nproto->sendline("KILL Internal Program v1.1");
		nproto->sendline("kill [-h] pid");
		nproto->sendline(" -h	show this help");
		nproto->sendline(" pid	is number of process to kill");
		nproto->sendline("example: kill 1238");
	}
	else
	{
        unsigned long pid=atol(mx[xlogon].cmdline); //convert input to integer
		char exterm[COMMAND_LINE];
		_snprintf(exterm,sizeof(exterm),"Sending Termination Signal to: %05u   %-20s ",pid,"");
		nproto->setAction(exterm);
		if (fns->KillProcess(pid)) nproto->setActionStatus(true);
		else nproto->setActionStatus(false);
	}
}

void Cintep::prg_killall(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	nproto->senddata("KILLALL Internal Program\n");
	if (mx[xlogon].cmdline[8]=='-' && mx[xlogon].cmdline[9]=='h')
	{
		nproto->senddata("Usage:\nKillAll processname\nthe matchs (can be substrings) will be killed\n");
	}
	else
	{
		if (!strcmp(mx[xlogon].cmdline+8,"")) 
		{
			nproto->senddata("WARNING: Must select any filter\nusage \"killall -h\" to more help\n");
			return;
		}
		long r;
		HANDLE hSnapShot;
		PROCESSENTRY32 uProcess;
		hSnapShot = CreateToolhelp32Snapshot (TH32CS_SNAPALL, NULL);
		uProcess.dwSize= sizeof(uProcess);
		r = Process32First (hSnapShot, &uProcess );
		while (r)
		{
            char cmpare[COMMAND_LINE];
			char exterm[COMMAND_LINE];
			if (!data_g->nt)
			{
				LPSTR pCurChar;
				for (pCurChar = (uProcess.szExeFile + lstrlen (uProcess.szExeFile));
					*pCurChar != '\\' && pCurChar != uProcess.szExeFile; 
					--pCurChar)
				strncpy(cmpare,pCurChar,sizeof(cmpare));
			}
			else //WinNT! (names are in low format)
			{
				strncpy(cmpare,uProcess.szExeFile,sizeof(cmpare));
			}
			strupr(cmpare);
			strupr(mx[xlogon].cmdline);
			if (strstr(cmpare,mx[xlogon].cmdline+8)!=NULL)
			{
				_snprintf(exterm,sizeof(exterm),"Sending Termination Signal to: %05u - %-20s ",uProcess.th32ProcessID,cmpare);
				nproto->setAction(exterm);
				if (fns->KillProcess(uProcess.th32ProcessID)) nproto->setActionStatus(true);
				else nproto->setActionStatus(false);
			}
			r = Process32Next(hSnapShot, &uProcess );
		}
	}

}

void Cintep::prg_rmuser(con_v mx[SERVER_CONNECTIONS], int xlogon) //deletes user
{
	passed=TRUE;
	// rmuser name
    fns->denter(mx[xlogon].cmdline); //filter username

	char keydir[1024];
	_snprintf(keydir,sizeof(keydir),"Users\\%s",mx[xlogon].cmdline+6);
	data_g->PutURCSKey(keydir, "level","none");
	nproto->senddata("rmuser name - Remove user utility... excecuted\n\r"); //inform
}

void Cintep::prg_cmd(con_v mx[SERVER_CONNECTIONS], int xlogon)  //Changes User information
{
	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,""))
	{
		nproto->sendline("WARNING: CMD MAY HANG URCS CONNECTION");
		nproto->sendline("cmd [-h] command");
		nproto->sendline(" -h	show this help");
		nproto->sendline(" command command to run");
		nproto->sendline("example: cmd nslookup -type=MX hotmail.com");
	}
	else
	{
		ExecAndGetOutput(mx[xlogon].cmdline);
	}
}
void Cintep::prg_chuser(con_v mx[SERVER_CONNECTIONS], int xlogon)  //Changes User information
{
	passed=TRUE;
	char level[128];

	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	bool p=fns->getboolarg(mx[xlogon].cmdline,"p");
	strncpy(level,fns->getchararg(mx[xlogon].cmdline,"l"),128);
	fns->getzeroarg(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline,""))
	{
		nproto->sendline("chuser [-h] [-p] [-l level] user");
		nproto->sendline(" -h	show this help");
		nproto->sendline(" -p change password");
		nproto->sendline(" -l change level");
		nproto->sendline(" user username");
		nproto->sendline("example: chuser -l admin route");
	}
	else
	{
		//Changes user information.
		if (p)
		{
			//Preguntar el pass:...
			char pass[512];
			nproto->senddata("password: ");
			nproto->getdnpass(pass,512);
			fns->denter(pass);
			//Cambiar el pass:
			char keydir[1024];
			_snprintf(keydir,sizeof(keydir),"Users\\%s",mx[xlogon].cmdline);
			if(data_g->PutURCSKey(keydir, "password",fns->md5sum(pass)))
				nproto->sendline("\nPassword changed succesfully...");
			else
				nproto->sendline("\nPassword not changed...");
		}
		if (strcmp(level,""))
		{
			//Cambiar el pass:
			char keydir[1024];
			_snprintf(keydir,sizeof(keydir),"Users\\%s",mx[xlogon].cmdline);
			if(data_g->PutURCSKey(keydir, TEXT("level"),level))
				nproto->sendline("Level changed succesfully...");
			else
				nproto->sendline("Level not changed...");
		}

	}
}


void Cintep::prg_mklevel(con_v mx[SERVER_CONNECTIONS], int xlogon) //make new level
{
	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,""))
	{
		nproto->sendline("mklevel [-h] level");
		nproto->sendline(" -h	show this help");
		nproto->sendline("example: mklevel shutdown");
	}
	else
	{
		char strtp[COMMAND_LINE];
		if(data_g->GetURCSKey("Groups",mx[xlogon].cmdline,strtp,sizeof(strtp)) || !strcmp(mx[xlogon].cmdline,"admin"))
		{
			nproto->sendline("This level already exist. Nothing to do.");
		}
		else
		{
			if(data_g->PutURCSKey("Groups", mx[xlogon].cmdline,","))
				nproto->sendline("Level created succesfully...");
			else
				nproto->sendline("Level not created...");
		}
	}
}

void Cintep::prg_rmlevel(con_v mx[SERVER_CONNECTIONS], int xlogon) //remove level
{
	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp("",mx[xlogon].cmdline))
	{
		nproto->sendline("rmlevel [-h] level");
		nproto->sendline(" -h	show this help");
		nproto->sendline("example: rmlevel shutdown");
	}
	else
	{
		char strtp[COMMAND_LINE];
		if(data_g->GetURCSKey("Groups",mx[xlogon].cmdline,strtp,sizeof(strtp)))
		{
			if (data_g->DelURCSKey("Groups",mx[xlogon].cmdline))
				nproto->sendline("succesfully removed...");	
			else
				nproto->sendline("Error removing level...");	
		}
		else			nproto->sendline("Error removing level...");	
	}
}
void Cintep::prg_lslevel(con_v mx[SERVER_CONNECTIONS], int xlogon) //List level
{
	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp("",mx[xlogon].cmdline))
	{
		nproto->sendline("lslevel [-h] level");
		nproto->sendline(" -h	show this help");
		nproto->sendline("example: lslevel shutdown");
	}
	else
	{

		char strtp[COMMAND_LINE];
		if(data_g->GetURCSKey("Groups",mx[xlogon].cmdline,strtp,sizeof(strtp)))
		{
			char *token;
			token = strtok( strtp, "," );
			while( token != NULL )
			{
				nproto->sendline(token);
				token = strtok( NULL, "," );
			}
		}
		else nproto->sendline("Error level not founded...");	


	}
}
void Cintep::prg_chlevel(con_v mx[SERVER_CONNECTIONS], int xlogon) //change level commands.
{
	passed=TRUE;
	char command_add[128],command_sub[128],level_add[128];
	
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	bool brespda=fns->getboolarg(mx[xlogon].cmdline,"da");
	
	strncpy(command_add,fns->getchararg(mx[xlogon].cmdline,"c"),128);
	strncpy(command_sub,fns->getchararg(mx[xlogon].cmdline,"d"),128);
	strncpy(level_add,fns->getchararg(mx[xlogon].cmdline,"a"),128);
	strupr(command_add);
	strupr(command_sub);


	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp("",mx[xlogon].cmdline))
	{
		nproto->sendline("chlevel [-h] [-a level] [-c command] [-d command] [-da] level");
		nproto->sendline(" -h	show this help");
		nproto->sendline(" -a 	add all commands from other level/group (not admin)");
		nproto->sendline(" -c 	add specific command to this level");
		nproto->sendline(" -d 	deletes specific command from this level");
		nproto->sendline(" -da 	deletes all commands from this level");
		nproto->sendline(" level	level to change");
		nproto->sendline("example: chlevel -c shutdown shutdown");
	}
	else
	{
		char strtpb[COMMAND_LINE];
		if (data_g->GetURCSKey("Groups",mx[xlogon].cmdline,strtpb,sizeof(strtpb)))
		{
			if (strcmp("",level_add)) //add level to this level
			{
				char strtpa[COMMAND_LINE];
				if(data_g->GetURCSKey("Groups",level_add,strtpa,sizeof(strtpa)))
				{
					//adding level.
					char newlevel[COMMAND_LINE];
					_snprintf(newlevel,sizeof(newlevel),"%s%s",strtpa,strtpb+1);
					if(data_g->PutURCSKey("Groups", mx[xlogon].cmdline,newlevel))
						nproto->sendline("Level modified succesfully, level attached...");
					else
						nproto->sendline("Level not modified...");
				}
				else nproto->sendline("Error level to add not founded...");	
			}
			if (strcmp("",command_add)) //add command to this level
			{
				//adding command to level.
				char newlevel[COMMAND_LINE];
				_snprintf(newlevel,sizeof(newlevel),",%s%s",command_add,strtpb);
				if(data_g->PutURCSKey("Groups", mx[xlogon].cmdline,newlevel))
					nproto->sendline("Level modified succesfully, command added...");
				else
					nproto->sendline("Level not modified...");
			}
			if (strcmp("",command_sub)) //remove command from this level
			{
				//Search for command sub...
				char newlevel[COMMAND_LINE];
				strcpy(newlevel,",");
				char *token;
				token = strtok( strtpb, "," );
				while( token != NULL )
				{
					if (strcmp(token,command_sub))
					{
						strncat(newlevel,token,sizeof(newlevel)-strlen(newlevel));
						strncat(newlevel,",",sizeof(newlevel)-strlen(newlevel));
					}
					token = strtok( NULL, "," );
				}
				if(data_g->PutURCSKey("Groups", mx[xlogon].cmdline,newlevel))
					nproto->sendline("Level modified succesfully, command substracted...");
				else
					nproto->sendline("Level not modified...");
			}
			if (brespda)
			{
				if(data_g->PutURCSKey("Groups", mx[xlogon].cmdline,","))
					nproto->sendline("Level modified succesfully...");
				else
					nproto->sendline("Level not modified...");
			}
		}
		else nproto->sendline("Error level not founded...");	
	}
}

void Cintep::prg_mkuser(con_v mx[SERVER_CONNECTIONS], int xlogon) //make user
{
	passed=TRUE;
    // mkuser will be executed now...
	// format: mkuser -l [level] -p [password] name
	// if not password, will request this data
	// if not level, admin mode is taked.
	
	char respp[256];
	char respl[128];
//	char resph[12];

	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	strncpy(respp,fns->getchararg(mx[xlogon].cmdline,"p"),256);
	strncpy(respl,fns->getchararg(mx[xlogon].cmdline,"l"),128);

	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,""))
		nproto->senddata("mkuser -l [level] -p [password] name\n");
	else
	{
		char u_level[128]="admin";
		char u_pass[COMMAND_LINE];
		char u_name[128];
        //Verify for existence of level...
		if (strcmp(respl,"")) // level defined...
			strncpy(u_level,respl,sizeof(u_level));
		if (!strcmp(respp,"")) // password not defined...
		{
			nproto->senddata("Insert password: "); //prompt for password
			nproto->getdnpass(u_pass,COMMAND_LINE); //get password
			fns->denter(u_pass); //filter.
		}
		else
		{
			strncpy(u_pass,respp,sizeof(u_pass));	//copy from command line
			fns->denter(u_pass);		//filter
		}
		strncpy(u_name,mx[xlogon].cmdline,128);

		char keydir[1024];
		_snprintf(keydir,sizeof(keydir),"Users\\%s",u_name);
		if(data_g->PutURCSKey(keydir, TEXT("password"),fns->md5sum(u_pass)))
			nproto->sendline("\nPassword changed...");
		else
			nproto->sendline("\nCant change account password...");
		if(data_g->PutURCSKey(keydir, TEXT("level"),u_level))
			nproto->sendline("level changed...");
		else
			nproto->sendline("Cant change account level...");
	}
}

void Cintep::prg_passwd(con_v mx[SERVER_CONNECTIONS], int xlogon) //change password
{
	passed=TRUE;
	//Change User's password
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	
	if (bresph)
	{
		nproto->sendline("Usage: passwd [password]");
		nproto->sendline("Example: passwd 13245678");
	}
	else
	{
		if (!strcmp(mx[xlogon].cmdline,"")) // prompt for password if not specified
		{
			nproto->senddata("Insert password: ");
			nproto->getdnpass(mx[xlogon].cmdline,COMMAND_LINE);
			fns->denter(mx[xlogon].cmdline); //filter's password
		}
		/*do change.*/
		char keydir[1024];
		_snprintf(keydir,sizeof(keydir),"Users\\%s",mx[xlogon].c_User);
		if(data_g->PutURCSKey(keydir, "password",fns->md5sum(mx[xlogon].cmdline)))
			nproto->senddata("\nPassword changed...\n");
		else
			nproto->senddata("\nPassword not changed...\n");
	}
}

void Cintep::prg_who(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	char exterm[COMMAND_LINE]; 
	nproto->senddata("  UAID SOCKid            Username:        IP Address:           Conected since:\n");
	for(int n=0;n<SERVER_CONNECTIONS;n++)
	{
		if (mx[n].busy==TRUE)
		{
			_snprintf(exterm,sizeof(exterm)," %5u  %5u   %18s  %17s  %s",n,mx[n].socket,mx[n].c_User, mx[n].ip_from, ctime(&mx[n].since));
			nproto->senddata(exterm);
		}
	}
}

void Cintep::prg_cat(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	char respt[12]="";
	char resph[12]="";

	strncpy(respt,fns->deparg(mx[xlogon].cmdline,"-t",TRUE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
	{
		nproto->senddata("Format: cat [-t] [-h] file\n\r-t Complete 10,13 bytes\n\r-h help\n\r");
		return;
	}
	fns->denter(mx[xlogon].cmdline);
	char buffer[256];
	memset(&buffer,0,256);
	int fh;
	unsigned int nbytes = 256, bytesread;
	if( (fh = _open(mx[xlogon].cmdline+4 , _O_RDONLY )) == -1 )
	{
		nproto->senddata("ERROR: Cannot open local file on telnet server...\n");
	}
	else
	{
		while( ( bytesread = _read( fh, buffer, nbytes ) ) > 0 && fh!=-1 )
		{
			buffer[bytesread]=0;
			if (strcmp(respt,"-t"))	fns->renter(buffer,sizeof(buffer));
			nproto->senddata(buffer);
			memset(&buffer,0,256);
		}
		_close( fh );
	}
}

void Cintep::prg_cd(con_v mx[SERVER_CONNECTIONS], int xlogon) //Change Directory.
{
	passed=TRUE;
	char recpt2[_MAX_PATH];
	char buff[_MAX_PATH];

	strncpy(recpt2,mx[xlogon].cmdline+3, _MAX_PATH-3); 
	fns->denter(recpt2); 

	if (_chdir(recpt2)==-1) //Change directory...
		nproto->senddata("Can't locate specified directory\n\r"); //error 

	_getcwd( buff, _MAX_PATH );   /* Get the current working directory: */
	strncpy(mx[xlogon].localdir,buff, _MAX_PATH); //copy it to "user specifications"...
}
void Cintep::prg_ps(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	nproto->senddata("PSLIST Internal Program 1.0\n\r");
	if (mx[xlogon].cmdline[3]=='-' && mx[xlogon].cmdline[4]=='h')
	{
		nproto->senddata("#PS is a internal program who show all aplications\n\rpid's and ppids (process id) & (parent Process ID)\n\rYou can also use: KILL (Terminate program by pid)\n\r");
	}
	else
	{
		nproto->senddata("\n\r   PPID  PID   APPNAME\n\r");
		long r;
		HANDLE hSnapShot;
		PROCESSENTRY32 uProcess;
		hSnapShot = CreateToolhelp32Snapshot (TH32CS_SNAPALL, NULL);
		uProcess.dwSize= sizeof(uProcess);
		r = Process32First (hSnapShot, &uProcess );
		while (r)
		{
			char exterm[COMMAND_LINE];
			if (!data_g->nt)
			{
	            char cmpare[COMMAND_LINE];
				LPSTR pCurChar;
				for (pCurChar = (uProcess.szExeFile + lstrlen (uProcess.szExeFile));
					*pCurChar != '\\' && pCurChar != uProcess.szExeFile; 
					--pCurChar)
				strncpy(cmpare,pCurChar,sizeof(cmpare));
				_snprintf(exterm,sizeof(exterm)," %5u %5u   %s\n\r",uProcess.th32ParentProcessID,uProcess.th32ProcessID,cmpare);
			}
			else //WinNT! (names are in low format)
			{
				_snprintf(exterm,sizeof(exterm)," %5u %5u   %s\n\r",uProcess.th32ParentProcessID,uProcess.th32ProcessID,uProcess.szExeFile );
			}
			nproto->senddata(exterm);
			r = Process32Next(hSnapShot, &uProcess );
		}
	}

}
void Cintep::prg_rmdir(con_v mx[SERVER_CONNECTIONS], int xlogon) //Remove an directory
{
	passed=TRUE;
	char resph[12]="";
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);
	if (strcmp(resph,"-h"))
		nproto->senddata("Format: rmdir [-h] directory (can be formatted such as: dir$snext\n\r");
	else
	{
		fns->denter(mx[xlogon].cmdline);
			
		if( RemoveDirectory( fns->convert(mx[xlogon].cmdline+6,mx,xlogon) ) )
			nproto->senddata("Dir. removed\n");
		else
			nproto->senddata( "Could not removes directory\n");
	}
}
void Cintep::prg_mkdir(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	char resph[12]="";
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);
	if (strcmp(resph,"-h"))
		nproto->senddata("Format: mkdir [-h] directory (can be formatted such as: dir$snext\n\r");
	else
	{
		fns->denter(mx[xlogon].cmdline);
			
		if( CreateDirectory( fns->convert(mx[xlogon].cmdline+6,mx,xlogon),NULL ) )
			nproto->senddata("Created.\n\r");
		else
			nproto->senddata( "Could not create directory\n");
	}
}
void Cintep::prg_rm(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	bool brespy=fns->getboolarg(mx[xlogon].cmdline,"y");
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	
	if (bresph || !strcmp(mx[xlogon].cmdline,""))
	{
		nproto->sendline("RM Internal Command v1.1");
		nproto->sendline("Remove file");
		nproto->sendline("rm [-y] [-h] file");
		nproto->sendline(" -y		dont ask for delete");
		nproto->sendline(" -h		Show this help");
		nproto->sendline("Note: Compatible with formmated data such as: Hola$sComo$sEstas$n");
	}
	else
	{
		bool bOK=true; 
		if (!brespy)
		{
			char msw[20];
			nproto->senddata("Press N to cancel delete file operation: ");
			nproto->getdnline(msw,20);
			strupr(msw);
			if (msw[0]=='N')	bOK=false;
		}
		nproto->setAction("Deleting...");
		if (bOK)
		{
			char cvt[COMMAND_LINE];
			strncpy(cvt,fns->convert(mx[xlogon].cmdline,mx, xlogon),sizeof(cvt));
			if( DeleteFile( cvt ) == 0)	nproto->setActionStatus(false);
			else  						nproto->setActionStatus(true);
		}
		else nproto->setActionStatus(false);
	}
}

void Cintep::prg_cp(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//Copy file
	passed=TRUE;
	char respd[COMMAND_LINE];
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	bool brespo=fns->getboolarg(mx[xlogon].cmdline,"o");
	strncpy(respd,fns->getchararg(mx[xlogon].cmdline,"d"),COMMAND_LINE);
	fns->getzeroarg(mx[xlogon].cmdline);
	if (!strcmp(mx[xlogon].cmdline,"") || bresph || !strcmp(respd,""))
	{
		nproto->sendline("CP Internal Command v1.1");
		nproto->sendline("Copy file");
		nproto->sendline("cp source [-o] -d destination");
		nproto->sendline(" source	Source filename");
		nproto->sendline(" -o		Overwrite");
		nproto->sendline(" -d		Destination filename.");
		nproto->sendline("Note: Compatible with formmated data such as: Hola$sComo$sEstas$n");
	}
	else
	{
		//Do operations...
		char csa[COMMAND_LINE];
		char src[COMMAND_LINE];
		char dst[COMMAND_LINE];

		strncpy(dst,fns->convert(respd,mx, xlogon),COMMAND_LINE); //convert destination
		strncpy(src,fns->convert(mx[xlogon].cmdline,mx,xlogon),COMMAND_LINE); //convert source
		
		_snprintf(csa,sizeof(csa),"Copying %s to %s...",src,dst);
		nproto->setAction(csa);

		if (CopyFile(src,dst,!brespo))
			nproto->setActionStatus(true);
		else
			nproto->setActionStatus(false);
	}
}

void Cintep::prg_mv(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//Copy file
	passed=TRUE;

	char respd[COMMAND_LINE];
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	bool brespo=fns->getboolarg(mx[xlogon].cmdline,"o");
	strncpy(respd,fns->getchararg(mx[xlogon].cmdline,"d"),COMMAND_LINE);
	fns->getzeroarg(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline,"") || bresph || !strcmp(respd,""))
	{
		nproto->sendline("MV Internal Command v1.1");
		nproto->sendline("Move file");
		nproto->sendline("mv source [-o] -d destination");
		nproto->sendline(" source	Source filename");
		nproto->sendline(" -o		Overwrite");
		nproto->sendline(" -d		Destination filename.");
		nproto->sendline("Note: Compatible with formmated data such as: Hola$sComo$sEstas$n");
	}
	else
	{
		//Do operations...
		char csa[COMMAND_LINE];
		char src[COMMAND_LINE];
		char dst[COMMAND_LINE];

		strncpy(dst,fns->convert(respd,mx, xlogon),COMMAND_LINE); //convert destination
		strncpy(src,fns->convert(mx[xlogon].cmdline,mx, xlogon),COMMAND_LINE); //convert source
		
		_snprintf(csa,sizeof(csa),"Moving %s to %s...",src,dst);
		nproto->setAction(csa);
	
		if (brespo) 
		{
			if (MoveFileEx(src,dst,MOVEFILE_REPLACE_EXISTING))	nproto->setActionStatus(true);
			else												nproto->setActionStatus(false);
		}
		else
		{
			if (MoveFile(src,dst))	nproto->setActionStatus(true);
			else					nproto->setActionStatus(false);
		}
	}
}

void Cintep::prg_exec(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	// Format: winexec [-e Exec_Number] path
	char respe[COMMAND_LINE];
	char resph[12];
	int execution=SW_SHOWDEFAULT;
	strncpy(respe,fns->deparg(mx[xlogon].cmdline,"-e",FALSE),COMMAND_LINE); 
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
	{
		//showhelp
		char comande[COMMAND_LINE];
		_snprintf(comande,sizeof(comande),"%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n",
		SW_HIDE, "SW_HIDE",
		SW_SHOWNORMAL, "SW_SHOWNORMAL",
		SW_SHOWMINIMIZED, "SW_SHOWMINIMIZED",
		SW_SHOWMAXIMIZED, "SW_SHOWMAXIMIZED",
		SW_MAXIMIZE, "SW_MAXIMIZE",
		SW_SHOWNOACTIVATE, "SW_SHOWNOACTIVATE",
		SW_SHOW, "SW_SHOW",
		SW_MINIMIZE, "SW_MINIMIZE",
		SW_SHOWMINNOACTIVE, "SW_SHOWMINNOACTIVE",
		SW_SHOWNA, "SW_SHOWNA",
		SW_RESTORE, "SW_RESTORE",
		SW_SHOWDEFAULT,"SW_SHOWDEFAULT" ,
		SW_FORCEMINIMIZE, "SW_FORCEMINIMIZE"
		);
		nproto->senddata("Format: winexec [-e Exec_Number] path\n\rExcecute an program file\n\rExec Number:\n\r");
		nproto->senddata(comande);
	}
	else
	{
		char lcs[COMMAND_LINE];
		if (strcmp(respe,"-e"))
		{
			//respe founded... 
			execution = atoi(respe);
		}
		fns->denter(mx[xlogon].cmdline);
		_snprintf(lcs,sizeof(lcs),"Excecuting: %s \n",fns->convert(mx[xlogon].cmdline+8,mx, xlogon));
		nproto->senddata(lcs);
		int x=WinExec(fns->convert(mx[xlogon].cmdline+8,mx, xlogon),execution);
		if (x==0)
			nproto->senddata("The system is out of memory or resources.\n\r");
		else
		{
			if (x==ERROR_BAD_FORMAT)
				nproto->senddata("The .EXE file is invalid (non-Win32 .EXE or error in .EXE image).\n\r");
			else
			{
				if (x==ERROR_FILE_NOT_FOUND) 
					nproto->senddata("The specified file was not found.\n\r");
				else
				{
					if (x==ERROR_PATH_NOT_FOUND) 
						nproto->senddata("The specified path was not found.\n\r");
					else
						nproto->senddata("Execution Completed\n\r");
				}
			}
		}
	}
}
void Cintep::prg_touch(con_v mx[SERVER_CONNECTIONS], int xlogon) //creates empty files
{
	passed=TRUE;
	// Format: touch [-h] [-v] filename
	// file manipulation errors are saved to: cpu.b_a
	bool bresph = fns->getboolarg(mx[xlogon].cmdline,"-h");
	bool brespv = fns->getboolarg(mx[xlogon].cmdline,"-h");
	fns->getzeroarg(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline,""))
	{	//showhelp
		nproto->senddata("Format:  touch [-h] [-v] filename\nCreates empty file\n-v Verbose mode.\n");
	}
	else
	{
		if (brespv) nproto->senddata("Creating file: ");
		FILE *fw;
		char pathoffile[_MAX_PATH];
		strncpy(pathoffile,fns->convert(mx[xlogon].cmdline,mx,xlogon),sizeof(pathoffile));
		if(!strcmp(pathoffile,""))
		{
			mx[xlogon].cpu.b_a=0; //not created
			if (brespv) nproto->senddata("Error: Enter a file-name\n");
			return; 
		}
		if (( fw = fopen( pathoffile, "a+"))==NULL)
		{
			mx[xlogon].cpu.b_a=0; //not created
			if (brespv) nproto->senddata("Cannot open file\n");
		}
		else
		{
			mx[xlogon].cpu.b_a=1; //not created
			if (brespv) nproto->senddata("Created.\n");
			fclose(fw);
		} //end of oppened

	} //end of no-help

}
void Cintep::prg_fopen(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	// Format: fopen [-m mode] path
	// default mode is reading mode.
	char respm[COMMAND_LINE];
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	strncpy(respm,fns->getchararg(mx[xlogon].cmdline,"m"),COMMAND_LINE); 
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph)
	{
		//showhelp
		nproto->senddata("Format: fopen [-m mode] path\n	Opens an file, default_ is reading\nModes:\nr  reading mode. If the file does not exist or cannot be found, the fopen call fails.\nw  writing mode. If the given file exists, its contents are destroyed.\na  appending without removing the EOF marker before writing _new data to the file; creates the file first _if it doesn't exist.\nr+ both reading and writing mode. The file must exist. \nw+ both reading and writing mode. If the given file exists, its contents are destroyed. \na+ reading and appending mode; the appending operation includes the removal of the EOF marker.\nAnother's modes can be used.\n");
	}
	else
	{
		char lcs[COMMAND_LINE];
		if (!strcmp(respm,"")) strcpy(respm,"r"); //using default: read file
		fns->denter(mx[xlogon].cmdline);
		_snprintf(lcs,sizeof(lcs),"Openning: %s \n",fns->convert(mx[xlogon].cmdline,mx, xlogon));
		nproto->senddata(lcs);
		FILE *stream;
		char pathoffile[_MAX_PATH];
		strncpy(pathoffile,fns->convert(mx[xlogon].cmdline,mx, xlogon),_MAX_PATH);
		if(!strcmp(pathoffile,""))
		{
			nproto->senddata("Enter a file-name\n");
			return; 
		}
		if (( stream = fopen( pathoffile, respm))==NULL)
			nproto->senddata("Cannot open file\n");
		else
		{
			nproto->senddata("The file has been openned, attaching to Users file's handler\n");
			//Search for file handler.
			int freehandler=-1;
			for (int y=0; y<FILE_SLOTS;y++) //search some free handler...
				if (mx[xlogon].files[y]==0)
					freehandler=y;
			if (freehandler==-1)
			{
				nproto->senddata("No handlers availables, closing file\n");
				fclose(stream);
            }
			else
			{
				//putting it to file handler and print.
				mx[xlogon].files[freehandler]=stream;
				char out_dt[COMMAND_LINE];
				_snprintf(out_dt,sizeof(out_dt),"Handler #: %d\n",freehandler);
				nproto->senddata(out_dt);
			}
		} //end of oppened

	} //end of no-help

}

void Cintep::prg_fputc(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	// fputc [-h] [-d] data(integer mode)  file_handle.

	passed=TRUE;
	char respd[12];
	char resph[12];

	BOOL bresph=0;

	strncpy(respd,fns->deparg(mx[xlogon].cmdline,"-d",FALSE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;
	fns->denter(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline+6,""))
		nproto->senddata("fputc [-h] [-d] file handler\n-h help\n-d Data integer.\n");
	else
	{
		//Do operations...
		if (!strcmp(respd,"-f"))
		{
			nproto->senddata("not data specified.\n");
			return;
		}
		int hand=atoi(mx[xlogon].cmdline+6); //point to handler file...
		if (hand>FILE_SLOTS) nproto->senddata("Your value is out of range.\n");
		else
		{
			if(mx[xlogon].files[hand]==0) nproto->senddata("Not opened file at this handler\n");
			else
			{
				//ready for write.
				if(fputc(atoi(respd),mx[xlogon].files[hand])==EOF) nproto->senddata("Error.\n");
				else nproto->senddata("Writed.\n");
   			}
		}
	}
}

void Cintep::prg_fgetc(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//get char from file.
	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph)
		nproto->senddata("fgetc handler\n");
	else
	{
		//Do operations...
		int hand=atoi(mx[xlogon].cmdline); //point to handler file...
		if (hand>FILE_SLOTS) nproto->senddata("Your value is out of range.\n");
		else
		{
			if(mx[xlogon].files[hand]==0) nproto->senddata("Not opened file at this handler\n");
			else
			{
				int mfs=fgetc(mx[xlogon].files[hand]);
				if (mfs!=EOF)
				{
					//data getted. 
					char ary[80];
					_snprintf(ary,sizeof(ary),"Normal: %01c  Hexadecimal: %02X  Decimal: %03d\n",mfs,mfs,mfs);
					nproto->senddata(ary); //sending...
				}
				else 
					nproto->senddata("#EOF#\n");
			}
		}
	}

}
void Cintep::prg_fgets(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//get line from file.
	passed=TRUE;

	char respr[12];
	char resph[12];
	BOOL brespr=0;
	BOOL bresph=0;
	strncpy(respr,fns->deparg(mx[xlogon].cmdline,"-r",TRUE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(respr,"-r"))
		brespr=1;
	if (strcmp(resph,"-h"))
		bresph=1;
	fns->denter(mx[xlogon].cmdline);

	if (bresph)
		nproto->senddata("fgets [-r] handler\n-r Show in hexadecimal\n");
	else
	{
		//Do operations...
		int hand=atoi(mx[xlogon].cmdline+6); //point to handler file...
		if (hand>FILE_SLOTS) nproto->senddata("Your value is out of range.\n");
		else
		{
			if(mx[xlogon].files[hand]==0) nproto->senddata("Not opened file at this handler\n");
			else
			{
				char mfs[COMMAND_LINE];
				if (fgets(mfs,COMMAND_LINE,mx[xlogon].files[hand])!=NULL)
				{
					//data getted. nproto->senddata(tempget); //sending...
					int r=(int)strlen(mfs);
					if (!brespr)
						nproto->senddata(mfs);
					else
						nproto->senddatahex(mfs,r);
				}
				else 
					nproto->senddata("#EOF#\n");
			}
		}
	}

}
void Cintep::prg_fputs(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	// fputs [-h] [-f] data  
	passed=TRUE;
	char respf[24];
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	strncpy(respf,fns->getchararg(mx[xlogon].cmdline,"f"),24);
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,""))
		nproto->senddata("fputs [-h] [-f handler] data\n-h help\n-f File handler... obligaroty\n");
	else
	{
		if (!strcmp(respf,""))
		{
			nproto->senddata("not file specified.\n");
			return;
		}
		//Do operations...
		int hand=atoi(respf); //point to handler file...
		if (hand>FILE_SLOTS) nproto->senddata("Your value is out of range.\n");
		else
		{
			if(mx[xlogon].files[hand]==0) nproto->senddata("Not opened file at this handler\n");
			else
			{
				//ready for write.
				char datatoput[COMMAND_LINE];
				char dtaln[128];
				strncpy(datatoput,fns->convert(mx[xlogon].cmdline,mx,xlogon),COMMAND_LINE);
				_snprintf(dtaln,sizeof(dtaln),"Writed: %u characters\n",(unsigned int)strlen(datatoput));
				if(fputs(datatoput,mx[xlogon].files[hand])<0) nproto->senddata("Error. cannot write\n");
				else nproto->senddata(dtaln);
   			}
		}
	}
}
void Cintep::prg_fclose(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//close handler.
	passed=TRUE;
	
	fns->getzeroarg(mx[xlogon].cmdline);
	int hand=atoi(mx[xlogon].cmdline);
	nproto->senddata("file closer: fclose [handler]\n");
	if (hand>FILE_SLOTS) nproto->senddata("Your value is out of range.\n");
	else
	{
		if(mx[xlogon].files[hand]==0) nproto->senddata("Not opened file at this handler\n");
		else
		{
			if (fclose(mx[xlogon].files[hand])==0) 
			{
				nproto->senddata("File closed, and handler released\n");
				mx[xlogon].files[hand]=0;
			}
			else
				nproto->senddata("Unknown error closing file. Not Closed.\n");
		}
	}
}


void Cintep::prg_shutdown(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	// Format: shutdown [-e Number_of_Extra_Args] MODE_Number
	char respe[COMMAND_LINE];
	UINT nexecution=EWX_SHUTDOWN;
	DWORD eexecution=0;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	strncpy(respe,fns->getchararg(mx[xlogon].cmdline,"e"),COMMAND_LINE); 
	fns->getzeroarg(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline,""))
	{
		//showhelp
		char cmde[COMMAND_LINE];
		char cmdf[COMMAND_LINE];
		_snprintf(cmde,sizeof(cmde),"%2d %s\n%2d %s\n%2d %s\n%2d %s\n",
		EWX_LOGOFF,"EWX_LOGOFF",
		EWX_POWEROFF,"EWX_POWEROFF",
		EWX_REBOOT,"EWX_REBOOT",
		EWX_SHUTDOWN,"EWX_SHUTDOWN");

		_snprintf(cmdf,sizeof(cmdf),"%2d %s\n%2d %s\n",
		EWX_FORCE, "EWX_FORCE",
		EWX_FORCEIFHUNG, "EWX_FORCEIFHUNG");

		nproto->senddata("Format: shutdown [-e Number_of_Extra_Args] MODE_Number\nShutDown Windows\nMODE_NUMBER:\n");
		nproto->senddata(cmde);
		nproto->senddata("Number_of_Extra_Args:\n");
		nproto->senddata(cmdf);
	}
	else
	{
		if (strcmp(respe,""))
			eexecution = atoi(respe); //respe founded... 

		nexecution = atoi(mx[xlogon].cmdline);
		nproto->senddata("Sending ShutDown Signal...\n");
      
		HANDLE hProcess=GetCurrentProcess();
		HANDLE token;
		OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&token);
		LUID priv;
		LookupPrivilegeValue("","SeShutdownPrivilege",&priv);
		TOKEN_PRIVILEGES tkp;
		TOKEN_PRIVILEGES oldTkp;
		tkp.PrivilegeCount=1;
		tkp.Privileges[0].Luid=priv;
		tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
		DWORD retLength;
		AdjustTokenPrivileges(token,FALSE,&tkp,sizeof(oldTkp),&oldTkp,&retLength);

		if (ExitWindowsEx(nexecution | eexecution,NULL)) nproto->senddata("Succefully Sended\n");
		else nproto->senddata("Error sending shutdown signal\n");
	}
}


void Cintep::prg_ls(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: LS - List files
{
	passed=TRUE;
	//List directory:
	//options:  ls=ls *
	//-d  //directorios
	//-l  //list details
	//-c  //All data of file
	//-h  //Help

	//	putfile(dir);
	//	char filez[COMMAND_LINE];
	char exlusive[COMMAND_LINE];
	bool brespd=fns->getboolarg(mx[xlogon].cmdline,"d");
	bool brespl=fns->getboolarg(mx[xlogon].cmdline,"l");
	bool brespc=fns->getboolarg(mx[xlogon].cmdline,"c");
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");

	fns->getzeroarg(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline,""))
		strcpy(exlusive,"*");
	else
		strncpy(exlusive,mx[xlogon].cmdline,COMMAND_LINE);

	if (bresph)
	{
		nproto->senddata("LS INTERNAL UTILITY\n\r-l list mode\n\r-h help\n\r-c Show all data\n\r-d Shows directory\n\r");
	}
	else
	{
		//Exlusive contain a comodin path
		WIN32_FIND_DATA ffd; // file information struct
		HANDLE sh = FindFirstFile(exlusive, &ffd);
		if(INVALID_HANDLE_VALUE == sh)
		{
			nproto->senddata("LS: not a proper path i guess\n");
			return; // not a proper path i guess
		}
		do 
		{
			//directory?
			if (brespd && (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			{
				char datafile[1500];
				time_t fct=fns->TimetFromFt(ffd.ftCreationTime);
				time_t flat=fns->TimetFromFt(ffd.ftLastAccessTime);
				time_t flwt=fns->TimetFromFt(ffd.ftLastWriteTime);
	
				//prefix:
				if (brespc)
					nproto->senddata("------------------------------------------------------------------------------\nFileName             : ");
				else if (brespl)
				{
						char ed[80];
						strncpy(ed,ctime(&flwt),sizeof(ed));
						fns->denter(ed);
						_snprintf(datafile,sizeof(datafile),"%25s %7u %03X ",ed,ffd.nFileSizeLow,ffd.dwFileAttributes,ffd.cFileName);
						nproto->senddata(datafile);
				}
				//get extension
				char ext[12]="";
				strncpy(ext,fns->extview(ffd.cFileName),12);
				strupr(ext);
				//select color...
				if (!strcmp(ext,"EXE") || !strcmp(ext,"COM") || !strcmp(ext,"BAT") || !strcmp(ext,"VBS") ||!strcmp(ext,"EXE") || !strcmp(ext,"CMD")|| !strcmp(ext,"JAR")|| !strcmp(ext,"WSH")|| !strcmp(ext,"JSE") || !strcmp(ext,"JS") || !strcmp(ext,"PIF") || !strcmp(ext,"VBE") || !strcmp(ext,"WSF"))
					mx[xlogon].cpu.i_a=nproto->setcolor(LIGHTGREEN);//pintar como verde
				if (!strcmp(ext,"BAT") || !strcmp(ext,"VBS") || !strcmp(ext,"CMD")|| !strcmp(ext,"WSH")|| !strcmp(ext,"JSE") || !strcmp(ext,"JS") || !strcmp(ext,"VBE") || !strcmp(ext,"JAR")|| !strcmp(ext,"WSF"))
					mx[xlogon].cpu.i_a=nproto->setcolor(GREEN);//pintar como verde
				if (!strcmp(ext,"ZIP") || !strcmp(ext,"RAR") || !strcmp(ext,"ARJ") || !strcmp(ext,"TAR") || !strcmp(ext,"TGZ") ||
					!strcmp(ext,"BAK") || !strcmp(ext,"MSI") || !strcmp(ext,"PAK") )
					mx[xlogon].cpu.i_a=nproto->setcolor(RED);//pintar como rojo
				if (!strcmp(ext,"JPG") || !strcmp(ext,"ICO") || !strcmp(ext,"JPE") || !strcmp(ext,"BMP") || !strcmp(ext,"PNG") || !strcmp(ext,"GIF") || !strcmp(ext,"JPEG") || !strcmp(ext,"PSD") )
					mx[xlogon].cpu.i_a=nproto->setcolor(LIGHTMAGENTA);//pintar como magenta brillante
				if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
					mx[xlogon].cpu.i_a=nproto->setcolor(LIGHTBLUE);//set colour to blue.
				// print filename.
				nproto->senddata(ffd.cFileName);
				nproto->senddata("\n");
				//return to normal color.
				mx[xlogon].cpu.i_a=nproto->setdefaultcolor();
				// print sufix
				if (brespc)
				{
					//sufix of brespc
					_snprintf(datafile,sizeof(datafile),"File Alternate name  : %s\nFile Size High       : %u\nFile Size Low        : %u\nFile Creation time   : %sFile Last Access time: %sFile Last Write time : %s",
					ffd.cAlternateFileName,	ffd.nFileSizeHigh,
					ffd.nFileSizeLow, ctime(&fct),	ctime(&flat), ctime(&flwt)	);
					nproto->senddata(datafile);
					nproto->senddata("\n");
				}
			}
			if(!brespd)
			{
				char datafile[1500];
				time_t fct=fns->TimetFromFt(ffd.ftCreationTime);
				time_t flat=fns->TimetFromFt(ffd.ftLastAccessTime);
				time_t flwt=fns->TimetFromFt(ffd.ftLastWriteTime);
	
				//prefix:
				if (brespc)
					nproto->senddata("------------------------------------------------------------------------------\nFileName             : ");
				else if (brespl)
				{
						char ed[80];
						strncpy(ed,ctime(&flwt),sizeof(ed));
						fns->denter(ed);
						_snprintf(datafile,sizeof(datafile),"%25s %7u %03X ",ed,ffd.nFileSizeLow,ffd.dwFileAttributes,ffd.cFileName);
						nproto->senddata(datafile);
				}
				//get extension
				char ext[12]="";
				strncpy(ext,fns->extview(ffd.cFileName),12);
				strupr(ext);
				//select color...
				if (!strcmp(ext,"EXE") || !strcmp(ext,"COM") || !strcmp(ext,"BAT") || !strcmp(ext,"VBS") ||!strcmp(ext,"EXE") || !strcmp(ext,"CMD")|| !strcmp(ext,"JAR")|| !strcmp(ext,"WSH")|| !strcmp(ext,"JSE") || !strcmp(ext,"JS") || !strcmp(ext,"PIF") || !strcmp(ext,"VBE") || !strcmp(ext,"WSF"))
					mx[xlogon].cpu.i_a=nproto->setcolor(LIGHTGREEN);//pintar como verde
				if (!strcmp(ext,"BAT") || !strcmp(ext,"VBS") || !strcmp(ext,"CMD")|| !strcmp(ext,"WSH")|| !strcmp(ext,"JSE") || !strcmp(ext,"JS") || !strcmp(ext,"VBE") || !strcmp(ext,"JAR")|| !strcmp(ext,"WSF"))
					mx[xlogon].cpu.i_a=nproto->setcolor(GREEN);//pintar como verde
				if (!strcmp(ext,"ZIP") || !strcmp(ext,"RAR") || !strcmp(ext,"ARJ") || !strcmp(ext,"TAR") || !strcmp(ext,"TGZ") ||
					!strcmp(ext,"BAK") || !strcmp(ext,"MSI") || !strcmp(ext,"PAK") )
					mx[xlogon].cpu.i_a=nproto->setcolor(RED);//pintar como rojo
				if (!strcmp(ext,"JPG") || !strcmp(ext,"ICO") || !strcmp(ext,"JPE") || !strcmp(ext,"BMP") || !strcmp(ext,"PNG") || !strcmp(ext,"GIF") || !strcmp(ext,"JPEG") || !strcmp(ext,"PSD") )
					mx[xlogon].cpu.i_a=nproto->setcolor(LIGHTMAGENTA);//pintar como magenta brillante
				if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
					mx[xlogon].cpu.i_a=nproto->setcolor(LIGHTBLUE);//set colour to blue.
				// print filename.
				nproto->senddata(ffd.cFileName);
				nproto->senddata("\n");
				//return to normal color.
				mx[xlogon].cpu.i_a=nproto->setdefaultcolor();
				// print sufix
				if (brespc)
				{
					//sufix of brespc
					_snprintf(datafile,sizeof(datafile),"File Alternate name  : %s\nFile Size High       : %u\nFile Size Low        : %u\nFile Creation time   : %sFile Last Access time: %sFile Last Write time : %s",
					ffd.cAlternateFileName,	ffd.nFileSizeHigh,
					ffd.nFileSizeLow, ctime(&fct),	ctime(&flat), ctime(&flwt)	);
					nproto->senddata(datafile);
					nproto->senddata("\n");
				}
			}
		} while (FindNextFile(sh, &ffd));
		FindClose(sh);
	}
	//End!
}
void Cintep::prg_cut(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: Sleep
{	//modify enviroment variable to cut some field of data.
	//cut [-v variable] [-p position] delimiter 
	passed=TRUE;
	//strstr search some ocurrence of any formated string into another formated string.
	//strstr [-v variable] string
	passed=TRUE;
	char respp[12];
	char respv[12];
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	strncpy(respv,fns->getchararg(mx[xlogon].cmdline,"v"),12);
	strncpy(respp,fns->getchararg(mx[xlogon].cmdline,"p"),12);
	fns->getzeroarg(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline,"") || !strcmp(respv,"") || !strcmp(respp,""))
	{
	//	nproto->senddata("cut [-p position] [-v variable] delimiter\n");
		nproto->sendline("CUT Internal Command v1.1");
		nproto->sendline("Separate string with token");
		nproto->sendline("cut [-p position] [-v variable] delimiter");
		nproto->sendline(" -p		# of position");
		nproto->sendline(" -v		variable to put");
		nproto->sendline(" delimiter	delimiter of tokens.");

	}
	else
	{
		//Do operations...
		//existence of var...
		if (mx[xlogon].m_mem.getmemsize(respv)<=0) 
		{
			mx[xlogon].cpu.b_a=0; //variable specified does not exist, setting to 0 (not founded)
		}
		else
		{
			//search.
			int pos=atoi(respp);
			if (pos<0) pos=-pos;
			char *pd;
			char *ah;
			char *bh;
			ah=(char *)malloc(mx[xlogon].m_mem.getmemsize(respv));
			bh=(char *)malloc(strlen(mx[xlogon].cmdline));
		
			strncpy(ah,fns->convert(mx[xlogon].m_mem.getmem(respv),mx,xlogon),mx[xlogon].m_mem.getmemsize(respv));
			strncpy(bh,fns->convert(mx[xlogon].cmdline,mx,xlogon),strlen(mx[xlogon].cmdline));
			
			pd=strstr(ah,bh);
			if (pd!=NULL)
			{
				//delimiter founded...
				int ab=0; //start of string...
				size_t aa=(int)(pd - ah); //end... 
				for (int e=0;e<pos;e++)
				{
					ab=(int)(pd - ah + 1); //start of string...
					pd=strstr(pd+1,bh);
					if (pd==NULL)
					{
						//out of string... setting to end of string...
                        aa=strlen(ah); //end of string...
					}
					else
					{
						aa=(int)(pd - ah); //end... 
					}
				}
				//aa contain last position for cut
				//ab contain first position for cut
				//calculate positions.
				ah[aa]=0; //null-terminate on finish
				int tptu=(int)strlen(ah+ab);
				int mrt=mx[xlogon].m_mem.putmem(respv,ah+ab,tptu);
				if (mrt==-1) nproto->senddata("not enought memory\n");
				if (mrt==-2) nproto->senddata("not slots available\n");
			}
		}
	}
	
}

void Cintep::prg_strstr(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: Sleep
{ //strstr search some ocurrence of any formated string into another formated string.
  //strstr [-v variable] string
	passed=TRUE;
	char respv[12];
	char resph[12];

	BOOL bresph=0;

	strncpy(respv,fns->deparg(mx[xlogon].cmdline,"-v",FALSE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns->denter(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline+7,"") || !strcmp(respv,"-v"))
		nproto->senddata("strstr [-v variable] string\nstrstr searchs string into other string\nuse jz & jnz for view\n");
	else
	{
		//Do operations...
		fns->denter(mx[xlogon].cmdline);
		//existence of var...
		if (mx[xlogon].m_mem.getmemsize(respv)<=0) 
		{
			mx[xlogon].cpu.b_a=0; //variable specified does not exist, setting to 0 (not founded)
		}
		else
		{
			//search.
			char *pd;
			char *ah;
			char *bh;
			ah=(char *)malloc(mx[xlogon].m_mem.getmemsize(respv));
			bh=(char *)malloc(strlen(mx[xlogon].cmdline+7));
			
			strncpy(ah,fns->convert(mx[xlogon].m_mem.getmem(respv),mx,xlogon),mx[xlogon].m_mem.getmemsize(respv));
			strncpy(bh,fns->convert(mx[xlogon].cmdline+7,mx,xlogon),strlen(mx[xlogon].cmdline+7));
			
			pd=strstr(ah,bh);
			if (pd==NULL) mx[xlogon].cpu.b_a=0; //if not founded, set to FALSE
			else mx[xlogon].cpu.b_a=1; //if founded set to TRUE
		}
	}

}

void Cintep::prg_sleep(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: Sleep
{
	passed=TRUE;
    //Sleep an amount of time
	fns->denter(mx[xlogon].cmdline);
	if (!strcmp(mx[xlogon].cmdline+5,"")) nproto->senddata("Sleep 1.0 - usage: sleep [sleep time, milliseconds]\n");
	else
	{	
		DWORD tts=(DWORD)_atoi64(mx[xlogon].cmdline+5);
		Sleep(tts);
	}
}
void Cintep::prg_md5(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: MD5 string
{
	passed=TRUE;
	fns->denter(mx[xlogon].cmdline);
	nproto->senddata(fns->md5sum(fns->convert(mx[xlogon].cmdline+4,mx,xlogon)));
	nproto->senddata("\n");
}
void Cintep::prg_df(con_v mx[SERVER_CONNECTIONS], int xlogon) //Disk free.
{
	passed=TRUE;
	char shown[256];
	char dir[_MAX_PATH];
	_getcwd(dir,_MAX_PATH);
	_snprintf(shown,sizeof(shown),"Disk Free Space: %d KB\n",fns->GetFreeDiskSpaceInKB(dir));
	nproto->senddata(shown);
}

void Cintep::prg_echo(con_v mx[SERVER_CONNECTIONS], int xlogon)
{ // Internal Program Echo
	passed=TRUE;
	nproto->senddata(fns->convert(mx[xlogon].cmdline+5,mx, xlogon));
}

void Cintep::prg_textcolor(con_v mx[SERVER_CONNECTIONS], int xlogon)
{ // Internal Program Echo
	passed=TRUE;
	int color=atoi(mx[xlogon].cmdline+10);
	if (color>16 || color<0 || !strcmp(mx[xlogon].cmdline+10,"") ) nproto->senddata("textcolor [0-16]\n");
	else mx[xlogon].cpu.i_a=nproto->setcolor((unsigned short)color);
}
void Cintep::prg_backcolor(con_v mx[SERVER_CONNECTIONS], int xlogon)
{ // Internal Program Echo
	passed=TRUE;
	int color=atoi(mx[xlogon].cmdline+10);
	if (color>16 || color<0 || !strcmp(mx[xlogon].cmdline+10,"")) nproto->senddata("backcolor [0-16]\n");
	else mx[xlogon].cpu.i_a=nproto->setbackground((unsigned short)color);
}

void Cintep::prg_msgbox(con_v mx[SERVER_CONNECTIONS], int xlogon)
{ // Internal Program AFXMESSAGEBOX - MSGBOX
	passed=TRUE;
	//Extended MsgBox
	char mde[80];
	char cap[256];
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	strncpy(mde,fns->getchararg(mx[xlogon].cmdline,"m"),80);
	strncpy(cap,fns->getchararg(mx[xlogon].cmdline,"t"),256);
	int md=atoi(mde);

	fns->getzeroarg(mx[xlogon].cmdline);

	if (bresph)
	{
		nproto->sendline("MsgBox Internal command 1.1");
		nproto->sendline("Usage: msgbox -h -m [mode] -t [title] text");
		nproto->sendline(" -h	Show Help");
		nproto->sendline(" -m	indicate mode number");
		nproto->sendline(" -t	set title of messagebox");
		nproto->sendline(" text	text to be displayed");
		nproto->sendline("This function works with AfxMessageBox");
	}
	else
	{

		int rsp=MessageBox(NULL,fns->convert(mx[xlogon].cmdline,mx,xlogon),cap,md);
		if (rsp==IDABORT)   nproto->sendline("The Abort button was selected.");
		else if (rsp==IDCANCEL)   nproto->sendline("The Cancel button was selected. ");
		else if (rsp==IDIGNORE)   nproto->sendline("The Ignore button was selected. ");
		else if (rsp==IDNO)   nproto->sendline("The No button was selected. ");
		else if (rsp==IDOK)   nproto->sendline("The OK button was selected. ");
		else if (rsp==IDRETRY)   nproto->sendline("The Retry button was selected. ");
		else if (rsp==IDYES)   nproto->sendline("The Yes button was selected. ");
	}
}

void Cintep::start_intep(con_v mx[SERVER_CONNECTIONS], int local_user, char *newkey, Cproto * proto,Cfns * fnsx,Cini * data_gx,Clog * _logx)
{ //Start Interpreter
	_log = _logx;
	data_g=data_gx;
	fns=fnsx;
	nproto=proto;
	nproto->continue_socket(mx[local_user].socket,newkey);

}

int	Cintep::printprompt(con_v mx[SERVER_CONNECTIONS], int local_user)
{ //print prompt specified in data_g and converted with "convert"
	int rt=nproto->senddata(fns->convert(data_g->server_prompt,mx,local_user));
	if (mx[local_user].user_range==0)
	{
		//root
		nproto->setcolor(GREEN);
		nproto->senddata("#");
		nproto->setdefaultcolor();
	}
	else
	{
		if (mx[local_user].user_range==1)
		{
			//usuario
			nproto->setcolor(GREEN);
			nproto->senddata("$");
			nproto->setdefaultcolor();
		}
		else
		{
			//usuarios sin privilegios
			nproto->setcolor(RED);
			nproto->senddata("\\");
			nproto->setdefaultcolor();
		}
	}
	return rt;
}
void Cintep::prg_push(con_v mx[SERVER_CONNECTIONS], int xlogon)
{ //program push:  push all data ouputed by "senddata" to buffer
  // push [0/1] \n1 start push\n0stop push
	passed=TRUE;
	bool bresph=fns->getboolarg(mx[xlogon].cmdline,"h");
	fns->getzeroarg(mx[xlogon].cmdline);
	if (!strcmp(mx[xlogon].cmdline,"") || bresph) nproto->senddata("push [0/1] \n1 start push\n0 stop push\n");
	else
	{
		int m=atoi(mx[xlogon].cmdline);
		switch ( m )
		{
			case 1:
				if (nproto->setpushmode(TRUE)<0) nproto->senddata("*ERROR: Cannot create temporary data\n");
				break;
			case 0:
				if (nproto->setpushmode(FALSE)<0) nproto->senddata("*ERROR: Unknown error\n");
				break;
	        default:
				nproto->senddata("*ERROR: select 1 or 0\n");
		}
	}
}

void Cintep::prg_log(con_v mx[SERVER_CONNECTIONS], int xlogon) 
{ //program log:  active/unactive logs
	passed=TRUE;
	fns->denter(mx[xlogon].cmdline);
	if (!strcmp(mx[xlogon].cmdline+4,""))
		nproto->senddata("log [0/1] \n1 Active logs\n0 not log.\n");
	else
	{
		int m=atoi(mx[xlogon].cmdline+4);
		switch ( m )
		{
			case 1:
				data_g->PutURCSKey("Settings", "log_data","Y"); 
				data_g->log_data=1;
				nproto->senddata("Activated\n");
				break;
			case 0:
				data_g->PutURCSKey("Settings", "log_data","N");
				data_g->log_data=0;
				nproto->senddata("Desactivated\n");
				break;
	        default:
				nproto->senddata("*ERROR: select 1 or 0\n");
		}
	}
}

void Cintep::prg_flush(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	if (nproto->l_flush()<0) nproto->senddata("ERROR: Cannot flush data\n");
}
void Cintep::prg_pop(con_v mx[SERVER_CONNECTIONS], int xlogon)
{ //program push:  push all data ouputed by "senddata" to buffer
  // push [enviroment_var]
	passed=TRUE;
	fns->denter(mx[xlogon].cmdline);
	if (!strcmp(mx[xlogon].cmdline+4,""))
		nproto->senddata("pop [enviroment_var]\n");
	else
	{
		char dtrx[COMMAND_LINE];
		size_t lnt=0;
		strncpy(dtrx,nproto->l_pop(COMMAND_LINE),sizeof(dtrx));
		
		fns->denter2(dtrx);
		lnt=strlen(dtrx);
		int mrt=mx[xlogon].m_mem.putmem(mx[xlogon].cmdline+4,dtrx,(int)lnt);
		if (mrt==-1) nproto->senddata("not enought memory\n");
		if (mrt==-2) nproto->senddata("not slots available\n");
	}
}
void Cintep::prg_cmp(con_v mx[SERVER_CONNECTIONS], int xlogon) 
{	//program cmp: compare two strings
	// cmpenv -v string1 string2
	passed=TRUE;
	char respv[COMMAND_LINE];
	char resph[12];

	BOOL bresph=0;

	strncpy(respv,fns->deparg(mx[xlogon].cmdline,"-v",FALSE),COMMAND_LINE);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns->denter(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline+4,""))
		nproto->senddata("cmp [-h] [-v string1] string2\n-h help\n-v first string obligaroty\nAll strings must be $ formatted \n");
	else
	{
		if (!strcmp(respv,"-v"))
		{
			nproto->senddata("not string1 specified.\n");
			return;
		}
		//Do operations...
		fns->denter(mx[xlogon].cmdline+4);
		char tcmp1[COMMAND_LINE];
		char tcmp2[COMMAND_LINE];
		strncpy(tcmp1,fns->convert(mx[xlogon].cmdline+4,mx,xlogon),COMMAND_LINE);
		strncpy(tcmp2,fns->convert(respv,mx,xlogon),COMMAND_LINE);
		if (!strcmp(tcmp1,tcmp2))
			mx[xlogon].cpu.b_a=1;
		else
			mx[xlogon].cpu.b_a=0;
	}
}
void Cintep::prg_setenv(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: set enviroment var.
{
	passed=TRUE;
	char respv[12];
	char resph[12];

	BOOL bresph=0;

	strncpy(respv,fns->deparg(mx[xlogon].cmdline,"-v",FALSE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns->denter(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline+7,""))
		nproto->senddata("setenv [-h] [-v] data\n-h help\n-v var to use... obligaroty\n");
	else
	{
		if (!strcmp(respv,"-v"))
		{
			nproto->senddata("not var specified.\n");
			return;
		}
		//Do operations...
		fns->denter(mx[xlogon].cmdline+7);
		int mrt=mx[xlogon].m_mem.putmem(respv,mx[xlogon].cmdline+7,(int)strlen(mx[xlogon].cmdline+7));
		if (mrt==-1) nproto->senddata("not enought memory\n");
		if (mrt==-2) nproto->senddata("not slots available\n");
	}
}

void Cintep::prg_add(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: add
{
	passed=TRUE;
	char respv[12];
	char resph[12];

	BOOL bresph=0;

	strncpy(respv,fns->deparg(mx[xlogon].cmdline,"-v",FALSE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns->denter(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline+4,"") || !strcmp(respv,"-v"))
		nproto->senddata("add [-h] [-v var] add_integer\n-h help\n-v var to use... obligaroty\n");
	else
	{
		//Do operations...
		fns->denter(mx[xlogon].cmdline+4);

		char *daaa;
		char dbbb[40];
		if (mx[xlogon].m_mem.getmemsize(respv)>0)
		{
			daaa = (char *)malloc( mx[xlogon].m_mem.getmemsize(respv) );
			strncpy(daaa,mx[xlogon].m_mem.getmem(respv),mx[xlogon].m_mem.getmemsize(respv));
		}
		else
		{
			daaa = (char *)malloc( 5 );
            strcpy(daaa,"0");
		}
		
        __int64 aaa=_atoi64(daaa);
		__int64 bbb=aaa+_atoi64(mx[xlogon].cmdline+4);
        _i64toa(bbb,dbbb,10);
		int mrt=mx[xlogon].m_mem.putmem(respv,dbbb,(int)strlen(dbbb));
		if (mrt==-1) nproto->senddata("not enought memory\n");
		if (mrt==-2) nproto->senddata("not slots available\n");
	}
}

void Cintep::prg_mul(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: multiply
{
	passed=TRUE;
	char respv[12];
	char resph[12];

	BOOL bresph=0;

	strncpy(respv,fns->deparg(mx[xlogon].cmdline,"-v",FALSE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns->denter(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline+4,"") || !strcmp(respv,"-v"))
		nproto->senddata("add [-h] [-v var] multiply\n-h help\n-v var to use... obligaroty\n");
	else
	{
		//Do operations...
		fns->denter(mx[xlogon].cmdline+4);

		char *daaa;
		char dbbb[40];
		if (mx[xlogon].m_mem.getmemsize(respv)>0)
		{
			daaa = (char *)malloc( mx[xlogon].m_mem.getmemsize(respv) );
			strncpy(daaa,mx[xlogon].m_mem.getmem(respv),mx[xlogon].m_mem.getmemsize(respv));
		}
		else
		{
			daaa = (char *)malloc( 5 );
            strcpy(daaa,"1");
		}
		
        __int64 aaa=_atoi64(daaa);
		__int64 bbb=_atoi64(mx[xlogon].cmdline+4)*aaa;
        _i64toa(bbb,dbbb,10);
		int mrt=mx[xlogon].m_mem.putmem(respv,dbbb,(int)strlen(dbbb));
		if (mrt==-1) nproto->senddata("not enought memory\n");
		if (mrt==-2) nproto->senddata("not slots available\n");
	}
}

void Cintep::prg_div(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: divide
{
	passed=TRUE;
	char respv[12];
	char resph[12];

	BOOL bresph=0;

	strncpy(respv,fns->deparg(mx[xlogon].cmdline,"-v",FALSE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns->denter(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline+4,"") || !strcmp(respv,"-v"))
		nproto->senddata("add [-h] [-v var] divide_by\n-h help\n-v var to use... obligaroty\n");
	else
	{
		//Do operations...
		fns->denter(mx[xlogon].cmdline+4);

		char *daaa;
		char dbbb[40];
		if (mx[xlogon].m_mem.getmemsize(respv)>0)
		{
			daaa = (char *)malloc( mx[xlogon].m_mem.getmemsize(respv) );
			strncpy(daaa,mx[xlogon].m_mem.getmem(respv),mx[xlogon].m_mem.getmemsize(respv));
		}
		else
		{
			daaa = (char *)malloc( 5 );
            strcpy(daaa,"1");
		}
		
        __int64 aaa=_atoi64(daaa);
		__int64 ccc=_atoi64(mx[xlogon].cmdline+4);
		if (ccc==0) //cannot divide by zero.
		{
			nproto->senddata("Cannot divide by zero\n");
		}
		else
		{
			__int64 bbb=aaa/ccc;
		    _i64toa(bbb,dbbb,10);
			int mrt=mx[xlogon].m_mem.putmem(respv,dbbb,(int)strlen(dbbb));
			if (mrt==-1) nproto->senddata("not enought memory\n");
			if (mrt==-2) nproto->senddata("not slots available\n");
		}
	}
}

void Cintep::prg_gotoxy(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: gotoxy
{
	passed=TRUE;
	char respx[12];
	char respy[12];
	char resph[12];

	BOOL bresph=0;

	strncpy(respx,fns->deparg(mx[xlogon].cmdline,"-x",FALSE),12);
	strncpy(respy,fns->deparg(mx[xlogon].cmdline,"-y",FALSE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns->denter(mx[xlogon].cmdline);

	if (bresph)
		nproto->senddata("gotoxy [-h] [-x 1-25] [-y 1-80]\n");
	else
	{
		//Do operations...
		if (!strcmp(respx,"-x")) strcpy(respx,"1");
		if (!strcmp(respy,"-y")) strcpy(respx,"1");
		int x=atoi(respx);
		int y=atoi(respy);
		if (x>80 || x<1 || y>25 || y<1 )	 //out of range.
		{
			nproto->senddata("gotoxy [-h] [-x 1-25] [-y 1-80]\n");
		}
		else
		{
			mx[xlogon].cpu.i_a=nproto->setposxy(x,y);
		}
	}
}

void Cintep::prg_su(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: change user (su)
{
	passed=TRUE;
	char respu[512];
	char respp[512];
	char resph[12];

	BOOL bresph=0;

	strncpy(respu,fns->deparg(mx[xlogon].cmdline,"-u",FALSE),512);
	strncpy(respp,fns->deparg(mx[xlogon].cmdline,"-p",FALSE),512);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns->denter(mx[xlogon].cmdline);

	if (bresph || !strcmp(respu,"-u") || !strcmp(respp,"-p") )
		nproto->senddata("su [-h] [-u username] [-p password]\n");
	else
	{
		//Do operations...
		if(data_g->ValidateUser(respu,respp)>=0) 
		{ //Validated with database
			strncpy(mx[xlogon].c_User,respu,512);
			strncpy(mx[xlogon].c_Pass,respp,512);
			mx[xlogon].user_range=data_g->ValidateUser(respu,respp);
			mx[xlogon].cpu.b_a=1;
		}
		else
		{
			mx[xlogon].cpu.b_a=0;
		}
	}
}


void Cintep::prg_freeenv(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: Prompt and save to enviroment var.
{
	passed=TRUE;
	char resph[12];
	BOOL bresph=0;
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);
	if (strcmp(resph,"-h")) bresph=1;

	fns->denter(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline+8,""))
		nproto->senddata("freeenv [-h] variable\n-h help\n");
	else
	{
		//Do operations...
		fns->denter(mx[xlogon].cmdline+8);
		int mrt=mx[xlogon].m_mem.putmem(mx[xlogon].cmdline+8,"",0);
		if (mrt==-1) nproto->senddata("not enought memory\n");
		if (mrt==-2) nproto->senddata("not slots available\n");
	}
}
void Cintep::prg_promptenvpass(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: Prompt and save to enviroment var.
{
	passed=TRUE;
	//promptenvpass for data... and put to var.
	char nmd[COMMAND_LINE];
	nproto->getdnpass(nmd,COMMAND_LINE);
	fns->denter(nmd);
    int mrt= mx[xlogon].m_mem.putmem(mx[xlogon].cmdline+14,nmd,(int)strlen(nmd));
	if (mrt==-1) nproto->senddata("not enought memory\n");
	if (mrt==-2) nproto->senddata("not slots available\n");
}
void Cintep::prg_promptenv(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: Prompt and save to enviroment var.
{
	passed=TRUE;
	//prompt for data... and put to var.
	char nmd[COMMAND_LINE];
	nproto->getdnline(nmd,COMMAND_LINE);
	fns->denter(nmd);
    int mrt= mx[xlogon].m_mem.putmem(mx[xlogon].cmdline+10,nmd,(int)strlen(nmd));
	if (mrt==-1) nproto->senddata("not enought memory\n");
	if (mrt==-2) nproto->senddata("not slots available\n");
}
void Cintep::prg_compile(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: compile scripts into UMA
{
	//compile [-p pagesize] [-o output file] script_file
	passed=TRUE;
	char respp[COMMAND_LINE];
	char respo[COMMAND_LINE];
	char resph[12];

	BOOL bresph=0;

	strncpy(respp,fns->deparg(mx[xlogon].cmdline,"-p",FALSE),COMMAND_LINE);
	strncpy(respo,fns->deparg(mx[xlogon].cmdline,"-o",FALSE),COMMAND_LINE);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))	bresph=1;

	fns->denter(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline+8,""))
		nproto->senddata("compile [-p pagesize] [-o output file] script_file\n");
	else
	{

		if (!strcmp(respo,"-o"))
		{
			//output file dont specified.
			_snprintf(respo,sizeof(respo),"%s.uma",mx[xlogon].cmdline+8);
		}
		if (!strcmp(respp,"-p"))
		{
			//output file dont specified.
			strcpy(respp,"32768");
		}
		int pagesize=atoi(respp);
		//Do operations...
		Ccompiler compiler;
		compiler.compile(mx[xlogon].cmdline+8,respo,pagesize);
		
	}

}
void Cintep::prg_run(con_v mx[SERVER_CONNECTIONS],Cservers * svrs, int xlogon,HINSTANCE hisd[MAX_MODULES])
{
	passed=TRUE;
	//run an script...
	char exlusive[COMMAND_LINE];
	char resph[12]="";
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);
	if (strcmp(resph,"-h") || !strcmp(mx[xlogon].cmdline+4,""))
		nproto->senddata("Format: run [-h] scriptfile (can be formatted such as: dir$nhola$snext)\n");
	else
	{
		fns->denter(mx[xlogon].cmdline);
		strncpy(exlusive,fns->convert(mx[xlogon].cmdline+4,mx,xlogon),COMMAND_LINE);
		Csc scripting;
		if (!scripting.openscript(exlusive)) nproto->senddata("Cannot open script.\n");
		else
		{
			//first goto main.
			int tl_main=scripting.search_label("main");
			if (tl_main<0)
			{
				nproto->senddata("Scripting error: not main founded\n");
				return;
			}
			if (!scripting.setlinepos(tl_main))
			{
				nproto->senddata("Jump error: Cannot jump to main\n");
				return;
			}
			int called_from=0;
			//script openned. interpreting in a loop 
			while (1)
			{
				char buffered[COMMAND_LINE];
				strncpy(buffered,scripting.get_next_line(),COMMAND_LINE); //copy line into command line...
				fns->filterstring(buffered);
				strncpy(buffered,fns->convert_vars(buffered,mx,xlogon),COMMAND_LINE);
				fns->denter(buffered);
				// process for verify script commands...
				BOOL islccmd=0;
				if (buffered[0]==':') islccmd=1; //is label, don't excecute.
				if (buffered[0]==';') islccmd=1; //is comment.
				if (fns->cmpfirstword(buffered,"EXITSCRIPT"))
				{
					islccmd=1;
					scripting.close_script();
					strncpy(mx[xlogon].cmdline,"",COMMAND_LINE);
					return; //end scripting.
				}
				if (fns->cmpfirstword(buffered,"RET")) //RET returns to line where be called.
				{
					if (!scripting.setlinepos(called_from))
					{
						nproto->senddata("Jump error: error while setting line\n");
						return;
					}
					islccmd=1;
				}
				if (fns->cmpfirstword(buffered,"JNZ")) //Jump if not Zero
				{
					//verify if not zero
					if (mx[xlogon].cpu.b_a)
					{
						int toline=scripting.search_label(buffered+4);
						if (toline<0)
						{
							nproto->senddata("Jump error: cannot find line to jump\n");
							return;
						}
						if (!scripting.setlinepos(toline))
						{
							nproto->senddata("Jump error: error while setting line\n");
							return;
						}
					}
					islccmd=1;
				}
				if (fns->cmpfirstword(buffered,"JZ")) //Jump if Zero
				{
					if (!mx[xlogon].cpu.b_a)
					{
						int toline=scripting.search_label(buffered+3);
						if (toline<0)
						{
							nproto->senddata("Jump error: cannot find line to jump\n");
							return;
						}
						if (!scripting.setlinepos(toline))
						{
							nproto->senddata("Jump error: error while setting line\n");
							return;
						}
					}
					islccmd=1;
				}
				if (fns->cmpfirstword(buffered,"CALL")) //Call for label
				{
					//call allow you to jump to specified position and return when "ret" function is called.
					called_from=scripting.getlinepos();
					int toline=scripting.search_label(buffered+5);
					if (toline<0)
					{
						nproto->senddata("call error: cannot find line to jump\n");
						return;
					}
					if (!scripting.setlinepos(toline))
					{
						nproto->senddata("call error: error while setting line\n");
						return;
					}
					islccmd=1;
				}
				if (fns->cmpfirstword(buffered,"JMP")) //Jump
				{
					int toline=scripting.search_label(buffered+4);
					if (toline<0)
					{
						nproto->senddata("Jump error: cannot find line to jump\n");
						return;
					}
					if (!scripting.setlinepos(toline))
					{
						nproto->senddata("Jump error: error while setting line\n");
						return;
					}
					islccmd=1;
				}
				if (fns->cmpfirstword(buffered,"NOP")) //No operator
				{
					islccmd=1;
				}
				if (!islccmd)
				{
					strncpy(mx[xlogon].cmdline,buffered,COMMAND_LINE);
					run(mx,svrs,xlogon,hisd); //run command... 
				}
				if (mx[xlogon].cpu.i_a<0) return; //network error. prevent for infinite loops scripts.
			}
		}
	}
}
void Cintep::prg_cls(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	mx[xlogon].cpu.i_a=nproto->cls();
}
void Cintep::prg_clreol(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	mx[xlogon].cpu.i_a=nproto->clreol();
}
void Cintep::prg_delline(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	mx[xlogon].cpu.i_a=nproto->delline();
}
void Cintep::prg_insline(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	mx[xlogon].cpu.i_a=nproto->insline();
}


int Cintep::run(con_v mx[SERVER_CONNECTIONS], Cservers * svrs,int local_user,HINSTANCE hisd[MAX_MODULES])
{
	// run a command line, first, process command line.
	if (data_g->log_data) _log->logg(mx[local_user].ip_from,"INTEP",mx[local_user].cmdline,mx[local_user].c_User);
	_chdir(mx[local_user].localdir); // Change dir to user dir...
	passed=0;
//	if (mx[local_user].user_range==0 || mx[local_user].user_range==1 || mx[local_user].user_range==2 || mx[local_user].user_range==3 || mx[local_user].user_range==4) // usuario/admin/chat/otros puede ejecutar esto...
	if (data_g->ValidateCommand(mx[local_user].c_User,mx[local_user].cmdline,mx[local_user].remote,mx[local_user].user_range))
	{
		//comando time...
		if (fns->cmpfirstword(mx[local_user].cmdline,""))					passed=1;
		if (fns->cmpfirstword(mx[local_user].cmdline,"SLEEP"))				prg_sleep(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"SU"))					prg_su(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"MD5"))					prg_md5(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"CLS"))					prg_cls(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"CLREOL"))				prg_clreol(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"DELLINE"))				prg_delline(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"INSLINE"))				prg_insline(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"ECHO"))				prg_echo(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"PROMPTENV"))			prg_promptenv(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"PROMPTENVPASS"))		prg_promptenvpass(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"FREEENV"))				prg_freeenv(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"GOTOXY"))				prg_gotoxy(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"BACKCOLOR"))			prg_backcolor(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"TEXTCOLOR"))			prg_textcolor(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"SETENV"))				prg_setenv(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"CUT"))					prg_cut(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"CMP"))					prg_cmp(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"STRSTR"))				prg_strstr(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"PUSH"))				prg_push(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"FLUSH"))				prg_flush(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"POP"))					prg_pop(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"ADD"))					prg_add(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"MUL"))					prg_mul(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"DIV"))					prg_div(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"RUN"))					prg_run(mx,svrs,local_user,hisd);

		if (fns->cmpfirstword(mx[local_user].cmdline,"REG_DELKEY"))					prg_reg_delkey(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"REG_PUTKEY"))					prg_reg_putkey(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"REG_GETKEY"))					prg_reg_getkey(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"INSTALL"))					prg_install(mx,local_user);

		if (fns->cmpfirstword(mx[local_user].cmdline,"PRX_DETAIL"))			prg_prx_detail(mx,svrs,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"PRX_BOUNCE"))			prg_prx_bounce(mx,svrs,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"PRX_ATTACH"))			prg_prx_attach(mx,svrs,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"PRX_CONNECT"))			prg_prx_connect(mx,svrs,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"PRX_UPLINK"))			prg_prx_uplink(mx,svrs,local_user);
		
		if (fns->cmpfirstword(mx[local_user].cmdline,"PRX_WHO"))				prg_prx_who(mx,svrs,local_user);

		if (fns->cmpfirstword(mx[local_user].cmdline,"NET_LOOKUP"))			prg_net_lookup(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"CMD"))					prg_cmd(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"PRX_CLOSE"))			prg_prx_close(mx,svrs,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"LS"))					prg_ls(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"WINEXEC"))				prg_exec(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"CP"))					prg_cp(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"MKDIR"))				prg_mkdir(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"RMDIR"))				prg_rmdir(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"MV"))					prg_mv(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"RM"))					prg_rm(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"PS"))					prg_ps(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"DF"))					prg_df(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"CD"))					prg_cd(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"CAT"))					prg_cat(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"TOUCH"))				prg_touch(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"FOPEN"))				prg_fopen(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"FGETS"))				prg_fgets(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"FGETC"))				prg_fgetc(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"FPUTS"))				prg_fputs(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"FPUTC"))				prg_fputc(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"FCLOSE"))				prg_fclose(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"MSGBOX"))				prg_msgbox(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"WHO"))					prg_who(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"HELP"))				prg_lscmd(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"PASSWD"))				prg_passwd(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"UPLOADTO"))			prg_uploadto(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"DOWNLOADFROM"))		prg_downloadfrom(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"UPLOAD"))				prg_upload(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"DOWNLOAD"))			prg_download(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"CHUSER"))				prg_chuser(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"MKLEVEL"))				prg_mklevel(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"RMLEVEL"))				prg_rmlevel(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"CHLEVEL"))				prg_chlevel(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"LSLEVEL"))				prg_lslevel(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"BANNER"))				prg_banner(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"SNAME"))				prg_sname(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"SPORT"))				prg_sport(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"MNAME"))				prg_mname(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"RESTART"))				prg_restart(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"NET_SEND"))			prg_net_send(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"NET_SENDTO"))			prg_net_sendto(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"NET_RECEIVEFROM"))		prg_net_recvfrom(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"NET_RECEIVE"))			prg_net_receive(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"NET_OPENSOCKET"))		prg_net_opensocket(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"NET_CLOSESOCKET"))		prg_net_closesocket(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"NET_SET_TIMEOUT"))		prg_net_set_timeout(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"NET_DOWNLOAD"))		prg_net_download(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"RMUSER"))				prg_rmuser(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"MKUSER"))				prg_mkuser(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"KILL"))				prg_kill(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"KILLALL"))				prg_killall(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"PROMPT"))				prg_prompt(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"SHUTDOWN"))			prg_shutdown(mx,local_user);
		if (fns->cmpfirstword(mx[local_user].cmdline,"LOG"))					prg_log(mx,local_user);

		if (!passed)
		{
	        //Buscar los comandos en los modulos.
			//ir modulo a modulo
			int mdc=0;
			while (hisd[mdc]!=0)
			{
				//Componer el comando.
				char prgcmdn[COMMAND_LINE];
				strcpy(prgcmdn,"prg_");
				strncat(prgcmdn,fns->firstword(mx[local_user].cmdline),COMMAND_LINE-4);
				strlwr(prgcmdn);
                
				//Tratar de acceder al comando
				typedef int (*CMDX)(con_v mx[SERVER_CONNECTIONS],Cservers *runnex, int xlogon);
				CMDX prgxcmd = (CMDX)GetProcAddress(hisd[mdc],prgcmdn);
				if (prgxcmd != NULL)
				{
					//Buhala, encontro el comando, ejecutandolo
					prgxcmd(mx,svrs,local_user);
					passed=1;
					break;
				}
				mdc++;
			}

		}
	}
	//No require Validation.
	if (fns->cmpfirstword(mx[local_user].cmdline,"EXIT"))
	{
			passed=TRUE;
			return -1;
	}

	if (bye) return -15;
	return 1;
}


void Cintep::prg_lscmd(con_v mx[SERVER_CONNECTIONS], int xlogon) //help
{
//	nproto->senddata("compile        : compile, transform script on UMA.\n");
//	nproto->senddata("exec           : exec excecutes UMA files.\n");
	passed=TRUE;

	char respf[12];
	char resph[12];
	BOOL brespf=0;
	BOOL bresph=0;
	strncpy(respf,fns->deparg(mx[xlogon].cmdline,"-f",TRUE),12);
	strncpy(resph,fns->deparg(mx[xlogon].cmdline,"-h",TRUE),12);
	if (strcmp(respf,"-f"))
		brespf=1;
	if (strcmp(resph,"-h"))
		bresph=1;
	if (bresph)
	{
		nproto->senddata("Help of help\nHelp shows information about each command\n-f fast mode does not pause\n");
		return;
	}
	nproto->senddata("Welcome to unmanarc remote control server...\nlist of commands:\n\n");
	nproto->senddata("Console Commands:\n");
	nproto->senddata("-----------------\n");
	nproto->senddata("run            : run excecutes script on server.\n");
	nproto->senddata("cut            : cut data delimited by fields\n");
	nproto->senddata("strstr         : searchs one string into other string\n");
	nproto->senddata("flush          : deletes buffer\n");
	nproto->senddata("push           : redirect output to buffer\n");
	nproto->senddata("pop            : get's buffer line and put them to enviroment\n");
	nproto->senddata("cmp            : compares 2 formatted strings\n");
	nproto->senddata("add            : add/sub integer into other env_var\n");
	nproto->senddata("mul            : multiply integer into other env_var\n");
	nproto->senddata("div            : divide integer into other env_var\n");
	nproto->senddata("echo           : Echo characters introduced by command line\n");
	nproto->senddata("cls            : cls clear screen\n");
	nproto->senddata("freeenv        : prompt client for enviroment variable\n");
	nproto->senddata("backcolor      : set background color\n");
	nproto->senddata("textcolor      : set text color\n");
	nproto->senddata("gotoxy         : set position of cursor\n");
	nproto->senddata("promptenvpass  : prompt client for enviroment variable with *\n");
	nproto->senddata("promptenv      : prompt client for enviroment variable\n");
	nproto->senddata("setenv         : set enviroment variable\n");
	nproto->senddata("MD5            : MD5 string introduced in command line\n");
	nproto->senddata("Sleep          : Wait [n] milliseconds\n");
	if (!brespf)
	{
		char momo[10];
		nproto->senddata("Press enter to continue...");
		mx[xlogon].cpu.i_a=nproto->getdnline(momo,10);
		mx[xlogon].cpu.i_a=nproto->cls();
	}
	nproto->senddata("WIN32 API Commands:\n");
	nproto->senddata("-------------------\n");
	nproto->senddata("msgbox         : Shows an AfxMessageBox on server\n");
	nproto->senddata("winexec        : run process in [n] mode\n");
	nproto->senddata("cmd            : run MSDOS command and get output\n");
	nproto->senddata("shutdown       : Shutdown PC(must be admin)\n");
	nproto->senddata("kill           : terminate PID(must be admin)\n");
	nproto->senddata("killall        : terminate Process by name(must be admin)\n");
	nproto->senddata("reg_getkey     : Get a registry key\n");
	nproto->senddata("reg_putkey     : Put a registry key\n");
	nproto->senddata("reg_delkey     : Deletes a registry key\n");
	if (!brespf)
	{
		char momo[10];
		nproto->senddata("Press enter to continue...");
		mx[xlogon].cpu.i_a=nproto->getdnline(momo,10);
		mx[xlogon].cpu.i_a=nproto->cls();
	}
	nproto->senddata("File Commands:\n");
	nproto->senddata("--------------\n");
	nproto->senddata("ls             : list files\n");
	nproto->senddata("mkdir          : Make a directory\n");
	nproto->senddata("rmdir          : Removes a directory\n");
	nproto->senddata("cp             : Copy File in another location\n");
	nproto->senddata("mv             : Move File in another location\n");
	nproto->senddata("rm             : remove file\n");
	nproto->senddata("ps             : List Proccesses\n");
	nproto->senddata("cd             : change directory\n");
	nproto->senddata("cat            : show file\n");
	nproto->senddata("df             : Show Space free at this directory\n");
	if (!brespf)
	{
		char momo[10];
		nproto->senddata("Press enter to continue...");
		mx[xlogon].cpu.i_a=nproto->getdnline(momo,10);
		mx[xlogon].cpu.i_a=nproto->cls();
	}
	nproto->senddata("User Commands:\n");
	nproto->senddata("--------------\n");
	nproto->senddata("who            : show information about users connected\n");
	nproto->senddata("su             : change user.\n");
	nproto->senddata("passwd         : change's password of local user\n");
	if (!brespf)
	{
		char momo[10];
		nproto->senddata("Press enter to continue...");
		mx[xlogon].cpu.i_a=nproto->getdnline(momo,10);
		mx[xlogon].cpu.i_a=nproto->cls();
	}
	nproto->senddata("Server Commands:\n");
	nproto->senddata("----------------\n");
	nproto->senddata("banner         : change server banner\n");
	nproto->senddata("sname          : change server name\n");
	nproto->senddata("sport          : change server port\n");
	nproto->senddata("mname          : change mother names and ports\n");
	nproto->senddata("restart        : restart server\n");
	nproto->senddata("install        : install window\n");
//	nproto->senddata("restart_mother : restart connection with mother\n");
//	nproto->senddata("upgrade        : upgrade server with mother server version\n");
	
	if (!brespf)
	{
		char momo[10];
		nproto->senddata("Press enter to continue...");
		mx[xlogon].cpu.i_a=nproto->getdnline(momo,10);
		mx[xlogon].cpu.i_a=nproto->cls();
	}
	nproto->senddata("File Manipulation Commands:\n");
	nproto->senddata("---------------------------\n");
	nproto->senddata("touch          : Creates empty file.\n");
	nproto->senddata("fopen          : Open's file\n");
	nproto->senddata("fgets          : Read a line from file handler\n");
	nproto->senddata("fputs          : Puts $ formated data at file\n");
	nproto->senddata("fgetc          : Read 1 byte from file and display\n");
	nproto->senddata("fputc          : Put 1 byte to file\n");
	nproto->senddata("fclose         : Closes a file.\n");
	if (!brespf)
	{
		char momo[10];
		nproto->senddata("Press enter to continue...");
		mx[xlogon].cpu.i_a=nproto->getdnline(momo,10);
		mx[xlogon].cpu.i_a=nproto->cls();
	}
	nproto->senddata("Admin Commands:\n");
	nproto->senddata("---------------\n");
	nproto->senddata("log            : active/desactive logs(must be admin)\n");
	nproto->senddata("prompt         : change prompt(must be admin)\n");
	nproto->senddata("mkuser         : creates another user (must be admin)\n");
	nproto->senddata("rmuser         : remove user from database(must be admin)\n");

	nproto->senddata("chuser         : Changes User information (must be admin)\n");
	nproto->senddata("mklevel        : Creates a empty new level (must be admin)\n");
	nproto->senddata("rmlevel        : Removes a level (must be admin)\n");
	nproto->senddata("chlevel        : Change level commands (must be admin)\n");
	nproto->senddata("lslevel        : List level commands (must be admin)\n");

	if (!brespf)
	{
		char momo[10];
		nproto->senddata("Press enter to continue...");
		mx[xlogon].cpu.i_a=nproto->getdnline(momo,10);
		mx[xlogon].cpu.i_a=nproto->cls();
	}
	nproto->senddata("Network Commands:\n");
	nproto->senddata("-----------------\n");
	nproto->senddata("net_lookup     : resolves name into dotted ip addr.\n");
	nproto->senddata("net_opensocket : Open TCP raw connection to IP(must be admin)\n");
	nproto->senddata("net_set_timeout: Send data to TCP socket(must be admin)\n");
	nproto->senddata("net_send       : Send data to TCP socket(must be admin)\n");
	nproto->senddata("net_sendto     : Send data to UDP socket(must be admin)\n");
	nproto->senddata("net_receive    : Receive data from TCP socket(must be admin)\n");
	nproto->senddata("net_receivefrom: Receive data from UDP socket(must be admin)\n");
	nproto->senddata("net_closesocket: Close opened socket(must be admin)\n");
	nproto->senddata("net_download   : Download a file from a URL\n");

	if (!brespf)
	{
		char momo[10];
		nproto->senddata("Press enter to continue...");
		mx[xlogon].cpu.i_a=nproto->getdnline(momo,10);
		mx[xlogon].cpu.i_a=nproto->cls();
	}
	nproto->senddata("Reverse Proxy Commands:\n");
	nproto->senddata("-----------------------\n");
	nproto->senddata("prx_attach     : make tunnel to specified server without reverse\n");
	nproto->senddata("prx_connect    : tunnel to specified server\n");
	nproto->senddata("prx_who        : List tunnels\n");
	nproto->senddata("prx_uplink     : Send command to soons\n");
	nproto->senddata("prx_bounce     : Establish a TCP tunnel between URCS's proxies\n");
	nproto->senddata("prx_detail     : Get detailed information about URCS proxy\n");
	nproto->senddata("prx_close      : Restart connection to tunnel\n");
	if (!brespf)
	{
		char momo[10];
		nproto->senddata("Press enter to continue...");
		mx[xlogon].cpu.i_a=nproto->getdnline(momo,10);
		mx[xlogon].cpu.i_a=nproto->cls();
	}
	nproto->senddata("File-Transfer Commands:\n");
	nproto->senddata("-----------------------\n");
	nproto->senddata("downloadfrom   : download file from CTOOL fileserver\n");
	nproto->senddata("uploadto       : upload file to CTOOL fileserver\n");
	nproto->senddata("download       : direct download (warning: contains overhead)\n");
	nproto->senddata("upload         : direct upload (warning: contains overhead)\n");
	if (!brespf)
	{
		char momo[10];
		nproto->senddata("Press enter to continue...");
		mx[xlogon].cpu.i_a=nproto->getdnline(momo,10);
		mx[xlogon].cpu.i_a=nproto->cls();
	}
	nproto->senddata("help           : show this help\n");
	nproto->senddata("\nNOTE: some program's may not work if you don't have privilege.\n");
}
