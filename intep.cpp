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
#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include "Tlhelp32.h"
#include <direct.h>
#include "stdio.h"
#include <io.h>
#include "proxy.h"
#include "file_transfer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


Cintep::Cintep()
{ //Constructor
	data_g.LoadData();
}
Cintep::~Cintep()
{
}

void Cintep::prg_downloadfrom(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	
	char respd[512];
	char respf[512];
	char resph[12];

	BOOL brespd=FALSE;
	BOOL brespf=FALSE;
	BOOL bresph=FALSE;

	strncpy(respd,fns.deparg(mx[xlogon].cmdline,"-d",FALSE),512);
	strncpy(respf,fns.deparg(mx[xlogon].cmdline,"-f",FALSE),512);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(respd,"-d"))
		brespd=TRUE;
	if (strcmp(respf,"-f"))
		brespf=TRUE;
	if (strcmp(resph,"-h"))
		bresph=TRUE;
	fns.denter(mx[xlogon].cmdline);

	if (bresph==TRUE)
		nproto.senddata("DOWNLOADFROM INTERNAL UTILITY\n-d destination IP\n-h help\n-f RemoteFile\n\rand local file\n\rExample:  download -d 216.72.226.8 -f readme.txt rd.txt\n");
	else
	{
		if (brespd==FALSE || brespf==FALSE)
			nproto.senddata("not enought information...\n");
		else
		{
			// All data well...
            //Making new connection to: respd
			Ctransf ft;
			nproto.senddata("Downloading file...");
			int g=ft.downloadfrom(respd,respf,mx[xlogon].cmdline+13);
			if (g>0) nproto.senddata("downloaded\n");
			else
			{
				if (g==-4) nproto.senddata("Host not founded\n");
				if (g==-1) nproto.senddata("file not found\n");
				if (g==-2) nproto.senddata("Unknown error\n");
				if (g==-3) nproto.senddata("Cannot Start sockets\n");
			}
		}
	}
}
void Cintep::prg_uploadto(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//upload file to IP
	//upload -d [destip] -f [localfile] remotefile
	passed=TRUE;

	char respd[512];
	char respf[512];
	char resph[12];

	BOOL brespd=FALSE;
	BOOL brespf=FALSE;
	BOOL bresph=FALSE;

	strncpy(respd,fns.deparg(mx[xlogon].cmdline,"-d",FALSE),512);
	strncpy(respf,fns.deparg(mx[xlogon].cmdline,"-f",FALSE),512);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(respd,"-d"))
		brespd=TRUE;
	if (strcmp(respf,"-f"))
		brespf=TRUE;
	if (strcmp(resph,"-h"))
		bresph=TRUE;
	fns.denter(mx[xlogon].cmdline);

	if (bresph==TRUE)
		nproto.senddata("UPLOADTO INTERNAL UTILITY\n-d destination IP\n-h help\n-f LocalFile\nand remote file\n\rExample_:  upload -d 216.72.226.8 -f c:\\autoexec.bat autoexec.bat.bak\n");
	else
	{
		if (brespd==FALSE || brespf==FALSE)
			nproto.senddata("not enought information...\n");
		else
		{
			// All data well...
            //Making new connection to: respd
			Ctransf ft;
			nproto.senddata("uploading file...");
			int g=ft.uploadto(respd,respf,mx[xlogon].cmdline+9);
			if (g>0) nproto.senddata("uploaded\n");
			else
			{
				if (g==-4) nproto.senddata("Host not founded\n");
				if (g==-1) nproto.senddata("Local file not found\n");
				if (g==-2) nproto.senddata("Unknown error\n");
				if (g==-3) nproto.senddata("Cannot Start sockets\n");
			}
		}
	}
}

void Cintep::prg_prx_who(con_v mx[SERVER_CONNECTIONS],ers_svr svrs[SERVER_SLOTS], int xlogon)
{
	passed=TRUE;
	char exterm[512];
	nproto.senddata("   ID  SOCK                IP     Name of server\n");
	for(int n=0;n<SERVER_SLOTS;n++)
	{
		if (svrs[n].busy)
		{
			sprintf(exterm,"%5u %5u %17s %18s\n",n,svrs[n].f,svrs[n].ip,svrs[n].nameserver);
			nproto.senddata(exterm);
		}
	}
}
void Cintep::prg_prx_close(con_v mx[SERVER_CONNECTIONS],ers_svr svrs[SERVER_SLOTS], int xlogon)
{
	//USAGE: prx_connect prx_id
	passed=TRUE;
	char exlusive[512];
	int mew=0;
	fns.denter(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline+strlen("PRX_CLOSE "),""))
	{
		nproto.senddata("USAGE: prx_close [prx_ID]");
		return;
	}
	else
		strncpy(exlusive,mx[xlogon].cmdline+strlen("PRX_CLOSE "),512);

	//on exlusive 
	mew=atoi(exlusive);
	//mew is an PRX_ID
	if(!svrs[mew].busy)
	{
		nproto.senddata("This pointer does not has an asociation\n");
		return;
	}
	if (svrs[mew].cbsy)
	{
		nproto.senddata("Another client are connected to this server.\n");
		return;
	}
	closesocket(svrs[mew].f);
	svrs[mew].cbsy=0;
	svrs[mew].busy=0;
	nproto.senddata("Closed Connection :S we hope that client are alive\n");
}
void Cintep::prg_prx_connect(con_v mx[SERVER_CONNECTIONS],ers_svr svrs[SERVER_SLOTS], int xlogon)
{
	passed=TRUE;
	//USAGE: prx_connect prx_id
	char exlusive[512];
	int mew=0;
	fns.denter(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline+strlen("PRX_CONNECT "),""))
	{
		nproto.senddata("USAGE: prx_connect [prx_ID]\n");
		return;
	}
	else
		strncpy(exlusive,mx[xlogon].cmdline+strlen("PRX_CONNECT "),512);

	//on exlusive 
	mew=atoi(exlusive);
	//mew is an PRX_ID
	if(!svrs[mew].busy)
	{
		nproto.senddata("This pointer does not has an asociation\n");
		return;
	}
	if (svrs[mew].cbsy)
	{
		nproto.senddata("Another client are connected to this server.\n");
		return;
	}
	//Send Activation Seq.
	
	svrs[mew].cbsy=1;
	Cproxy prd;
	prd.start_proxy_instance(mx[xlogon].socket,svrs[mew].f);

	//Activated Sequence... All server data will be redirected to
	//client.
	int j=prd.transfer(); //do proxy...

	if (j<=0)//server closes connection
	{		
		svrs[mew].busy=0;
		svrs[mew].cbsy=0;
		closesocket(svrs[mew].f);
	}
}

void Cintep::firstinstall(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	//First Installation
	int y=1;
	char rcv[3]="";
	
	BOOL pass_a=FALSE;
	BOOL pass_b=FALSE;
	BOOL pass_c=FALSE;
	
	BOOL write_ini=TRUE;
	BOOL start_server=TRUE;
	char mip[512]="";
	char dtm[512]="";
	char usr[512]="";
	char pas[512]="";
	char dpt[80]="";
	char mpt[80]="";

	char banner[512]="";
	char servername[80]="";
	nproto.senddata("Welcome to server... first running mode\nInstallation program");
	
	while (pass_a==FALSE)
	{
		nproto.senddata("\nDo you want start server when windows start [Y,N]: ");
		nproto.getdnline(dtm,512);
		if (dtm[0]=='Y' || dtm[0]=='y')
		{
			pass_a=TRUE;
		}
		else
		{
			if (dtm[0]=='N' || dtm[0]=='n')
			{
	            start_server=FALSE;
				pass_a=TRUE;
			}
		}
	}
	//Segmento A pasado...
	memset(&banner,0,512);
	memset(&servername,0,80);

	nproto.senddata("\nInsert a Banner (format $ admited): ");
	nproto.getdnline(banner,512);
	fns.denter(banner);

	nproto.senddata("\nInsert a Server Name: ");
	nproto.getdnline(servername,80);
	fns.denter(servername);

	nproto.senddata("\nInsert a port [3359]: ");
	nproto.getdnline(dpt,80);
	fns.denter(dpt);
	
	if (!strcmp(dpt,"")) strcpy(dpt,"3359");
	else
	{
		int frp=atoi(dpt);
		itoa(frp,dpt,10);
	}

	nproto.senddata("\nInsert a mother name: ");
	nproto.getdnline(mip,512);
	fns.denter(mip);


	nproto.senddata("\nInsert a mother port [3359]: ");
	nproto.getdnline(mpt,80);
	fns.denter(mpt);
	
	if (!strcmp(mpt,"")) strcpy(mpt,"3359");
	else
	{
		int frpa=atoi(mpt);
		itoa(frpa,mpt,10);
	}

	nproto.senddata("\nUsername: ");
	nproto.getdnline(usr,512);
	fns.denter(usr);

	nproto.senddata("\nPassword: ");
	nproto.getdnpass(pas,512);
	fns.denter(pas);

	nproto.senddata("\nOK+");
	
	memset(&rcv,0,3);
	nproto.senddata("\nAll Esencial information filled.");
	while (pass_c==FALSE)
	{
		nproto.senddata("\nSave this config? [Y,N]: ");
		nproto.getdnline(dtm,512);
		if (dtm[0]=='Y' || dtm[0]=='y')	pass_c=TRUE;
		else
		{	
			if (dtm[0]=='N' || dtm[0]=='n')
			{
	            write_ini=FALSE;
				pass_c=TRUE;
			}
		}
	}

	if (write_ini)
	{

		data_g.PutData("URCS","server_banner",banner);
		data_g.PutData("URCS","server_prompt","[$d $u]#");
		data_g.PutData("URCS","server_name",servername);
		data_g.PutData("URCS","server_port",dpt);
		data_g.PutData("URCS","mother_name",mip);
		data_g.PutData("URCS","mother_port",mpt);
		data_g.PutData("URCS","server_crypted","F");

		data_g.PutData("PASSWORDS",usr,fns.md5sum(pas));
		data_g.PutData("UGROUP",usr,"admin");
	
		//IF Start on init, copy itself to windows directory and
		//Make a modification on registry... /s
		//HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run
		if (start_server)
		{
			data_g.CopyMeToWinDir();
			if (!data_g.PutKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "URCS","rms"))
				nproto.senddata("\nRegistry writing failed.");
		}
		nproto.senddata("\nwrited.\n");
	}
}



void Cintep::prg_net_lookup(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	fns.denter(mx[xlogon].cmdline);
	if (!strcmp(mx[xlogon].cmdline+10,""))
		nproto.senddata("NetLookup 1.0\nUSAGE: net_lookup hostname\n");
	else
	{
		struct sockaddr_in Server_add;
		struct hostent *hentry;
		if ((hentry = gethostbyname(mx[xlogon].cmdline+11)) == NULL)
			nproto.senddata("Cannot resolve hostname\n");
		else
		{
			//data resolved... 
			char dottedip[80];
			memset((char *)&Server_add, 0, sizeof(Server_add));
			memcpy(&Server_add.sin_addr.s_addr, *(hentry->h_addr_list),sizeof(Server_add.sin_addr.s_addr));
			//host are copied to Server_add
			sprintf(dottedip,"IP: %s\n",inet_ntoa(Server_add.sin_addr));
			nproto.senddata(dottedip);
		}
	}
}
void Cintep::prg_net_send(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	
	char exlusive[4096];
	char respp[12];
	char respl[12];
	char resph[12];

	BOOL bresph=0;

	strncpy(respp,fns.deparg(mx[xlogon].cmdline,"-p",FALSE),12);
	strncpy(respl,fns.deparg(mx[xlogon].cmdline,"-l",FALSE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns.denter(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline+strlen("NET_SEND "),""))
	{
		nproto.senddata("USAGE: net_send [-l long] -p ticket [data]\n");
		return;
	}
	else
		strncpy(exlusive,mx[xlogon].cmdline+strlen("NET_SEND "),4000);

	if (bresph)
		nproto.senddata("net_send [-l long] -p ticket [data]\n-l use this long\n\r-p Ticket (Socket Number)\n[Data] Formmated data such as: Hola$sComo$sEstas\n");
	else
	{
		//Do operations...
		SOCKET p=atoi(respp);
		char mfs[4096];
		int r=-1;
		size_t lng=0;
		memset(&mfs,0,4096);
		strncpy(mfs,fns.convert(exlusive,mx, xlogon),4096);
		if (strcmp(respl,"-l"))
			lng=atoi(respl);
		else
			lng=strlen(mfs);
		r=send(p,mfs,(int)lng,0);
		if (r<=0)
			nproto.senddata("Can't send data\n");
		else
		{
			char ds[80];
			sprintf(ds,"%u bytes sended succesfull to socket %u\n",r,p);
			nproto.senddata(ds);
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

	strncpy(respp,fns.deparg(mx[xlogon].cmdline,"-p",FALSE),12);
	strncpy(respl,fns.deparg(mx[xlogon].cmdline,"-l",FALSE),12);

	strncpy(respr,fns.deparg(mx[xlogon].cmdline,"-r",TRUE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(respr,"-r"))
		brespr=1;
	if (strcmp(resph,"-h"))
		bresph=1;
	if (!strcmp(respp,"-p"))
		brespp=1;
	fns.denter(mx[xlogon].cmdline);

	if (bresph || brespp)
		nproto.senddata("net_receive [-r] [-l long] -p ticket\n-l use this long (default is 4096)\n-p Ticket (Socket Number)\n-r Show in hexadecimal\n");
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
		r=recv(p,mfs,lng,0);
		if (r<=0)
			nproto.senddata("Can't Receive data");
		else
		{
			char ds[100]="";
			sprintf(ds,"%u bytes received succesfully from socket %u:\n\r",r,p);
			nproto.senddata(ds);
			if (!brespr)
				nproto.senddata(mfs);
			else
			{
				//send byte to byte 23 hex's by line.
				nproto.senddata("---------HEXADECIMAL VIEW-------------------------------------------\n");
				int ha=0;
				for (int h=0; h<r; h++)
				{
					if (ha==23)
					{
						nproto.senddata("\n");
						ha=0;
					}
					else ha++;
					char snd[80];
					sprintf(snd,"%02X ",mfs[h]);
					nproto.senddata(snd);
				}
				nproto.senddata("\n---------NORMAL VIEW------------------------------------------------\n");
				nproto.senddata(mfs);
			}
			nproto.senddata("\n\r");
		}
	}
}
void Cintep::prg_net_opensocket(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;

	char exlusive[512];
	char respp[12];
	char resph[12];

	BOOL bresph=0;
	BOOL brespu=0;

	strncpy(respp,fns.deparg(mx[xlogon].cmdline,"-p",FALSE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns.denter(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline+strlen("NET_OPENSOCKET "),""))
	{
		nproto.senddata("Need IP to connecto to: net_opensocket -p port [ip]\n");
		return;
	}
	else
		strncpy(exlusive,mx[xlogon].cmdline+strlen("NET_OPENSOCKET "),512);

	if (bresph)
		nproto.senddata("net_opensocket -p port [ip]\n\r-p Port Number 1-65535\n\r[ip] IP Address such as: 127.0.0.1\n\r");
	else
	{
		//Do operations...
		int port=23;
		port=atoi(respp);
		struct sockaddr_in Server_add;
		WORD wVersionRequested;
		WSADATA wsaData;
		// Start Winsock
		wVersionRequested = MAKEWORD( 1, 0 );
		if( WSAStartup( wVersionRequested, &wsaData ) == 0 )
		{
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
				nproto.senddata("Socket Number: ");
				nproto.senddata(socke);
				nproto.senddata("\n\r");
			}
			else
			{
				nproto.senddata("Host is unavailable.\n");
			}
		}
		else
		{
			nproto.senddata("Can´t initialize socket.\n");
		}
	}

}
void Cintep::prg_net_closesocket(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	nproto.senddata("Closesocket program\n\r");
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
		nproto.senddata("error closing... A successful WSAStartup call must occur before using this function.\n");
		okq=0;
	}
	if (rspw==WSAENETDOWN)
	{
		nproto.senddata("The network subsystem has failed.\n");
		okq=0;
	}
	if (rspw==WSAENOTSOCK)
	{
		nproto.senddata("The descriptor is not a socket.\n");
		okq=0;
	}
	if (rspw==WSAEINTR)
	{
		nproto.senddata("The (blocking) Windows Socket 1.1 call was canceled through WSACancelBlockingCall.\n");
		okq=0;
	}
	if (rspw==WSAEWOULDBLOCK)
	{
		nproto.senddata("The socket is marked as nonblocking and SO_LINGER is set to a nonzero time-out value.\n");
		okq=0;
	}
	if (rspw==WSAEINPROGRESS)
	{
		nproto.senddata("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.\n");
		okq=0;
	}
	if(okq)
	{
		if (rspw>=0)
			nproto.senddata("Socket Closed.\n");
		else
			nproto.senddata("Unknown error\n");
	}

}

void Cintep::prg_prompt(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	//Change prompt of the program...
	//format:
	// prompt 
	// $d directory
	// $f date
	// $h hour
	// $s space
	// $C Color
	data_g.PutData("URCS","server_prompt",mx[xlogon].cmdline+7);
	data_g.LoadData();
	nproto.senddata("Prompt changed...\n\r");
}


void Cintep::prg_kill(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	//Kill a process..
	//Command: KILL
	int pid=atoi(mx[xlogon].cmdline+5); //convert input to integer
	fns.KillProcess(pid); //Send termination signal
	nproto.senddata("Termination Signal Sended...\n"); //Send message...
}

void Cintep::prg_rmuser(con_v mx[SERVER_CONNECTIONS], int xlogon) //deletes user
{
	passed=TRUE;
	// rmuser name
    fns.denter(mx[xlogon].cmdline); //filter username
	data_g.PutData("PASSWORDS",mx[xlogon].cmdline+6,NULL); //deletes entry for user
	data_g.PutData("UGROUP",mx[xlogon].cmdline+6,NULL); //deletes group
	nproto.senddata("rmuser name - Remove user utility... excecuted\n\r"); //inform
}

void Cintep::prg_mkuser(con_v mx[SERVER_CONNECTIONS], int xlogon) //make user
{
	passed=TRUE;
    // mkuser will be executed now...
	// format: mkuser -l [level] -p [password] name
	// if not password, will request this data
	// if not level, admin mode is taked.
	char exlusive[512];
	char respp[256];
	char respl[12];
	char resph[12];
	BOOL bresph=0;

	strncpy(respp,fns.deparg(mx[xlogon].cmdline,"-p",FALSE),256);
	strncpy(respl,fns.deparg(mx[xlogon].cmdline,"-l",FALSE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns.denter(mx[xlogon].cmdline);
	if (!strcmp(mx[xlogon].cmdline+6,""))
	{
		nproto.senddata("Need user... more: mkuser -h\n");
		return;
	}
	else
		strncpy(exlusive,mx[xlogon].cmdline+6,512);

	if (bresph)
		nproto.senddata("mkuser -l [level] -p [password] name\n");
	else
	{
		//do...
		char u_level[80]="admin";
		char u_pass[512];
		char u_name[80];
        //Verify for existence of level...
		if (strcmp(respl,"-l")) // level defined...
			strcpy(u_level,respl);
		if (!strcmp(respp,"-p")) // password not defined...
		{
			nproto.senddata("Insert password: "); //prompt for password
			nproto.getdnpass(u_pass,512); //get password
			fns.denter(u_pass); //filter.
		}
		else
		{
			strcpy(u_pass,respp); //copy from command line
			fns.denter(u_pass); //filter
		}
		strncpy(u_name,mx[xlogon].cmdline+6,80);

		if(data_g.PutData("PASSWORDS",u_name,fns.md5sum(u_pass)))
			nproto.senddata("Password changed...\n");
		else
			nproto.senddata("Cant change account password...\n");
		if(data_g.PutData("UGROUP",u_name,u_level))
			nproto.senddata("level changed...\n");
		else
			nproto.senddata("Cant change account password...\n");
	}
}

void Cintep::prg_passwd(con_v mx[SERVER_CONNECTIONS], int xlogon) //change password
{
	passed=TRUE;
	//Change User's password
	if (!strcmp(mx[xlogon].cmdline+7,"")) // prompt for password if not specified
	{
		nproto.senddata("Insert password: ");
		nproto.getdnpass(mx[xlogon].cmdline+7,512-7);
	}
	fns.denter(mx[xlogon].cmdline); //filter's password
	/*do change.*/
	if(data_g.PutData("PASSWORDS",mx[xlogon].c_User,fns.md5sum(mx[xlogon].cmdline+7)))
		nproto.senddata("\n\rPassword changed...\n\rpasswd [password]\n\r");
	else
		nproto.senddata("Password not changed...");
}

void Cintep::prg_who(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	char exterm[512]; 
	nproto.senddata("  UAID SOCKid            Username:  Conected since:\n");
	for(int n=0;n<SERVER_CONNECTIONS;n++)
	{
		if (mx[n].busy==TRUE)
		{
			sprintf(exterm," %5u  %5u   %18s  %s\r",n,mx[n].socket,mx[n].c_User, ctime(&mx[n].since));
			nproto.senddata(exterm);
		}
	}
}

void Cintep::prg_cat(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	char respt[12]="";
	char resph[12]="";

	strncpy(respt,fns.deparg(mx[xlogon].cmdline,"-t",TRUE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
	{
		nproto.senddata("Format: cat [-t] [-h] file\n\r-t Complete 10,13 bytes\n\r-h help\n\r");
		return;
	}
	fns.denter(mx[xlogon].cmdline);
	char buffer[256];
	memset(&buffer,0,256);
	int fh;
	unsigned int nbytes = 256, bytesread;
	if( (fh = _open(mx[xlogon].cmdline+4 , _O_RDONLY )) == -1 )
	{
		nproto.senddata("Cannot open local file on telnet server...");
	}
	else
	{
		while( ( bytesread = _read( fh, buffer, nbytes ) ) > 0 && fh!=-1 )
		{
			buffer[bytesread]=0;
			if (strcmp(respt,"-t"))
				fns.renter(buffer);
			nproto.senddata(buffer);
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
	fns.denter(recpt2); 

	if (_chdir(recpt2)==-1) //Change directory...
		nproto.senddata("Can't locate specified directory\n\r"); //error 

	_getcwd( buff, _MAX_PATH );   /* Get the current working directory: */
	strncpy(mx[xlogon].localdir,buff, _MAX_PATH); //copy it to "user specifications"...
}
void Cintep::prg_ps(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	nproto.senddata("PSLIST Internal Program 1.0\n\r");
	if (mx[xlogon].cmdline[3]=='-' && mx[xlogon].cmdline[4]=='h')
	{
		nproto.senddata("#PS is a internal program who show all aplications\n\rpid's and ppids (process id) & (parent Process ID)\n\rYou can also use: KILL (Terminate program by pid)\n\r");
	}
	else
	{
		nproto.senddata("\n\r   PPID  PID   APPNAME\n\r");
		long r;
		HANDLE hSnapShot;
		PROCESSENTRY32 uProcess;
		hSnapShot = CreateToolhelp32Snapshot (TH32CS_SNAPALL, NULL);
		uProcess.dwSize= sizeof(uProcess);
		r = Process32First (hSnapShot, &uProcess );
		while (r)
		{
			char exterm[512];
			if (!data_g.nt)
			{
	            char cmpare[512];
				LPSTR pCurChar;
				for (pCurChar = (uProcess.szExeFile + lstrlen (uProcess.szExeFile));
					*pCurChar != '\\' && pCurChar != uProcess.szExeFile; 
					--pCurChar)
				strcpy(cmpare,pCurChar);
				sprintf(exterm," %5u %5u   %s\n\r",uProcess.th32ParentProcessID,uProcess.th32ProcessID,cmpare);
			}
			else //WinNT! (names are in low format)
			{
				sprintf(exterm," %5u %5u   %s\n\r",uProcess.th32ParentProcessID,uProcess.th32ProcessID,uProcess.szExeFile );
			}
			nproto.senddata(exterm);
			r = Process32Next(hSnapShot, &uProcess );
		}
	}

}

void Cintep::prg_rm(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	char respy[12]="";
	char resph[12]="";
	BOOL bOK=TRUE; 
	strncpy(respy,fns.deparg(mx[xlogon].cmdline,"-y",TRUE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);
	if (strcmp(resph,"-h"))
		nproto.senddata("Format: rm [-y] [-h] file (can be formatted such as: archivo$s1.txt)\n\r-y don't query\n\r-h help\n\r");
	if (!strcmp(respy,"-y"))
	{
		char msw[20];
		nproto.senddata("Press N to cancel delete file operation: ");
		nproto.getdnline(msw,20);
		strupr(msw);
		if (msw[0]=='N')
			bOK=FALSE;
	}
	if (bOK==TRUE)
	{
		fns.denter(mx[xlogon].cmdline);
		
		if( DeleteFile( fns.convert(mx[xlogon].cmdline+3,mx, xlogon) ) == -1 )
			nproto.senddata( "Could not delete the file\n");
		else
			nproto.senddata("Deleted.\n\r");
	}
	else
		nproto.senddata("not deleted.\n\r");
}

void Cintep::prg_cp(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//Copy file
	passed=TRUE;

	char exlusive[1024];
	char respd[1024];
	char respo[12];
	char resph[12];

	BOOL bresph=0;
	BOOL brespo=0;

	strncpy(respd,fns.deparg(mx[xlogon].cmdline,"-d",FALSE),1024);
	strncpy(respo,fns.deparg(mx[xlogon].cmdline,"-o",TRUE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;
	if (strcmp(respo,"-o"))
		brespo=1;
	fns.denter(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline+3,""))
	{
		nproto.senddata("USAGE: cp source [-o] -d destination\n-o Overwrite\n");
		return;
	}
	else
		strncpy(exlusive,mx[xlogon].cmdline+3,1024);

	if (bresph || !strcmp(respd,"-d")) //incomplete args, or -h help request
		nproto.senddata("cp source [-o] -d destination\n\r-d Destination Filename\n\rsource: Source FileName\n\r[Data] Compatible with formmated data such as: Hola$sComo$sEstas$n\n\r");
	else
	{
		//Do operations...
		char csa[1024];

		char src[1024];
		char dst[1024];

		strncpy(dst,fns.convert(respd,mx, xlogon),1024); //convert destination
		strncpy(src,fns.convert(exlusive,mx, xlogon),1024); //convert source
		
		sprintf(csa,"Copying %s to %s...",src,dst);
		nproto.senddata(csa);

		if (CopyFile(src,dst,!brespo))
			nproto.senddata("Completed.\n");
		else
			nproto.senddata("fail.\n");
	}
}

void Cintep::prg_mv(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//Copy file
	passed=TRUE;

	char exlusive[1024];
	char respd[1024];
	char respo[12];
	char resph[12];

	BOOL bresph=0;
	BOOL brespo=0;

	strncpy(respd,fns.deparg(mx[xlogon].cmdline,"-d",FALSE),1024);
	strncpy(respo,fns.deparg(mx[xlogon].cmdline,"-o",TRUE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;
	if (strcmp(respo,"-o"))
		brespo=1;
	fns.denter(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline+3,""))
	{
		nproto.senddata("USAGE: mv source [-o] -d destination\n-o Overwrite\n");
		return;
	}
	else
		strncpy(exlusive,mx[xlogon].cmdline+3,1024);

	if (bresph || !strcmp(respd,"-d")) //incomplete args, or -h help request
		nproto.senddata("mv source [-o] -d destination\n\r-d Destination Filename\n\rsource: Source FileName\n\r[Data] Compatible with formmated data such as: Hola$sComo$sEstas$n\n\r");
	else
	{
		//Do operations...
		char csa[1024];

		char src[1024];
		char dst[1024];

		strncpy(dst,fns.convert(respd,mx, xlogon),1024); //convert destination
		strncpy(src,fns.convert(exlusive,mx, xlogon),1024); //convert source
		
		sprintf(csa,"Moving %s to %s...",src,dst);
		nproto.senddata(csa);
		
		//CopyFile(src,dst,!brespo)
		if (brespo) 
		{
			if (MoveFileEx(src,dst,MOVEFILE_REPLACE_EXISTING)) nproto.senddata("Completed.\n");
			else nproto.senddata("fail.\n");
		}
		else
		{
			if (MoveFile(src,dst))
				nproto.senddata("Completed.\n");
			else nproto.senddata("fail.\n");
		}
	}
}

void Cintep::prg_exec(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	// Format: winexec [-e Exec_Number] path
	char respe[512];
	char resph[12];
	int execution=SW_SHOWDEFAULT;
	strncpy(respe,fns.deparg(mx[xlogon].cmdline,"-e",FALSE),512); 
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
	{
		//showhelp
		nproto.senddata("Format: winexec [-e Exec_Number] path\n\rExcecute an program file\n\rExec Number:\n\r");
	}
	else
	{
		char lcs[1024];
		if (strcmp(respe,"-e"))
		{
			//respe founded... 
			execution = atoi(respe);
		}
		fns.denter(mx[xlogon].cmdline);
		sprintf(lcs,"Excecuting: %s \n",fns.convert(mx[xlogon].cmdline+8,mx, xlogon));
		nproto.senddata(lcs);
		int x=WinExec(fns.convert(mx[xlogon].cmdline+8,mx, xlogon),execution);
		if (x==0)
			nproto.senddata("The system is out of memory or resources.\n\r");
		else
		{
			if (x==ERROR_BAD_FORMAT)
				nproto.senddata("The .EXE file is invalid (non-Win32 .EXE or error in .EXE image).\n\r");
			else
			{
				if (x==ERROR_FILE_NOT_FOUND) 
					nproto.senddata("The specified file was not found.\n\r");
				else
				{
					if (x==ERROR_PATH_NOT_FOUND) 
						nproto.senddata("The specified path was not found.\n\r");
					else
						nproto.senddata("Execution Completed\n\r");
				}
			}
		}
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
	//	char filez[1024];
	char exlusive[1024];
	char respd[12];
	char respl[12];
	char respc[12];
	char resph[12];

	BOOL brespd=FALSE;
	BOOL brespl=FALSE;
	BOOL brespc=FALSE;
	BOOL bresph=FALSE;

	strncpy(respd,fns.deparg(mx[xlogon].cmdline,"-d",TRUE),12);
	strncpy(respl,fns.deparg(mx[xlogon].cmdline,"-l",TRUE),12);
	strncpy(respc,fns.deparg(mx[xlogon].cmdline,"-c",TRUE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(respd,"-d"))
		brespd=TRUE;
	if (strcmp(respl,"-l"))
		brespl=TRUE;
	if (strcmp(respc,"-c"))
		brespc=TRUE;
	if (strcmp(resph,"-h"))
		bresph=TRUE;

	fns.denter(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline+3,""))
	{
		strcpy(exlusive,"*");
	}
	else
	{
		strncpy(exlusive,mx[xlogon].cmdline+3,1024);
	}
	if (bresph==TRUE)
	{
		nproto.senddata("LS INTERNAL UTILITY\n\r-l list mode\n\r-h help\n\r-c Show all data\n\r-d Shows directory\n\r");
	}
	else
	{
		//Exlusive contain a comodin path
		WIN32_FIND_DATA ffd; // file information struct
		HANDLE sh = FindFirstFile(exlusive, &ffd);
		if(INVALID_HANDLE_VALUE == sh)
		{
			nproto.senddata("LS: not a proper path i guess\n");
			return; // not a proper path i guess
		}
		do 
		{
			// get format:
			if (brespc==TRUE || brespl==TRUE)
			{
				//All file data...
				char datafile[1500];
				time_t fct=fns.TimetFromFt(ffd.ftCreationTime);
				time_t flat=fns.TimetFromFt(ffd.ftLastAccessTime);
				time_t flwt=fns.TimetFromFt(ffd.ftLastWriteTime);

				if (brespc)
				{
					sprintf(datafile,"*****************************\n\rFilename: %s\n\rFile Alternate name: %s\n\rFile Size High: %u\n\rFile Size Low: %u\n\rFile Creation time: %s\rFile Last Access time: %s\rFile Last Write time: %s",
					ffd.cFileName,
					ffd.cAlternateFileName,
					ffd.nFileSizeHigh,
					ffd.nFileSizeLow,
					ctime(&fct),
					ctime(&flat),
					ctime(&flwt)
					);
					nproto.senddata(datafile);
					nproto.senddata("\n\r");
				}
				if (brespl)
				{
					
					char ed[80];
					strcpy(ed,ctime(&flwt));
					fns.denter(ed);
					sprintf(datafile,"%25s %7u %s",	ed,ffd.nFileSizeLow,ffd.cFileName);
					nproto.senddata(datafile);
					nproto.senddata("\n\r");

				}

			}
			else
			{
				if (ffd.dwFileAttributes == 0x10 || ffd.dwFileAttributes == 0x11 || ffd.dwFileAttributes == 0x12 || ffd.dwFileAttributes == 0x14 || ffd.dwFileAttributes == 0x18)
				{
					nproto.senddata(ffd.cFileName);
					nproto.senddata("\n\r");
				}
				else
				{
					if (!brespd)
					{
						nproto.senddata(ffd.cFileName);
						nproto.senddata("\n\r");
					}
				}
			}

		} while (FindNextFile(sh, &ffd));
		FindClose(sh);
	}
	//End!
}

void Cintep::prg_sleep(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: Sleep
{
	passed=TRUE;
    //Sleep an amount of time
	fns.denter(mx[xlogon].cmdline);
	if (!strcmp(mx[xlogon].cmdline+5,"")) nproto.senddata("Sleep 1.0 - usage: sleep [sleep time, milliseconds]\n");
	else
	{	
		DWORD tts=(DWORD)_atoi64(mx[xlogon].cmdline+5);
		Sleep(tts);
	}
}
void Cintep::prg_md5(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: MD5 string
{
	passed=TRUE;
	fns.denter(mx[xlogon].cmdline);
	nproto.senddata(fns.convert(fns.md5sum(fns.convert(mx[xlogon].cmdline+4,mx, xlogon)),mx, xlogon));
	nproto.senddata("\n");
}

void Cintep::prg_echo(con_v mx[SERVER_CONNECTIONS], int xlogon)
{ // Internal Program Echo
	passed=TRUE;
	nproto.senddata(fns.convert(mx[xlogon].cmdline+5,mx, xlogon));
}

void Cintep::start_intep(con_v mx[SERVER_CONNECTIONS], int local_user, char *newkey)
{ //Start Interpreter
	nproto.continue_socket(mx[local_user].socket,newkey);
}

int	Cintep::printprompt(con_v mx[SERVER_CONNECTIONS], int local_user)
{ //print prompt specified in data_g and converted with "convert"
	return nproto.senddata(fns.convert(data_g.server_prompt,mx,local_user));
}


int Cintep::run(con_v mx[SERVER_CONNECTIONS], ers_svr svrs[SERVER_SLOTS],int local_user)
{
	// run a command line, first, process command line.
	_chdir(mx[local_user].localdir); // Change dir to user dir...
	passed=0;
	if (mx[local_user].user_range==0 || mx[local_user].user_range==1 || mx[local_user].user_range==2 || mx[local_user].user_range==3 || mx[local_user].user_range==4) // usuario/admin/chat/otros puede ejecutar esto...
	{
		//comando time...
		if (fns.cmpfirstword(mx[local_user].cmdline,""))
			passed=1;
		if (fns.cmpfirstword(mx[local_user].cmdline,"SLEEP"))
			prg_sleep(mx,local_user);
		if (fns.cmpfirstword(mx[local_user].cmdline,"MD5"))
			prg_md5(mx,local_user);
		if (fns.cmpfirstword(mx[local_user].cmdline,"ECHO"))
			prg_echo(mx,local_user);
	}
//	if (mx[local_user].user_range==0 || mx[local_user].user_range==1 || mx[local_user].user_range==2) // usuario/admin/chat puede ejecutar esto...
//	{
/*		if (fns.cmpfirstword(recpt,"SENDMAIL"))
			sendmail(d,recpt,dskq);
		if (fns.cmpfirstword(recpt,"MAIL"))
			mail(d,recpt,dskq);*/
//	}
	if (mx[local_user].user_range==0 || mx[local_user].user_range==1 || mx[local_user].user_range==4) // usuario/admin/route puede ejecutar esto...
	{
		if (fns.cmpfirstword(mx[local_user].cmdline,"PRX_CONNECT"))
			prg_prx_connect(mx,svrs,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"PRX_WHO"))
			prg_prx_who(mx,svrs,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"NET_LOOKUP"))
			prg_net_lookup(mx,local_user);
	}
	if (mx[local_user].user_range==0 || mx[local_user].user_range==1) //Solo admin/user puede ejecutar...:
	{
		if (fns.cmpfirstword(mx[local_user].cmdline,"PRX_CLOSE"))
			prg_prx_close(mx,svrs,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"LS"))
			prg_ls(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"WINEXEC"))
			prg_exec(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"CP"))
			prg_cp(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"MV"))
			prg_mv(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"RM"))
			prg_rm(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"PS"))
			prg_ps(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"CD"))
			prg_cd(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"CAT"))
			prg_cat(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"WHO"))
			prg_who(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"HELP"))
			prg_lscmd(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"PASSWD"))
			prg_passwd(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"UPLOADTO"))
			prg_uploadto(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"DOWNLOADFROM"))
			prg_downloadfrom(mx,local_user);

	}
	if (mx[local_user].user_range==0) //Solo admin puede ejecutar...: (comandos de config del server)
	{
		if (fns.cmpfirstword(mx[local_user].cmdline,"NET_SEND"))
			prg_net_send(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"NET_RECEIVE"))
			prg_net_receive(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"NET_OPENSOCKET"))
			prg_net_opensocket(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"NET_CLOSESOCKET"))
			prg_net_closesocket(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"RMUSER"))
			prg_rmuser(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"MKUSER"))
			prg_mkuser(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"KILL"))
			prg_kill(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"PROMPT"))
			prg_prompt(mx,local_user);
	}
	if (fns.cmpfirstword(mx[local_user].cmdline,"EXIT"))
	{
			passed=TRUE;
			return -1;
	}
	return 1;
}


void Cintep::prg_lscmd(con_v mx[SERVER_CONNECTIONS], int xlogon) //help
{
	passed=TRUE;
	nproto.senddata("Welcome to unmanarc remote control server...\nlist of commands:\n\n");
	nproto.senddata("echo           : Echo characters introduced by command line\n");
	nproto.senddata("MD5            : MD5 string introduced in command line\n");
	nproto.senddata("Sleep          : Wait [n] milliseconds\n");
	nproto.senddata("ls             : list files\n");
	nproto.senddata("winexec        : run process in [n] mode\n");
	nproto.senddata("cp             : Copy File in another location\n");
	nproto.senddata("mv             : Move File in another location\n");
	nproto.senddata("rm             : remove file\n");
	nproto.senddata("ps             : List Proccesses\n");
	nproto.senddata("cd             : change directory\n");
	nproto.senddata("cat            : show file\n");
	nproto.senddata("who            : show information about users connected to the system\n");
	nproto.senddata("passwd         : change's password of local user\n");
	nproto.senddata("\n");
	nproto.senddata("kill           : terminate PID(must be admin)\n");
	nproto.senddata("prompt         : change prompt(must be admin)\n");
	nproto.senddata("mkuser         : creates another user (must be admin)\n");
	nproto.senddata("rmuser         : remove user from database(must be admin)\n");
	nproto.senddata("\n");
	nproto.senddata("net_lookup     : resolves name into dotted ip addr.\n");
	nproto.senddata("net_opensocket : Open TCP raw connection to IP(must be admin)\n");
	nproto.senddata("net_send       : Send data to TCP socket(must be admin)\n");
	nproto.senddata("net_receive    : Receive data from TCP socket(must be admin)\n");
	nproto.senddata("net_closesocket: Close opened socket(must be admin)\n");
	nproto.senddata("\n");
	nproto.senddata("prx_connect    : tunnel to specified server\n");
	nproto.senddata("prx_who        : List tunnels\n");
	nproto.senddata("prx_close      : Restart connection to tunnel\n");
	nproto.senddata("\n");
	nproto.senddata("downloadfrom   : download file from CTOOL fileserver\n");
	nproto.senddata("uploadto       : upload file to CTOOL fileserver\n");
	nproto.senddata("\n");
	nproto.senddata("help           : show this help\n");
	nproto.senddata("\nNOTE: some program's may not work if you don't have privilege.\n");
}
