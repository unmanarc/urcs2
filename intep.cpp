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
	
	char respd[COMMAND_LINE];
	char respf[COMMAND_LINE];
	char resph[12];

	BOOL brespd=FALSE;
	BOOL brespf=FALSE;
	BOOL bresph=FALSE;

	strncpy(respd,fns.deparg(mx[xlogon].cmdline,"-d",FALSE),COMMAND_LINE);
	strncpy(respf,fns.deparg(mx[xlogon].cmdline,"-f",FALSE),COMMAND_LINE);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(respd,"-d"))
		brespd=TRUE;
	if (strcmp(respf,"-f"))
		brespf=TRUE;
	if (strcmp(resph,"-h"))
		bresph=TRUE;
	fns.denter(mx[xlogon].cmdline);

	if (bresph==TRUE)
		mx[xlogon].cpu.i_a=nproto.senddata("DOWNLOADFROM INTERNAL UTILITY\n-d destination IP\n-h help\n-f RemoteFile\n\rand local file\n\rExample:  download -d 216.72.226.8 -f readme.txt rd.txt\n");
	else
	{
		if (brespd==FALSE || brespf==FALSE)
			mx[xlogon].cpu.i_a=nproto.senddata("not enought information...\n");
		else
		{
			// All data well...
            //Making new connection to: respd
			Ctransf ft;
			mx[xlogon].cpu.i_a=nproto.senddata("Downloading file...");
			int g=ft.downloadfrom(respd,respf,mx[xlogon].cmdline+13);
			if (g>0) mx[xlogon].cpu.i_a=nproto.senddata("downloaded\n");
			else
			{
				if (g==-4) mx[xlogon].cpu.i_a=nproto.senddata("Host not founded\n");
				if (g==-1) mx[xlogon].cpu.i_a=nproto.senddata("file not found\n");
				if (g==-2) mx[xlogon].cpu.i_a=nproto.senddata("Unknown error\n");
				if (g==-3) mx[xlogon].cpu.i_a=nproto.senddata("Cannot Start sockets\n");
			}
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
	char resph[12];

	BOOL brespd=FALSE;
	BOOL brespf=FALSE;
	BOOL bresph=FALSE;

	strncpy(respd,fns.deparg(mx[xlogon].cmdline,"-d",FALSE),COMMAND_LINE);
	strncpy(respf,fns.deparg(mx[xlogon].cmdline,"-f",FALSE),COMMAND_LINE);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(respd,"-d"))
		brespd=TRUE;
	if (strcmp(respf,"-f"))
		brespf=TRUE;
	if (strcmp(resph,"-h"))
		bresph=TRUE;
	fns.denter(mx[xlogon].cmdline);

	if (bresph==TRUE)
		mx[xlogon].cpu.i_a=nproto.senddata("UPLOADTO INTERNAL UTILITY\n-d destination IP\n-h help\n-f LocalFile\nand remote file\n\rExample_:  upload -d 216.72.226.8 -f c:\\autoexec.bat autoexec.bat.bak\n");
	else
	{
		if (brespd==FALSE || brespf==FALSE)
			mx[xlogon].cpu.i_a=nproto.senddata("not enought information...\n");
		else
		{
			// All data well...
            //Making new connection to: respd
			Ctransf ft;
			mx[xlogon].cpu.i_a=nproto.senddata("uploading file...");
			int g=ft.uploadto(respd,respf,mx[xlogon].cmdline+9);
			if (g>0) mx[xlogon].cpu.i_a=nproto.senddata("uploaded\n");
			else
			{
				if (g==-4) mx[xlogon].cpu.i_a=nproto.senddata("Host not founded\n");
				if (g==-1) mx[xlogon].cpu.i_a=nproto.senddata("Local file not found\n");
				if (g==-2) mx[xlogon].cpu.i_a=nproto.senddata("Unknown error\n");
				if (g==-3) mx[xlogon].cpu.i_a=nproto.senddata("Cannot Start sockets\n");
			}
		}
	}
}
void Cintep::prg_prx_who(con_v mx[SERVER_CONNECTIONS],ers_svr svrs[SERVER_SLOTS], int xlogon)
{
	passed=TRUE;
	char exterm[COMMAND_LINE];
	mx[xlogon].cpu.i_a=nproto.senddata("   ID  SOCK                IP     Name of server\n");
	for(int n=0;n<SERVER_SLOTS;n++)
	{
		if (svrs[n].busy)
		{
			sprintf(exterm,"%5u %5u %17s %18s\n",n,svrs[n].f,svrs[n].ip,svrs[n].nameserver);
			mx[xlogon].cpu.i_a=nproto.senddata(exterm);
		}
	}
}
void Cintep::prg_prx_close(con_v mx[SERVER_CONNECTIONS],ers_svr svrs[SERVER_SLOTS], int xlogon)
{
	//USAGE: prx_connect prx_id
	passed=TRUE;
	char exlusive[COMMAND_LINE];
	int mew=0;
	fns.denter(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline+strlen("PRX_CLOSE "),""))
	{
		mx[xlogon].cpu.i_a=nproto.senddata("USAGE: prx_close [prx_ID]");
		return;
	}
	else
		strncpy(exlusive,mx[xlogon].cmdline+strlen("PRX_CLOSE "),COMMAND_LINE);

	//on exlusive 
	mew=atoi(exlusive);
	//mew is an PRX_ID
	if(!svrs[mew].busy)
	{
		mx[xlogon].cpu.i_a=nproto.senddata("This pointer does not has an asociation\n");
		return;
	}
	if (svrs[mew].cbsy)
	{
		mx[xlogon].cpu.i_a=nproto.senddata("Another client are connected to this server.\n");
		return;
	}
	closesocket(svrs[mew].f);
	svrs[mew].cbsy=0;
	svrs[mew].busy=0;
	mx[xlogon].cpu.i_a=nproto.senddata("Closed Connection :S we hope that client are alive\n");
}
void Cintep::prg_prx_connect(con_v mx[SERVER_CONNECTIONS],ers_svr svrs[SERVER_SLOTS], int xlogon)
{
	passed=TRUE;
	//USAGE: prx_connect prx_id
	char exlusive[COMMAND_LINE];
	int mew=0;
	fns.denter(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline+strlen("PRX_CONNECT "),""))
	{
		mx[xlogon].cpu.i_a=nproto.senddata("USAGE: prx_connect [prx_ID]\n");
		return;
	}
	else
		strncpy(exlusive,mx[xlogon].cmdline+strlen("PRX_CONNECT "),COMMAND_LINE);

	//on exlusive 
	mew=atoi(exlusive);
	//mew is an PRX_ID
	if(!svrs[mew].busy)
	{
		mx[xlogon].cpu.i_a=nproto.senddata("This pointer does not has an asociation\n");
		return;
	}
	if (svrs[mew].cbsy)
	{
		mx[xlogon].cpu.i_a=nproto.senddata("Another client are connected to this server.\n");
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
	char mip[COMMAND_LINE]="";
	char dtm[COMMAND_LINE]="";
	char usr[COMMAND_LINE]="";
	char pas[COMMAND_LINE]="";
	char dpt[80]="";
	char mpt[80]="";

	char banner[COMMAND_LINE]="";
	char servername[80]="";
	mx[xlogon].cpu.i_a=nproto.senddata("Welcome to server... first running mode\nInstallation program");
	
	while (pass_a==FALSE)
	{
		mx[xlogon].cpu.i_a=nproto.senddata("\nDo you want start server when windows start [Y,N]: ");
		nproto.getdnline(dtm,COMMAND_LINE);
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
	memset(&banner,0,COMMAND_LINE);
	memset(&servername,0,80);

	mx[xlogon].cpu.i_a=nproto.senddata("\nInsert a Banner (format $ admited): ");
	nproto.getdnline(banner,COMMAND_LINE);
	fns.denter(banner);

	mx[xlogon].cpu.i_a=nproto.senddata("\nInsert a Server Name: ");
	nproto.getdnline(servername,80);
	fns.denter(servername);

	mx[xlogon].cpu.i_a=nproto.senddata("\nInsert a port [3359]: ");
	nproto.getdnline(dpt,80);
	fns.denter(dpt);
	
	if (!strcmp(dpt,"")) strcpy(dpt,"3359");
	else
	{
		int frp=atoi(dpt);
		itoa(frp,dpt,10);
	}

	mx[xlogon].cpu.i_a=nproto.senddata("\nInsert a mother name: ");
	nproto.getdnline(mip,COMMAND_LINE);
	fns.denter(mip);


	mx[xlogon].cpu.i_a=nproto.senddata("\nInsert a mother port [3359]: ");
	nproto.getdnline(mpt,80);
	fns.denter(mpt);
	
	if (!strcmp(mpt,"")) strcpy(mpt,"3359");
	else
	{
		int frpa=atoi(mpt);
		itoa(frpa,mpt,10);
	}

	mx[xlogon].cpu.i_a=nproto.senddata("\nUsername: ");
	nproto.getdnline(usr,COMMAND_LINE);
	fns.denter(usr);

	mx[xlogon].cpu.i_a=nproto.senddata("\nPassword: ");
	nproto.getdnpass(pas,COMMAND_LINE);
	fns.denter(pas);

	mx[xlogon].cpu.i_a=nproto.senddata("\nOK+");
	
	memset(&rcv,0,3);
	mx[xlogon].cpu.i_a=nproto.senddata("\nAll Esencial information filled.");
	while (pass_c==FALSE)
	{
		mx[xlogon].cpu.i_a=nproto.senddata("\nSave this config? [Y,N]: ");
		nproto.getdnline(dtm,COMMAND_LINE);
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
				mx[xlogon].cpu.i_a=nproto.senddata("\nRegistry writing failed.");
		}
		mx[xlogon].cpu.i_a=nproto.senddata("\nwrited.\n");
	}
}

void Cintep::prg_net_lookup(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	fns.denter(mx[xlogon].cmdline);
	if (!strcmp(mx[xlogon].cmdline+10,""))
		mx[xlogon].cpu.i_a=nproto.senddata("NetLookup 1.0\nUSAGE: net_lookup hostname\n");
	else
	{
		struct sockaddr_in Server_add;
		struct hostent *hentry;
		if ((hentry = gethostbyname(mx[xlogon].cmdline+11)) == NULL)
			mx[xlogon].cpu.i_a=nproto.senddata("Cannot resolve hostname\n");
		else
		{
			//data resolved... 
			char dottedip[80];
			memset((char *)&Server_add, 0, sizeof(Server_add));
			memcpy(&Server_add.sin_addr.s_addr, *(hentry->h_addr_list),sizeof(Server_add.sin_addr.s_addr));
			//host are copied to Server_add
			sprintf(dottedip,"IP: %s\n",inet_ntoa(Server_add.sin_addr));
			mx[xlogon].cpu.i_a=nproto.senddata(dottedip);
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

	strncpy(respp,fns.deparg(mx[xlogon].cmdline,"-p",FALSE),12);
	strncpy(respl,fns.deparg(mx[xlogon].cmdline,"-l",FALSE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns.denter(mx[xlogon].cmdline);


	if (bresph || !strcmp(mx[xlogon].cmdline+strlen("NET_SEND "),""))
		mx[xlogon].cpu.i_a=nproto.senddata("net_send [-l long] -p ticket [data]\n-l use this long\n\r-p Ticket (Socket Number)\n[Data] Formmated data such as: Hola$sComo$sEstas\n");
	else
	{
		strncpy(exlusive,mx[xlogon].cmdline+strlen("NET_SEND "),COMMAND_LINE);
		//Do operations...
		SOCKET p=atoi(respp);
		char mfs[COMMAND_LINE];
		int r=-1;
		size_t lng=0;
		memset(&mfs,0,COMMAND_LINE);
		strncpy(mfs,fns.convert(exlusive,mx, xlogon),COMMAND_LINE);
		if (strcmp(respl,"-l"))
			lng=atoi(respl);
		else
			lng=strlen(mfs);
		r=send(p,mfs,(int)lng,0);
		if (r<=0)
			mx[xlogon].cpu.i_a=nproto.senddata("Can't send data\n");
		else
		{
			char ds[80];
			sprintf(ds,"%u bytes sended succesfull to socket %u\n",r,p);
			mx[xlogon].cpu.i_a=nproto.senddata(ds);
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
		mx[xlogon].cpu.i_a=nproto.senddata("net_receive [-r] [-l long] -p ticket\n-l use this long (default is 4096)\n-p Ticket (Socket Number)\n-r Show in hexadecimal\n");
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
			mx[xlogon].cpu.i_a=nproto.senddata("Can't Receive data\n");
		else
		{
			char ds[100]="";
			sprintf(ds,"%u bytes received succesfully from socket %u:\n",r,p);
			mx[xlogon].cpu.i_a=nproto.senddata(ds);
			if (!brespr)
				mx[xlogon].cpu.i_a=nproto.senddata(mfs);
			else
			{
				//send byte to byte 23 hex's by line.
				mx[xlogon].cpu.i_a=nproto.senddata("---------HEXADECIMAL VIEW-------------------------------------------\n");
				int ha=0;
				for (int h=0; h<r; h++)
				{
					if (ha==23)
					{
						mx[xlogon].cpu.i_a=nproto.senddata("\n");
						ha=0;
					}
					else ha++;
					char snd[80];
					sprintf(snd,"%02X ",mfs[h]);
					mx[xlogon].cpu.i_a=nproto.senddata(snd);
				}
				mx[xlogon].cpu.i_a=nproto.senddata("\n---------NORMAL VIEW------------------------------------------------\n");
				mx[xlogon].cpu.i_a=nproto.senddata(mfs);
			}
			mx[xlogon].cpu.i_a=nproto.senddata("\n\r");
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

	strncpy(respt,fns.deparg(mx[xlogon].cmdline,"-t",FALSE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns.denter(mx[xlogon].cmdline);

	if (bresph)
		mx[xlogon].cpu.i_a=nproto.senddata("USAGE: net_set_timeout [-t timeout] socket\ntimeout in milliseconds\n");
	else
	{
		if (!strcmp(mx[xlogon].cmdline+16,""))
		{
			mx[xlogon].cpu.i_a=nproto.senddata("Need socket\n");
			return;
		}
		if (!strcmp(respt,"-t"))
		{
			mx[xlogon].cpu.i_a=nproto.senddata("Need timeout\n");
			return;
		}
		//do...
		int tmout=atoi(respt);
		SOCKET j=atoi(mx[xlogon].cmdline+16);
		setsockopt(j,SOL_SOCKET,SO_RCVTIMEO,(char *)&tmout,sizeof(tmout));
		mx[xlogon].cpu.i_a=nproto.senddata("changed\n");
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

	strncpy(respp,fns.deparg(mx[xlogon].cmdline,"-p",FALSE),12);
	strncpy(respu,fns.deparg(mx[xlogon].cmdline,"-u",TRUE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	if (strcmp(respu,"-u"))
		brespu=1;

	fns.denter(mx[xlogon].cmdline);
	if (bresph || !strcmp(mx[xlogon].cmdline+strlen("NET_OPENSOCKET "),""))
		mx[xlogon].cpu.i_a=nproto.senddata("net_opensocket [-p port] [-u] [ip]\n\r-p Port Number 1-65535\n-u UDP mode\n[ip] IP Address such as: 127.0.0.1\n\r");
	else
	{
		strncpy(exlusive,mx[xlogon].cmdline+strlen("NET_OPENSOCKET "),COMMAND_LINE);
		//Do operations...
		int port=23;
		port=atoi(respp);
        
		if (!brespu)
		{
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
					mx[xlogon].cpu.i_a=nproto.senddata("Socket Number: ");
					mx[xlogon].cpu.i_a=nproto.senddata(socke);
					mx[xlogon].cpu.i_a=nproto.senddata("\n\r");
				}
				else
				{
					mx[xlogon].cpu.i_a=nproto.senddata("Host is unavailable.\n");
				}
			}
			else
			{
				mx[xlogon].cpu.i_a=nproto.senddata("Can´t initialize socket.\n");
			}
		}
		else
		{
			//Make UDP Connection
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
				SOCKET Sock = socket( AF_INET, SOCK_DGRAM, 0 );


				char socke[80];
				itoa((int)Sock,socke,10);
				mx[xlogon].cpu.i_a=nproto.senddata("Socket Number: ");
				mx[xlogon].cpu.i_a=nproto.senddata(socke);
				mx[xlogon].cpu.i_a=nproto.senddata("\nWarning: We don't verify the status of host.\nPacket's will be sended in siege mode.\n\n\nWarning 2: There is no functions actually that manage sendto and recvfrom\nclosesocket inmediatly.\n");
			}
			else
			{
				mx[xlogon].cpu.i_a=nproto.senddata("Can´t initialize socket.\n");
			}
		}
	}

}
void Cintep::prg_net_closesocket(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	mx[xlogon].cpu.i_a=nproto.senddata("Closesocket program\n\r");
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
		mx[xlogon].cpu.i_a=nproto.senddata("error closing... A successful WSAStartup call must occur before using this function.\n");
		okq=0;
	}
	if (rspw==WSAENETDOWN)
	{
		mx[xlogon].cpu.i_a=nproto.senddata("The network subsystem has failed.\n");
		okq=0;
	}
	if (rspw==WSAENOTSOCK)
	{
		mx[xlogon].cpu.i_a=nproto.senddata("The descriptor is not a socket.\n");
		okq=0;
	}
	if (rspw==WSAEINTR)
	{
		mx[xlogon].cpu.i_a=nproto.senddata("The (blocking) Windows Socket 1.1 call was canceled through WSACancelBlockingCall.\n");
		okq=0;
	}
	if (rspw==WSAEWOULDBLOCK)
	{
		mx[xlogon].cpu.i_a=nproto.senddata("The socket is marked as nonblocking and SO_LINGER is set to a nonzero time-out value.\n");
		okq=0;
	}
	if (rspw==WSAEINPROGRESS)
	{
		mx[xlogon].cpu.i_a=nproto.senddata("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.\n");
		okq=0;
	}
	if(okq)
	{
		if (rspw>=0)
			mx[xlogon].cpu.i_a=nproto.senddata("Socket Closed.\n");
		else
			mx[xlogon].cpu.i_a=nproto.senddata("Unknown error\n");
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
	mx[xlogon].cpu.i_a=nproto.senddata("Prompt changed...\n\r");
}


void Cintep::prg_kill(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	//Kill a process..
	//Command: KILL
	int pid=atoi(mx[xlogon].cmdline+5); //convert input to integer
	fns.KillProcess(pid); //Send termination signal
	mx[xlogon].cpu.i_a=nproto.senddata("Termination Signal Sended...\n"); //Send message...
}

void Cintep::prg_rmuser(con_v mx[SERVER_CONNECTIONS], int xlogon) //deletes user
{
	passed=TRUE;
	// rmuser name
    fns.denter(mx[xlogon].cmdline); //filter username
	data_g.PutData("PASSWORDS",mx[xlogon].cmdline+6,NULL); //deletes entry for user
	data_g.PutData("UGROUP",mx[xlogon].cmdline+6,NULL); //deletes group
	mx[xlogon].cpu.i_a=nproto.senddata("rmuser name - Remove user utility... excecuted\n\r"); //inform
}

void Cintep::prg_mkuser(con_v mx[SERVER_CONNECTIONS], int xlogon) //make user
{
	passed=TRUE;
    // mkuser will be executed now...
	// format: mkuser -l [level] -p [password] name
	// if not password, will request this data
	// if not level, admin mode is taked.
	char exlusive[COMMAND_LINE];
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

	if (bresph)
		mx[xlogon].cpu.i_a=nproto.senddata("mkuser -l [level] -p [password] name\n");
	else
	{
		if (!strcmp(mx[xlogon].cmdline+7,""))
		{
			mx[xlogon].cpu.i_a=nproto.senddata("Need user... more: mkuser -h\n");
			return;
		}
		else
			strncpy(exlusive,mx[xlogon].cmdline+6,COMMAND_LINE);
		//do...
		char u_level[80]="admin";
		char u_pass[COMMAND_LINE];
		char u_name[80];
        //Verify for existence of level...
		if (strcmp(respl,"-l")) // level defined...
			strcpy(u_level,respl);
		if (!strcmp(respp,"-p")) // password not defined...
		{
			mx[xlogon].cpu.i_a=nproto.senddata("Insert password: "); //prompt for password
			nproto.getdnpass(u_pass,COMMAND_LINE); //get password
			fns.denter(u_pass); //filter.
		}
		else
		{
			strcpy(u_pass,respp); //copy from command line
			fns.denter(u_pass); //filter
		}
		strncpy(u_name,mx[xlogon].cmdline+6,80);

		if(data_g.PutData("PASSWORDS",u_name,fns.md5sum(u_pass)))
			mx[xlogon].cpu.i_a=nproto.senddata("Password changed...\n");
		else
			mx[xlogon].cpu.i_a=nproto.senddata("Cant change account password...\n");
		if(data_g.PutData("UGROUP",u_name,u_level))
			mx[xlogon].cpu.i_a=nproto.senddata("level changed...\n");
		else
			mx[xlogon].cpu.i_a=nproto.senddata("Cant change account password...\n");
	}
}

void Cintep::prg_passwd(con_v mx[SERVER_CONNECTIONS], int xlogon) //change password
{
	passed=TRUE;
	//Change User's password
	if (!strcmp(mx[xlogon].cmdline+7,"")) // prompt for password if not specified
	{
		mx[xlogon].cpu.i_a=nproto.senddata("Insert password: ");
		nproto.getdnpass(mx[xlogon].cmdline+7,COMMAND_LINE-7);
	}
	fns.denter(mx[xlogon].cmdline); //filter's password
	/*do change.*/
	if(data_g.PutData("PASSWORDS",mx[xlogon].c_User,fns.md5sum(mx[xlogon].cmdline+7)))
		mx[xlogon].cpu.i_a=nproto.senddata("\n\rPassword changed...\n\rpasswd [password]\n\r");
	else
		mx[xlogon].cpu.i_a=nproto.senddata("Password not changed...");
}

void Cintep::prg_who(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	char exterm[COMMAND_LINE]; 
	mx[xlogon].cpu.i_a=nproto.senddata("  UAID SOCKid            Username:  Conected since:\n");
	for(int n=0;n<SERVER_CONNECTIONS;n++)
	{
		if (mx[n].busy==TRUE)
		{
			sprintf(exterm," %5u  %5u   %18s  %s\r",n,mx[n].socket,mx[n].c_User, ctime(&mx[n].since));
			mx[xlogon].cpu.i_a=nproto.senddata(exterm);
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
		mx[xlogon].cpu.i_a=nproto.senddata("Format: cat [-t] [-h] file\n\r-t Complete 10,13 bytes\n\r-h help\n\r");
		return;
	}
	fns.denter(mx[xlogon].cmdline);
	char buffer[256];
	memset(&buffer,0,256);
	int fh;
	unsigned int nbytes = 256, bytesread;
	if( (fh = _open(mx[xlogon].cmdline+4 , _O_RDONLY )) == -1 )
	{
		mx[xlogon].cpu.i_a=nproto.senddata("Cannot open local file on telnet server...");
	}
	else
	{
		while( ( bytesread = _read( fh, buffer, nbytes ) ) > 0 && fh!=-1 )
		{
			buffer[bytesread]=0;
			if (strcmp(respt,"-t"))
				fns.renter(buffer);
			mx[xlogon].cpu.i_a=nproto.senddata(buffer);
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
		mx[xlogon].cpu.i_a=nproto.senddata("Can't locate specified directory\n\r"); //error 

	_getcwd( buff, _MAX_PATH );   /* Get the current working directory: */
	strncpy(mx[xlogon].localdir,buff, _MAX_PATH); //copy it to "user specifications"...
}
void Cintep::prg_ps(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	mx[xlogon].cpu.i_a=nproto.senddata("PSLIST Internal Program 1.0\n\r");
	if (mx[xlogon].cmdline[3]=='-' && mx[xlogon].cmdline[4]=='h')
	{
		mx[xlogon].cpu.i_a=nproto.senddata("#PS is a internal program who show all aplications\n\rpid's and ppids (process id) & (parent Process ID)\n\rYou can also use: KILL (Terminate program by pid)\n\r");
	}
	else
	{
		mx[xlogon].cpu.i_a=nproto.senddata("\n\r   PPID  PID   APPNAME\n\r");
		long r;
		HANDLE hSnapShot;
		PROCESSENTRY32 uProcess;
		hSnapShot = CreateToolhelp32Snapshot (TH32CS_SNAPALL, NULL);
		uProcess.dwSize= sizeof(uProcess);
		r = Process32First (hSnapShot, &uProcess );
		while (r)
		{
			char exterm[COMMAND_LINE];
			if (!data_g.nt)
			{
	            char cmpare[COMMAND_LINE];
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
			mx[xlogon].cpu.i_a=nproto.senddata(exterm);
			r = Process32Next(hSnapShot, &uProcess );
		}
	}

}
void Cintep::prg_rmdir(con_v mx[SERVER_CONNECTIONS], int xlogon) //Remove an directory
{
	passed=TRUE;
	char resph[12]="";
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);
	if (strcmp(resph,"-h"))
		mx[xlogon].cpu.i_a=nproto.senddata("Format: rmdir [-h] directory (can be formatted such as: dir$snext\n\r");
	else
	{
		fns.denter(mx[xlogon].cmdline);
			
		if( RemoveDirectory( fns.convert(mx[xlogon].cmdline+6,mx,xlogon) ) )
			mx[xlogon].cpu.i_a=nproto.senddata("Dir. removed\n");
		else
			mx[xlogon].cpu.i_a=nproto.senddata( "Could not removes directory\n");
	}
}
void Cintep::prg_mkdir(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	char resph[12]="";
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);
	if (strcmp(resph,"-h"))
		mx[xlogon].cpu.i_a=nproto.senddata("Format: mkdir [-h] directory (can be formatted such as: dir$snext\n\r");
	else
	{
		fns.denter(mx[xlogon].cmdline);
			
		if( CreateDirectory( fns.convert(mx[xlogon].cmdline+6,mx,xlogon),NULL ) )
			mx[xlogon].cpu.i_a=nproto.senddata("Created.\n\r");
		else
			mx[xlogon].cpu.i_a=nproto.senddata( "Could not create directory\n");
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
		mx[xlogon].cpu.i_a=nproto.senddata("Format: rm [-y] [-h] file (can be formatted such as: archivo$s1.txt)\n\r-y don't query\n\r-h help\n\r");
	if (!strcmp(respy,"-y"))
	{
		char msw[20];
		mx[xlogon].cpu.i_a=nproto.senddata("Press N to cancel delete file operation: ");
		nproto.getdnline(msw,20);
		strupr(msw);
		if (msw[0]=='N')
			bOK=FALSE;
	}
	if (bOK==TRUE)
	{
		fns.denter(mx[xlogon].cmdline);
		
		if( DeleteFile( fns.convert(mx[xlogon].cmdline+3,mx, xlogon) ) == -1 )
			mx[xlogon].cpu.i_a=nproto.senddata( "Could not delete the file\n");
		else
			mx[xlogon].cpu.i_a=nproto.senddata("Deleted.\n\r");
	}
	else
		mx[xlogon].cpu.i_a=nproto.senddata("not deleted.\n\r");
}

void Cintep::prg_cp(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//Copy file
	passed=TRUE;

	char exlusive[COMMAND_LINE];
	char respd[COMMAND_LINE];
	char respo[12];
	char resph[12];

	BOOL bresph=0;
	BOOL brespo=0;

	strncpy(respd,fns.deparg(mx[xlogon].cmdline,"-d",FALSE),COMMAND_LINE);
	strncpy(respo,fns.deparg(mx[xlogon].cmdline,"-o",TRUE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;
	if (strcmp(respo,"-o"))
		brespo=1;
	fns.denter(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline+3,""))
	{
		mx[xlogon].cpu.i_a=nproto.senddata("USAGE: cp source [-o] -d destination\n-o Overwrite\n");
		return;
	}
	else
		strncpy(exlusive,mx[xlogon].cmdline+3,COMMAND_LINE);

	if (bresph || !strcmp(respd,"-d")) //incomplete args, or -h help request
		mx[xlogon].cpu.i_a=nproto.senddata("cp source [-o] -d destination\n\r-d Destination Filename\n\rsource: Source FileName\n\r[Data] Compatible with formmated data such as: Hola$sComo$sEstas$n\n\r");
	else
	{
		//Do operations...
		char csa[COMMAND_LINE];

		char src[COMMAND_LINE];
		char dst[COMMAND_LINE];

		strncpy(dst,fns.convert(respd,mx, xlogon),COMMAND_LINE); //convert destination
		strncpy(src,fns.convert(exlusive,mx, xlogon),COMMAND_LINE); //convert source
		
		sprintf(csa,"Copying %s to %s...",src,dst);
		mx[xlogon].cpu.i_a=nproto.senddata(csa);

		if (CopyFile(src,dst,!brespo))
			mx[xlogon].cpu.i_a=nproto.senddata("Completed.\n");
		else
			mx[xlogon].cpu.i_a=nproto.senddata("fail.\n");
	}
}

void Cintep::prg_mv(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//Copy file
	passed=TRUE;

	char exlusive[COMMAND_LINE];
	char respd[COMMAND_LINE];
	char respo[12];
	char resph[12];

	BOOL bresph=0;
	BOOL brespo=0;

	strncpy(respd,fns.deparg(mx[xlogon].cmdline,"-d",FALSE),COMMAND_LINE);
	strncpy(respo,fns.deparg(mx[xlogon].cmdline,"-o",TRUE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;
	if (strcmp(respo,"-o"))
		brespo=1;
	fns.denter(mx[xlogon].cmdline);

	if (!strcmp(mx[xlogon].cmdline+3,""))
	{
		mx[xlogon].cpu.i_a=nproto.senddata("USAGE: mv source [-o] -d destination\n-o Overwrite\n");
		return;
	}
	else
		strncpy(exlusive,mx[xlogon].cmdline+3,COMMAND_LINE);

	if (bresph || !strcmp(respd,"-d")) //incomplete args, or -h help request
		mx[xlogon].cpu.i_a=nproto.senddata("mv source [-o] -d destination\n\r-d Destination Filename\n\rsource: Source FileName\n\r[Data] Compatible with formmated data such as: Hola$sComo$sEstas$n\n\r");
	else
	{
		//Do operations...
		char csa[COMMAND_LINE];

		char src[COMMAND_LINE];
		char dst[COMMAND_LINE];

		strncpy(dst,fns.convert(respd,mx, xlogon),COMMAND_LINE); //convert destination
		strncpy(src,fns.convert(exlusive,mx, xlogon),COMMAND_LINE); //convert source
		
		sprintf(csa,"Moving %s to %s...",src,dst);
		mx[xlogon].cpu.i_a=nproto.senddata(csa);
		
		//CopyFile(src,dst,!brespo)
		if (brespo) 
		{
			if (MoveFileEx(src,dst,MOVEFILE_REPLACE_EXISTING)) mx[xlogon].cpu.i_a=nproto.senddata("Completed.\n");
			else mx[xlogon].cpu.i_a=nproto.senddata("fail.\n");
		}
		else
		{
			if (MoveFile(src,dst))
				mx[xlogon].cpu.i_a=nproto.senddata("Completed.\n");
			else mx[xlogon].cpu.i_a=nproto.senddata("fail.\n");
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
	strncpy(respe,fns.deparg(mx[xlogon].cmdline,"-e",FALSE),COMMAND_LINE); 
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
	{
		//showhelp
		char comande[COMMAND_LINE];
		sprintf(comande,"%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n%2d %s\n",
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
		mx[xlogon].cpu.i_a=nproto.senddata("Format: winexec [-e Exec_Number] path\n\rExcecute an program file\n\rExec Number:\n\r");
		mx[xlogon].cpu.i_a=nproto.senddata(comande);
	}
	else
	{
		char lcs[COMMAND_LINE];
		if (strcmp(respe,"-e"))
		{
			//respe founded... 
			execution = atoi(respe);
		}
		fns.denter(mx[xlogon].cmdline);
		sprintf(lcs,"Excecuting: %s \n",fns.convert(mx[xlogon].cmdline+8,mx, xlogon));
		mx[xlogon].cpu.i_a=nproto.senddata(lcs);
		int x=WinExec(fns.convert(mx[xlogon].cmdline+8,mx, xlogon),execution);
		if (x==0)
			mx[xlogon].cpu.i_a=nproto.senddata("The system is out of memory or resources.\n\r");
		else
		{
			if (x==ERROR_BAD_FORMAT)
				mx[xlogon].cpu.i_a=nproto.senddata("The .EXE file is invalid (non-Win32 .EXE or error in .EXE image).\n\r");
			else
			{
				if (x==ERROR_FILE_NOT_FOUND) 
					mx[xlogon].cpu.i_a=nproto.senddata("The specified file was not found.\n\r");
				else
				{
					if (x==ERROR_PATH_NOT_FOUND) 
						mx[xlogon].cpu.i_a=nproto.senddata("The specified path was not found.\n\r");
					else
						mx[xlogon].cpu.i_a=nproto.senddata("Execution Completed\n\r");
				}
			}
		}
	}
}

void Cintep::prg_fopen(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	// Format: fopen [-m mode] path
	// default mode is reading mode.
	char respm[COMMAND_LINE];
	char resph[12];
	int execution=SW_SHOWDEFAULT;
	strncpy(respm,fns.deparg(mx[xlogon].cmdline,"-m",FALSE),COMMAND_LINE); 
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);
	if (strcmp(resph,"-h"))
	{
		//showhelp
		mx[xlogon].cpu.i_a=nproto.senddata("Format: fopen [-m mode] path\n	Opens an file, default_ is reading\nModes:\nr  reading mode. If the file does not exist or cannot be found, the fopen call fails.\nw  writing mode. If the given file exists, its contents are destroyed.\na  appending without removing the EOF marker before writing _new data to the file; creates the file first _if it doesn't exist.\nr+ both reading and writing mode. The file must exist. \nw+ both reading and writing mode. If the given file exists, its contents are destroyed. \na+ reading and appending mode; the appending operation includes the removal of the EOF marker.\nAnother's modes can be used.\n");
	}
	else
	{
		char lcs[COMMAND_LINE];
		if (!strcmp(respm,"-m")) strcpy(respm,"r"); //using default: read file
		fns.denter(mx[xlogon].cmdline);
		sprintf(lcs,"Openning: %s \n",fns.convert(mx[xlogon].cmdline+6,mx, xlogon));
		mx[xlogon].cpu.i_a=nproto.senddata(lcs);
		FILE *stream;
		char pathoffile[_MAX_PATH];
		strncpy(pathoffile,fns.convert(mx[xlogon].cmdline+6,mx, xlogon),_MAX_PATH);
		if(!strcmp(pathoffile,""))
		{
			mx[xlogon].cpu.i_a=nproto.senddata("Enter a file-name\n");
			return; 
		}
		if (( stream = fopen( pathoffile, respm))==NULL)
			mx[xlogon].cpu.i_a=nproto.senddata("Cannot open file\n");
		else
		{
			mx[xlogon].cpu.i_a=nproto.senddata("The file has been openned, attaching to Users file's handler\n");
			//Search for file handler.
			int freehandler=-1;
			for (int y=0; y<FILE_SLOTS;y++) //search some free handler...
				if (mx[xlogon].files[y]==0)
					freehandler=y;
			if (freehandler==-1)
			{
				mx[xlogon].cpu.i_a=nproto.senddata("No handlers availables, closing file\n");
				fclose(stream);
            }
			else
			{
				//putting it to file handler and print.
				mx[xlogon].files[freehandler]=stream;
				char out_dt[COMMAND_LINE];
				sprintf(out_dt,"Handler #: %d\n",freehandler);
				mx[xlogon].cpu.i_a=nproto.senddata(out_dt);
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

	strncpy(respd,fns.deparg(mx[xlogon].cmdline,"-d",FALSE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;
	fns.denter(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline+6,""))
		mx[xlogon].cpu.i_a=nproto.senddata("fputc [-h] [-d] file handler\n-h help\n-d Data integer.\n");
	else
	{
		//Do operations...
		if (!strcmp(respd,"-f"))
		{
			mx[xlogon].cpu.i_a=nproto.senddata("not data specified.\n");
			return;
		}
		int hand=atoi(mx[xlogon].cmdline+6); //point to handler file...
		if (hand>FILE_SLOTS) mx[xlogon].cpu.i_a=nproto.senddata("Your value is out of range.\n");
		else
		{
			if(mx[xlogon].files[hand]==0) mx[xlogon].cpu.i_a=nproto.senddata("Not opened file at this handler\n");
			else
			{
				//ready for write.
				if(fputc(atoi(respd),mx[xlogon].files[hand])==EOF) mx[xlogon].cpu.i_a=nproto.senddata("Error.\n");
				else mx[xlogon].cpu.i_a=nproto.senddata("Writed.\n");
   			}
		}
	}
}

void Cintep::prg_fgetc(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//get char from file.
	passed=TRUE;

	char resph[12];
	BOOL bresph=0;

	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;
	fns.denter(mx[xlogon].cmdline);
	if (bresph)
		mx[xlogon].cpu.i_a=nproto.senddata("fgetc handler\n");
	else
	{
		//Do operations...
		int hand=atoi(mx[xlogon].cmdline+6); //point to handler file...
		if (hand>FILE_SLOTS) mx[xlogon].cpu.i_a=nproto.senddata("Your value is out of range.\n");
		else
		{
			if(mx[xlogon].files[hand]==0) mx[xlogon].cpu.i_a=nproto.senddata("Not opened file at this handler\n");
			else
			{
				int mfs=fgetc(mx[xlogon].files[hand]);
				if (mfs!=EOF)
				{
					//data getted. 
					char ary[80];
					sprintf(ary,"Normal: %01c  Hexadecimal: %02X  Decimal: %03d\n",mfs,mfs,mfs);
					mx[xlogon].cpu.i_a=nproto.senddata(ary); //sending...
				}
				else 
					mx[xlogon].cpu.i_a=nproto.senddata("#EOF#\n");
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
	strncpy(respr,fns.deparg(mx[xlogon].cmdline,"-r",TRUE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(respr,"-r"))
		brespr=1;
	if (strcmp(resph,"-h"))
		bresph=1;
	fns.denter(mx[xlogon].cmdline);

	if (bresph)
		mx[xlogon].cpu.i_a=nproto.senddata("fgets [-r] handler\n-r Show in hexadecimal\n");
	else
	{
		//Do operations...
		int hand=atoi(mx[xlogon].cmdline+6); //point to handler file...
		if (hand>FILE_SLOTS) mx[xlogon].cpu.i_a=nproto.senddata("Your value is out of range.\n");
		else
		{
			if(mx[xlogon].files[hand]==0) mx[xlogon].cpu.i_a=nproto.senddata("Not opened file at this handler\n");
			else
			{
				char mfs[COMMAND_LINE];
				if (fgets(mfs,COMMAND_LINE,mx[xlogon].files[hand])!=NULL)
				{
					//data getted. mx[xlogon].cpu.i_a=nproto.senddata(tempget); //sending...
					int r=(int)strlen(mfs);
					if (!brespr)
						mx[xlogon].cpu.i_a=nproto.senddata(mfs);
					else
					{
						//send byte to byte 23 hex's by line.
						mx[xlogon].cpu.i_a=nproto.senddata("---------HEXADECIMAL VIEW-------------------------------------------\n");
						int ha=0;
						for (int h=0; h<r; h++)
						{
							if (ha==23)
							{
								mx[xlogon].cpu.i_a=nproto.senddata("\n");
								ha=0;
							}
							else ha++;
							char snd[80];
							sprintf(snd,"%02X ",mfs[h]);
							mx[xlogon].cpu.i_a=nproto.senddata(snd);
						}
						mx[xlogon].cpu.i_a=nproto.senddata("\n---------NORMAL VIEW------------------------------------------------\n");
						mx[xlogon].cpu.i_a=nproto.senddata(mfs);
					}
				}
				else 
					mx[xlogon].cpu.i_a=nproto.senddata("#EOF#\n");
			}
		}
	}

}
void Cintep::prg_fputs(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	// fputs [-h] [-f] data  
	passed=TRUE;
	char respf[12];
	char resph[12];

	BOOL bresph=0;

	strncpy(respf,fns.deparg(mx[xlogon].cmdline,"-f",FALSE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns.denter(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline+6,""))
		mx[xlogon].cpu.i_a=nproto.senddata("fputs [-h] [-f] data\n-h help\n-f File handler... obligaroty\n");
	else
	{
		if (!strcmp(respf,"-f"))
		{
			mx[xlogon].cpu.i_a=nproto.senddata("not file specified.\n");
			return;
		}
		//Do operations...
		int hand=atoi(respf); //point to handler file...
		if (hand>FILE_SLOTS) mx[xlogon].cpu.i_a=nproto.senddata("Your value is out of range.\n");
		else
		{
			if(mx[xlogon].files[hand]==0) mx[xlogon].cpu.i_a=nproto.senddata("Not opened file at this handler\n");
			else
			{
				//ready for write.
				char datatoput[COMMAND_LINE];
				char dtaln[128];
				strncpy(datatoput,fns.convert(mx[xlogon].cmdline+6,mx,xlogon),COMMAND_LINE);
				sprintf(dtaln,"Writed: %u characters\n",(unsigned int)strlen(datatoput));
				if(fputs(datatoput,mx[xlogon].files[hand])<0) mx[xlogon].cpu.i_a=nproto.senddata("Error. cannot write\n");
				else mx[xlogon].cpu.i_a=nproto.senddata(dtaln);
   			}
		}
	}
}
void Cintep::prg_fclose(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	//close handler.
	passed=TRUE;
	int hand=atoi(mx[xlogon].cmdline+7);
	mx[xlogon].cpu.i_a=nproto.senddata("file closer: fclose [handler]\n");
	if (hand>FILE_SLOTS) mx[xlogon].cpu.i_a=nproto.senddata("Your value is out of range.\n");
	else
	{
		if(mx[xlogon].files[hand]==0) mx[xlogon].cpu.i_a=nproto.senddata("Not opened file at this handler\n");
		else
		{
			if (fclose(mx[xlogon].files[hand])==0) 
			{
				mx[xlogon].cpu.i_a=nproto.senddata("File closed, and handler released\n");
				mx[xlogon].files[hand]=0;
			}
			else
				mx[xlogon].cpu.i_a=nproto.senddata("Unknown error closing file. Not Closed.\n");
		}
	}
}


void Cintep::prg_shutdown(con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	passed=TRUE;
	// Format: shutdown [-e Number_of_Extra_Args] MODE_Number
	char respe[COMMAND_LINE];
	char resph[12];

	UINT nexecution=EWX_SHUTDOWN;
	DWORD eexecution=0;

	strncpy(respe,fns.deparg(mx[xlogon].cmdline,"-e",FALSE),COMMAND_LINE); 
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
	{
		//showhelp
		char cmde[COMMAND_LINE];
		char cmdf[COMMAND_LINE];
		sprintf(cmde,"%2d %s\n%2d %s\n%2d %s\n%2d %s\n",
		EWX_LOGOFF,"EWX_LOGOFF",
		EWX_POWEROFF,"EWX_POWEROFF",
		EWX_REBOOT,"EWX_REBOOT",
		EWX_SHUTDOWN,"EWX_SHUTDOWN");

		sprintf(cmdf,"%2d %s\n%2d %s\n",
		EWX_FORCE, "EWX_FORCE",
		EWX_FORCEIFHUNG, "EWX_FORCEIFHUNG");

		mx[xlogon].cpu.i_a=nproto.senddata("Format: shutdown [-e Number_of_Extra_Args] MODE_Number\nShutDown Windows\nMODE_NUMBER:\n");
		mx[xlogon].cpu.i_a=nproto.senddata(cmde);
		mx[xlogon].cpu.i_a=nproto.senddata("Number_of_Extra_Args:\n");
		mx[xlogon].cpu.i_a=nproto.senddata(cmdf);
	}
	else
	{
		if (strcmp(respe,"-e"))
			eexecution = atoi(respe); //respe founded... 
		if (strcmp(mx[xlogon].cmdline+9,""))
            nexecution = atoi(mx[xlogon].cmdline+9);

		fns.denter(mx[xlogon].cmdline);
		mx[xlogon].cpu.i_a=nproto.senddata("Sending ShutDown Signal...\n");
        
		if (ExitWindowsEx(nexecution | eexecution,NULL)) mx[xlogon].cpu.i_a=nproto.senddata("Succefully Sended\n");
		else mx[xlogon].cpu.i_a=nproto.senddata("Error sending shutdown signal\n");
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
		strncpy(exlusive,mx[xlogon].cmdline+3,COMMAND_LINE);
	}
	if (bresph==TRUE)
	{
		mx[xlogon].cpu.i_a=nproto.senddata("LS INTERNAL UTILITY\n\r-l list mode\n\r-h help\n\r-c Show all data\n\r-d Shows directory\n\r");
	}
	else
	{
		//Exlusive contain a comodin path
		WIN32_FIND_DATA ffd; // file information struct
		HANDLE sh = FindFirstFile(exlusive, &ffd);
		if(INVALID_HANDLE_VALUE == sh)
		{
			mx[xlogon].cpu.i_a=nproto.senddata("LS: not a proper path i guess\n");
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
					mx[xlogon].cpu.i_a=nproto.senddata(datafile);
					mx[xlogon].cpu.i_a=nproto.senddata("\n\r");
				}
				if (brespl)
				{
					
					char ed[80];
					strcpy(ed,ctime(&flwt));
					fns.denter(ed);
					sprintf(datafile,"%25s %7u %s",	ed,ffd.nFileSizeLow,ffd.cFileName);
					mx[xlogon].cpu.i_a=nproto.senddata(datafile);
					mx[xlogon].cpu.i_a=nproto.senddata("\n\r");

				}

			}
			else
			{
				if (ffd.dwFileAttributes == 0x10 || ffd.dwFileAttributes == 0x11 || ffd.dwFileAttributes == 0x12 || ffd.dwFileAttributes == 0x14 || ffd.dwFileAttributes == 0x18)
				{
					mx[xlogon].cpu.i_a=nproto.senddata(ffd.cFileName);
					mx[xlogon].cpu.i_a=nproto.senddata("\n\r");
				}
				else
				{
					if (!brespd)
					{
						mx[xlogon].cpu.i_a=nproto.senddata(ffd.cFileName);
						mx[xlogon].cpu.i_a=nproto.senddata("\n\r");
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
	if (!strcmp(mx[xlogon].cmdline+5,"")) mx[xlogon].cpu.i_a=nproto.senddata("Sleep 1.0 - usage: sleep [sleep time, milliseconds]\n");
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
	mx[xlogon].cpu.i_a=nproto.senddata(fns.convert(fns.md5sum(fns.convert(mx[xlogon].cmdline+4,mx, xlogon)),mx, xlogon));
	mx[xlogon].cpu.i_a=nproto.senddata("\n");
}
void Cintep::prg_df(con_v mx[SERVER_CONNECTIONS], int xlogon) //Disk free.
{
	passed=TRUE;
	char shown[256];
	char dir[_MAX_PATH];
	_getcwd(dir,_MAX_PATH);
	sprintf(shown,"Disk Free Space: %d KB\n",fns.GetFreeDiskSpaceInKB(dir));
	mx[xlogon].cpu.i_a=nproto.senddata(shown);
}

void Cintep::prg_echo(con_v mx[SERVER_CONNECTIONS], int xlogon)
{ // Internal Program Echo
	passed=TRUE;
	mx[xlogon].cpu.i_a=nproto.senddata(fns.convert(mx[xlogon].cmdline+5,mx, xlogon));
}

void Cintep::prg_msgbox(con_v mx[SERVER_CONNECTIONS], int xlogon)
{ // Internal Program AFXMESSAGEBOX - MSGBOX
	passed=TRUE;
	AfxMessageBox(fns.convert(mx[xlogon].cmdline+7,mx,xlogon));
}

void Cintep::start_intep(con_v mx[SERVER_CONNECTIONS], int local_user, char *newkey)
{ //Start Interpreter
	nproto.continue_socket(mx[local_user].socket,newkey);
}

int	Cintep::printprompt(con_v mx[SERVER_CONNECTIONS], int local_user)
{ //print prompt specified in data_g and converted with "convert"
	return nproto.senddata(fns.convert(data_g.server_prompt,mx,local_user));
}
void Cintep::prg_cmp(con_v mx[SERVER_CONNECTIONS], int xlogon) 
{	//program cmp: compare two strings
	// cmpenv -v string1 string2
	passed=TRUE;
	char respv[COMMAND_LINE];
	char resph[12];

	BOOL bresph=0;

	strncpy(respv,fns.deparg(mx[xlogon].cmdline,"-v",FALSE),COMMAND_LINE);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns.denter(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline+4,""))
		mx[xlogon].cpu.i_a=nproto.senddata("cmp [-h] [-v string1] string2\n-h help\n-v first string obligaroty\nAll strings must be $ formatted \n");
	else
	{
		if (!strcmp(respv,"-v"))
		{
			mx[xlogon].cpu.i_a=nproto.senddata("not string1 specified.\n");
			return;
		}
		//Do operations...
		fns.denter(mx[xlogon].cmdline+4);
		char tcmp1[COMMAND_LINE];
		char tcmp2[COMMAND_LINE];
		strncpy(tcmp1,fns.convert(mx[xlogon].cmdline+4,mx,xlogon),COMMAND_LINE);
		strncpy(tcmp2,fns.convert(respv,mx,xlogon),COMMAND_LINE);
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

	strncpy(respv,fns.deparg(mx[xlogon].cmdline,"-v",FALSE),12);
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);

	if (strcmp(resph,"-h"))
		bresph=1;

	fns.denter(mx[xlogon].cmdline);

	if (bresph || !strcmp(mx[xlogon].cmdline+7,""))
		mx[xlogon].cpu.i_a=nproto.senddata("setenv [-h] [-v] data\n-h help\n-v var to use... obligaroty\n");
	else
	{
		if (!strcmp(respv,"-v"))
		{
			mx[xlogon].cpu.i_a=nproto.senddata("not var specified.\n");
			return;
		}
		//Do operations...
		fns.denter(mx[xlogon].cmdline+7);
		int mrt=mx[xlogon].m_mem.putmem(respv,mx[xlogon].cmdline+7,(int)strlen(mx[xlogon].cmdline+7));
		if (mrt==-1) mx[xlogon].cpu.i_a=nproto.senddata("not enought memory\n");
		if (mrt==-2) mx[xlogon].cpu.i_a=nproto.senddata("not slots available\n");
	}
}
void Cintep::prg_promptenv(con_v mx[SERVER_CONNECTIONS], int xlogon) //program: Prompt and save to enviroment var.
{
	passed=TRUE;
	//prompt for data... and put to var.
	char nmd[COMMAND_LINE];
	nproto.getdnline(nmd,COMMAND_LINE);
	fns.denter(nmd);
    int mrt= mx[xlogon].m_mem.putmem(mx[xlogon].cmdline+10,nmd,(int)strlen(nmd));
	if (mrt==-1) mx[xlogon].cpu.i_a=nproto.senddata("not enought memory\n");
	if (mrt==-2) mx[xlogon].cpu.i_a=nproto.senddata("not slots available\n");
}
void Cintep::prg_run(con_v mx[SERVER_CONNECTIONS],ers_svr svrs[SERVER_SLOTS], int xlogon)
{
	passed=TRUE;
	//run an script...
	char exlusive[COMMAND_LINE];
	char resph[12]="";
	strncpy(resph,fns.deparg(mx[xlogon].cmdline,"-h",TRUE),12);
	if (strcmp(resph,"-h") || !strcmp(mx[xlogon].cmdline+4,""))
		mx[xlogon].cpu.i_a=nproto.senddata("Format: run [-h] scriptfile (can be formatted such as: dir$nhola$snext)\n");
	else
	{
		fns.denter(mx[xlogon].cmdline);
		strncpy(exlusive,fns.convert(mx[xlogon].cmdline+4,mx,xlogon),COMMAND_LINE);
		Csc scripting;
		if (!scripting.openscript(exlusive)) mx[xlogon].cpu.i_a=nproto.senddata("Cannot open script.\n");
		else
		{
			//first goto main.
			int tl_main=scripting.search_label("main");
			if (tl_main<0)
			{
				mx[xlogon].cpu.i_a=nproto.senddata("Scripting error: not main founded\n");
				return;
			}
			if (!scripting.setlinepos(tl_main))
			{
				mx[xlogon].cpu.i_a=nproto.senddata("Jump error: Cannot jump to main\n");
				return;
			}
			//script openned. interpreting in a loop 
			while (1)
			{
				char buffered[COMMAND_LINE];
				strncpy(buffered,scripting.get_next_line(),COMMAND_LINE); //copy line into command line...
				fns.filterstring(buffered);
				strncpy(buffered,fns.convert_vars(buffered,mx,xlogon),COMMAND_LINE);
				fns.denter(buffered);
				// process for verify script commands...
				BOOL islccmd=0;
				if (buffered[0]==':') islccmd=1; //is label, don't excecute.
				if (fns.cmpfirstword(buffered,"EXITSCRIPT"))
				{
					islccmd=1;
					scripting.close_script();
					strncpy(mx[xlogon].cmdline,"",COMMAND_LINE);
					return; //end scripting.
				}
				if (fns.cmpfirstword(buffered,"JNZ")) //Jump if not Zero
				{
					//verify if not zero
					if (mx[xlogon].cpu.b_a)
					{
						int toline=scripting.search_label(buffered+4);
						if (toline<0)
						{
							mx[xlogon].cpu.i_a=nproto.senddata("Jump error: cannot find line to jump\n");
							return;
						}
						if (!scripting.setlinepos(toline))
						{
							mx[xlogon].cpu.i_a=nproto.senddata("Jump error: error while setting line\n");
							return;
						}
					}
					islccmd=1;
				}
				if (fns.cmpfirstword(buffered,"JZ")) //Jump if Zero
				{
					if (!mx[xlogon].cpu.b_a)
					{
						int toline=scripting.search_label(buffered+3);
						if (toline<0)
						{
							mx[xlogon].cpu.i_a=nproto.senddata("Jump error: cannot find line to jump\n");
							return;
						}
						if (!scripting.setlinepos(toline))
						{
							mx[xlogon].cpu.i_a=nproto.senddata("Jump error: error while setting line\n");
							return;
						}
					}
					islccmd=1;
				}
				if (fns.cmpfirstword(buffered,"JMP")) //Jump
				{
					int toline=scripting.search_label(buffered+4);
					if (toline<0)
					{
						mx[xlogon].cpu.i_a=nproto.senddata("Jump error: cannot find line to jump\n");
						return;
					}
					if (!scripting.setlinepos(toline))
					{
						mx[xlogon].cpu.i_a=nproto.senddata("Jump error: error while setting line\n");
						return;
					}
					islccmd=1;
				}
				if (!islccmd)
				{
					strncpy(mx[xlogon].cmdline,buffered,COMMAND_LINE);
					run(mx,svrs,xlogon); //run command... 
				}
				if (mx[xlogon].cpu.i_a<0) return; //network error. prevent for infinite loops scripts.
			}
		}
	}
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

		if (fns.cmpfirstword(mx[local_user].cmdline,"PROMPTENV"))
			prg_promptenv(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"SETENV"))
			prg_setenv(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"CMP"))
			prg_cmp(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"RUN"))
			prg_run(mx,svrs,local_user);
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

		if (fns.cmpfirstword(mx[local_user].cmdline,"MKDIR"))
			prg_mkdir(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"RMDIR"))
			prg_rmdir(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"MV"))
			prg_mv(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"RM"))
			prg_rm(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"PS"))
			prg_ps(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"DF"))
			prg_df(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"CD"))
			prg_cd(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"CAT"))
			prg_cat(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"FOPEN"))
			prg_fopen(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"FGETS"))
			prg_fgets(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"FGETC"))
			prg_fgetc(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"FPUTS"))
			prg_fputs(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"FPUTC"))
			prg_fputc(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"FCLOSE"))
			prg_fclose(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"MSGBOX"))
			prg_msgbox(mx,local_user);

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

		//if (fns.cmpfirstword(mx[local_user].cmdline,"NET_SENDTO"))
		//	prg_net_sendto(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"NET_RECEIVE"))
			prg_net_receive(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"NET_OPENSOCKET"))
			prg_net_opensocket(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"NET_CLOSESOCKET"))
			prg_net_closesocket(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"NET_SET_TIMEOUT"))
			prg_net_set_timeout(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"RMUSER"))
			prg_rmuser(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"MKUSER"))
			prg_mkuser(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"KILL"))
			prg_kill(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"PROMPT"))
			prg_prompt(mx,local_user);

		if (fns.cmpfirstword(mx[local_user].cmdline,"SHUTDOWN"))
			prg_shutdown(mx,local_user);
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
	mx[xlogon].cpu.i_a=nproto.senddata("Welcome to unmanarc remote control server...\nlist of commands:\n\n");
	mx[xlogon].cpu.i_a=nproto.senddata("run             : run excecutes script on server.\n");
	mx[xlogon].cpu.i_a=nproto.senddata("cmp			    : compares 2 formatted strings (echo is suppresed to internal vars.).\n");
	mx[xlogon].cpu.i_a=nproto.senddata("echo            : Echo characters introduced by command line\n");
	mx[xlogon].cpu.i_a=nproto.senddata("promptenv		: prompt client for enviroment variable\n");
	mx[xlogon].cpu.i_a=nproto.senddata("setenv			: set enviroment variable\n");
	mx[xlogon].cpu.i_a=nproto.senddata("MD5             : MD5 string introduced in command line\n");
	mx[xlogon].cpu.i_a=nproto.senddata("Sleep           : Wait [n] milliseconds\n");
	mx[xlogon].cpu.i_a=nproto.senddata("ls              : list files\n");
	mx[xlogon].cpu.i_a=nproto.senddata("winexec         : run process in [n] mode\n");
	mx[xlogon].cpu.i_a=nproto.senddata("mkdir           : Make a directory\n");
	mx[xlogon].cpu.i_a=nproto.senddata("rmdir           : Removes a directory\n");
	mx[xlogon].cpu.i_a=nproto.senddata("cp              : Copy File in another location\n");
	mx[xlogon].cpu.i_a=nproto.senddata("mv              : Move File in another location\n");
	mx[xlogon].cpu.i_a=nproto.senddata("rm              : remove file\n");
	mx[xlogon].cpu.i_a=nproto.senddata("ps              : List Proccesses\n");
	mx[xlogon].cpu.i_a=nproto.senddata("cd              : change directory\n");
	mx[xlogon].cpu.i_a=nproto.senddata("cat             : show file\n");
	mx[xlogon].cpu.i_a=nproto.senddata("msgbox			: Shows an AfxMessageBox on server\n");
	mx[xlogon].cpu.i_a=nproto.senddata("df				: Show Space free at this directory\n");
	mx[xlogon].cpu.i_a=nproto.senddata("who             : show information about users connected to the system\n");
	mx[xlogon].cpu.i_a=nproto.senddata("passwd          : change's password of local user\n");
	mx[xlogon].cpu.i_a=nproto.senddata("\n");
	mx[xlogon].cpu.i_a=nproto.senddata("fopen			: Open's file\n");
	mx[xlogon].cpu.i_a=nproto.senddata("fgets			: Read a line from file handler\n");
	mx[xlogon].cpu.i_a=nproto.senddata("fputs			: Puts $ formated data at file\n");
	mx[xlogon].cpu.i_a=nproto.senddata("fgetc			: Read 1 byte from file and display\n");
	mx[xlogon].cpu.i_a=nproto.senddata("fputc			: Put 1 byte to file\n");
	mx[xlogon].cpu.i_a=nproto.senddata("fclose			: Closes a file.\n");
	mx[xlogon].cpu.i_a=nproto.senddata("\n");
	mx[xlogon].cpu.i_a=nproto.senddata("shutdown        : Shutdown PC(must be admin)\n");
	mx[xlogon].cpu.i_a=nproto.senddata("kill            : terminate PID(must be admin)\n");
	mx[xlogon].cpu.i_a=nproto.senddata("prompt          : change prompt(must be admin)\n");
	mx[xlogon].cpu.i_a=nproto.senddata("mkuser          : creates another user (must be admin)\n");
	mx[xlogon].cpu.i_a=nproto.senddata("rmuser          : remove user from database(must be admin)\n");
	mx[xlogon].cpu.i_a=nproto.senddata("\n");
	mx[xlogon].cpu.i_a=nproto.senddata("net_lookup      : resolves name into dotted ip addr.\n");
	mx[xlogon].cpu.i_a=nproto.senddata("net_opensocket  : Open TCP raw connection to IP(must be admin)\n");
	mx[xlogon].cpu.i_a=nproto.senddata("net_set_timeout : Send data to TCP socket(must be admin)\n");
	mx[xlogon].cpu.i_a=nproto.senddata("net_send        : Send data to TCP socket(must be admin)\n");
//	mx[xlogon].cpu.i_a=nproto.senddata("net_sendto      : Send data to UDP socket(must be admin)\n");
	mx[xlogon].cpu.i_a=nproto.senddata("net_receive     : Receive data from TCP socket(must be admin)\n");
	mx[xlogon].cpu.i_a=nproto.senddata("net_closesocket : Close opened socket(must be admin)\n");
	mx[xlogon].cpu.i_a=nproto.senddata("\n");
	mx[xlogon].cpu.i_a=nproto.senddata("prx_connect     : tunnel to specified server\n");
	mx[xlogon].cpu.i_a=nproto.senddata("prx_who         : List tunnels\n");
	mx[xlogon].cpu.i_a=nproto.senddata("prx_close       : Restart connection to tunnel\n");
	mx[xlogon].cpu.i_a=nproto.senddata("\n");
	mx[xlogon].cpu.i_a=nproto.senddata("downloadfrom    : download file from CTOOL fileserver\n");
	mx[xlogon].cpu.i_a=nproto.senddata("uploadto        : upload file to CTOOL fileserver\n");
	mx[xlogon].cpu.i_a=nproto.senddata("\n");
	mx[xlogon].cpu.i_a=nproto.senddata("help            : show this help\n");
	mx[xlogon].cpu.i_a=nproto.senddata("\nNOTE: some program's may not work if you don't have privilege.\n");
}
