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
#include "protocol.h"


#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


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

Cproto::Cproto(Cini *inix)
{ //Constructor
	pushmode=0;
	prx=0;
	auth=0;
	ini=inix;
}
Cproto::~Cproto()
{ //destructor
	
}
void chrcpy(BYTE *a, char *b, int c)
{
	for (int p=0;p<c;p++)
        a[p]=b[p];
	//end with \0
	a[c]=0;
}

int Cproto::l_flush()
{
	if (pushmode)
	{
		pushmode=0;
	}
	//remove temporary file
	_rmtmp();
	pushmode=0;
	return 1;
}
int Cproto::setpushmode(BOOL pshmode)
{
	pushmode=pshmode;
	if (pushmode)
	{
		//creates temporary file
		if( (tempfile = tmpfile()) == NULL )
		{
			pushmode=0; //cannot open... not in pushmode
			return -1; //cannot open file.
		}
		w_pos=0;
		r_pos=0;
	}
	return 1;
}
char *Cproto::l_pop(int max)
{
	fsetpos(tempfile,&r_pos);
	char *data;
	data=(char *)malloc(max);
	if (fgets(data,max,tempfile)==NULL)
		return "";
	fgetpos(tempfile,&r_pos);
	return data;
}
int Cproto::l_push(char *data)
{
	fsetpos(tempfile,&w_pos);
	fputs(data,tempfile);
	fgetpos(tempfile,&w_pos);
	return 0;
}
int Cproto::setdefaultcolor()
{
	return setcolor(7);
}
int Cproto::setAction(char *action)
{
	char act[80];
	memset(&act,0,80);
	memset(&act,' ',69);
	size_t ask=strlen(action);
	if (ask>=69) ask=68;
	strncpy(act,action,ask);
	return senddata(act);
}
int Cproto::setActionStatus(bool success)
{
	if (success)
	{
		senddata("   [");
		setcolor(GREEN);
		senddata("DONE");
		setdefaultcolor();
		sendline("]");
	}
	else
	{
		senddata("   [");
		setcolor(RED);
		senddata("FAIL");
		setdefaultcolor();
		sendline("]");
	}
	return 1;
}

int Cproto::senddatacenter(char *data, short line) 
{
	size_t lugar=strlen(data);
	if (lugar>80) setposxy(1,line);
	else
	{
		lugar=lugar/2;
		lugar=40-lugar;
		setposxy((int)lugar,line);
	}
    return senddata(data);
}

int Cproto::drawRectangle(unsigned short x,			// X inicial
						  unsigned short y,			// Y final
						  unsigned short w,			// Ancho
						  unsigned short h,			// Alto
						  unsigned short line_type,		// 0: no line, 1: single line 2: double line
						  unsigned short line_color,	// Line color
						  unsigned short filled_color	// 0-15 colors (>=16 is trasparent)
						  )
{
	//Se verifican los datos de entrada
	if (x+w>80) return -9; //error, tamaño de pantalla superado
	if (y+h>25) return -9; //error, tamaño de pantalla superado
	w--;

	// Inicialmente se dibuja el rectangulo de color interno
	if (filled_color<16)
	{
		setcolor(filled_color);
		//Dibujar linea a linea
		char fillline[81];
		memset(&fillline,0,81);
		memset(&fillline,0xDB,w-1);
		for (unsigned short lh=1;lh<(h-1);lh++)
		{
            setposxy(x+1,y+lh);
			senddata(fillline);
		}
		setdefaultcolor();
	}
	//Se dibuja la linea
	if (line_type!=0 && line_type<3)
	{
			char topline[81];
			char endline[81];
			memset(&topline,0,81);
			memset(&endline,0,81);
			if (line_type==1)
			{
				memset(&topline,0xC4,w);
				memset(&endline,0xC4,w);
				topline[0]=(char)218;
				topline[w]=(char)191;
				endline[0]=(char)192;
				endline[w]=(char)217;
			}
			else if (line_type==2)
			{
				memset(&topline,205,w);
				memset(&endline,205,w);
				topline[0]=(char)201;
				topline[w]=(char)187;
				endline[0]=(char)200;
				endline[w]=(char)188;
			}
			//Enviar linea inicial
            setcolor(line_color);
			setposxy(x,y);
			sendline(topline);

			for (int lxh=1;lxh<(h-1);lxh++)
			{
				char ib[2]={(char)179,(char)0};
				if (line_type==2) ib[0]=(char)186;
				setposxy(x,y+lxh);
				senddata(ib);
				setposxy(x+w,y+lxh);
				senddata(ib);
			}

			//Enviar linea final
            setposxy(x,y+h-1);
			sendline(endline);
			setdefaultcolor();
	}
	return 1;
}


int Cproto::senddatahex(char *data, int lenght) //send as maximum 4094 bytes of data.
{
	__int64 offset=0;
	int dtr;
	char variab[80]={(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)197,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)197,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)10,(char)13,(char)0 };
	char variac[80]={(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)193,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)193,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)196,(char)10,(char)13,(char)0 };

	dtr=senddata(" Offset  \xB3  0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F \xB3            data\n");
	//senddata("---------+-------------------------------------------------+----------------\n");
	dtr=senddata(variab);
	while(offset<lenght)
	{
        BYTE prcl[17];
		char datah[85];
		memset(&datah,0,83);
		memset(&prcl,0,17);
		if (offset+16>lenght)	chrcpy(prcl,data+offset,lenght-(int)offset);
		else chrcpy(prcl,data+offset,16);
		char datab[20];
		_snprintf(datah,sizeof(datah)," %08X\xB3",offset);
		for (int i=0;i<16;i++)
		{
			memset(&datab,0,sizeof(datab));
			_snprintf(datab,sizeof(datab)," %02X",prcl[i]);
			strncat(datah,datab,sizeof(datah)-strlen(datah));
			if(i==7) strncat(datah," ",sizeof(datah)-strlen(datah));
		}
		strncat(datah," \xB3",sizeof(datah)-strlen(datah));
		//convert to "safe bytes" and catem
		size_t h=strlen(datah);
		for (int i=0;i<16;i++)
		{
			if (offset+i<=lenght)
			{
				if(prcl[i]==0 || prcl[i]==7 ||prcl[i]==8 ||prcl[i]==9 ||prcl[i]==10 || prcl[i]==13 ||prcl[i]==26 ) 	prcl[i]='.';
				datah[h+i]=prcl[i];
			}
		}
		prcl[16]=0;
		strncat(datah,"\n",sizeof(datah)-strlen(datah));
        offset=offset+16;
		dtr=senddata(datah);
	}
	dtr=senddata(variac);
	return dtr;
}
int Cproto::sendline(char *data) //send as maximum 4094 bytes of data.
{
	char dta[4094];
	strncpy(dta,data,4093);
	strncat(dta,"\n",sizeof(dta)-strlen(dta));
	return senddata(dta);
}
int Cproto::senddata(char *data) //send as maximum 4094 bytes of data.
{
	int dlenght=(int)strlen(data); //
	int dsnd=0;
	char bftsnd[4096]; //buffer to send
	if ( dlenght>4092 ) return -1;		//data is too large...
	if ( dlenght<=0 ) return -2;		//data is too short or be corrupted...
	memcpy(bftsnd,&dlenght,4);
	strncpy(bftsnd+4,data,sizeof(bftsnd)-4);
	if (!pushmode)
	{
		for (int g=4;g<dlenght+4;g++) bftsnd[g]=255-bftsnd[g];
		dsnd=send(x,bftsnd,dlenght+4,0);
	}
	else
	{ //pushmode
		dsnd=(int)strlen(data);
		l_push(data);
	}
	if (dsnd<=0) return -1;
    return dsnd;
}
int Cproto::sendfile(char *remotefilename, char *localfilename)
{
	int dlenght=5001; //5001 server----->client ftransfer...

	char bftsnd[4]; //buffer to send
	memcpy(bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for request put file
	if (y<=0) return -1; //error sending data

	char intep[1];
	char rmf[512];
	strncpy(rmf,remotefilename,512);
	if (send(x,rmf,512,0)<=0) return -3;
	if (recv2(x,intep,1,0)<=0) return -3;
	if (intep[0]==1) 
	{
		//Commencing opening file...
		ftrs ft;
		int fh;
		unsigned int nbytes = sizeof(ft.data), bytesread;
		char lcfile[512];
		strncpy(lcfile,localfilename,512);
		if( (fh = _open(lcfile , _O_RDONLY | _O_BINARY )) != -1 ) 
		{
			while( ( bytesread = _read( fh, ft.data, nbytes ) ) > 0 )
			{
				ft.bt=(short)bytesread;
				if (ft.bt>0) 
				{
					if (send(x,(char *)&ft,sizeof(ftrs),0)<=0) return -3;
				}
			}
			_close( fh );
			ft.bt=0;
			if (send(x,(char *)&ft,sizeof(ftrs),0)<=0) return -3;
			return 1;
		}
		ft.bt=0;
		if (send(x,(char *)&ft,sizeof(ftrs),0)<=0) return -3;
	}
	return -1;
}
int Cproto::recvfile(char *remotefilename, char *localfilename)
{
	int dlenght=5002; //5002 client---->server ftransfer...

	char bftsnd[4]; //buffer to send
	memcpy(bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for request put file
	if (y<=0) return -1; //error sending data

	ftrs ft;
	char rmf[512];
	char intep[1];
	strncpy(rmf,remotefilename,512);
	if (send(x,rmf,512,0)<=0) return -3;

	//Commencing writing file...
	int fh;
	char lcfile[512];
	DWORD tot=0;
	strncpy(lcfile,localfilename,512);
	if( (fh = _open(lcfile, _O_WRONLY | _O_CREAT | _O_BINARY )) != -1 )
	{
		intep[0]=1;
		if (send(x,intep,1,0)<=0) return -3;
		do
		{
			if (recv2(x,(char *)&ft,sizeof(ft),0)<=0) return -3;
			tot=tot+ft.bt;
			_write(fh,&ft.data,ft.bt);
		}
		while (ft.bt > 0);
		int jh=_close( fh );
		if (tot==0) 
		{
			DeleteFile(lcfile);
			return -4;
		}

		return 1;
	}
	else 
	{
		intep[0]=0;
		if (send(x,intep,1,0)<=0) return -3;
		return -2; //cannot open localfile
	}
}
void Cproto::deny_connection(SOCKET s)
{
	char buff[4096];
	memset(&buff,0,4096);
	recv2(s,buff,4096,0); // receive initial command
	send(s,"--------------------------------",32,0); //send enc. error. cannot continue.
}

int Cproto::receive(char *data, size_t maxdatasize)
{
	int header,b;
	b=GetBlock(&header, 4); //get header
	if (b<=0 || (int)maxdatasize<header) return -1;
	b=GetBlock(data, header);
	for (int g=0;g<header;g++) data[g]=255-data[g];
	if (b<0) return -1;
	else
	{
		data[b]=0;
	}
	return header;
}
int Cproto::GetBlock(void *p, int len)
{
	int tmplen=0;
	if ( (tmplen=recv2(x,(char *)p,len,0)) <= 0 ) return -1;
	else return tmplen;
}
int Cproto::getproxyline()
{
	char med[80];
	int p=recv2(x,med,80,0);
	if (p<=0) return -1;
	if (!strcmp(med,"CONNECT")) return 1;
	else if (!strcmp(med,"SERVAUTH")) return 2;
	else if (!strcmp(med,"TCPBOUNCE")) return 3;
	else if (!strcmp(med,"PING")) { strcpy(med,"PONG"); send(x,med,80,0); }
	return -2;
}

int Cproto::getdline(char *line)
{
	strcpy(line,"");
	return 0;
}
int Cproto::setposxy(int posx, int posy)
{
	if (posx>80 || posx<0 || posy>25 || posy<0 ) return -2; //out of range.
	int dlenght=posy-1; 
	dlenght=dlenght*80;
	dlenght=dlenght+6000+posx-1; //6000-8000
	char bftsnd[4]; //buffer to send
	memcpy(bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for cls
	if (y<=0) return -1; //error sending data
	return 0;
}

int Cproto::setcolor(unsigned short color)
{
	int dlenght=5003+color; //5003-5019
	if (dlenght>5019) return -2; //color not supported. 
	char bftsnd[4]; //buffer to send
	memcpy(bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for cls
	if (y<=0) return -1; //error sending data
	return 0;
}
int Cproto::setbackground(unsigned short color)
{
	int dlenght=5020+color; //5020-5036
	if (dlenght>5036) return -2; //color not supported. 
	char bftsnd[4]; //buffer to send
	memcpy(bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for cls
	if (y<=0) return -1; //error sending data
	return 0;
}
int Cproto::cls()
{
	int dlenght=5000; //5000 is a command to cls
	char bftsnd[4]; //buffer to send
	memcpy(bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for cls
	if (y<=0) return -1; //error sending data
	return 0;
}

int Cproto::settitle(char *title)
{
	int dlenght=5040; //5040 is a command to set title
	char bftsnd[4]; //buffer to send
	memcpy(bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for cls
	if (y<=0) return -1; //error sending data
	//Send data of title
	return senddata(title);
}
int Cproto::clreol(void)//Borrar hasta el final de línea
{
	int dlenght=5037; //5000 is a command to cls
	char bftsnd[4]; //buffer to send
	memcpy(bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for cls
	if (y<=0) return -1; //error sending data
	return 0;
}
int Cproto::delline(void)//Eliminar la línea actual
{
	int dlenght=5038; //5000 is a command to cls
	char bftsnd[4]; //buffer to send
	memcpy(bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for cls
	if (y<=0) return -1; //error sending data
	return 0;
}
int Cproto::insline(void)//Insertar una nueva línea
{
	int dlenght=5039; //5000 is a command to cls
	char bftsnd[4]; //buffer to send
	memcpy(bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for cls
	if (y<=0) return -1; //error sending data
	return 0;
}
int Cproto::getdnpass(char *line,int maxi)
{
	//get pass
	char buff[4096],bftsnd[4];
	int t=0,y,dlenght=4098; //4098 is a command to get pass

	memset(buff,0,4096); //initialize buff
	memcpy(bftsnd,&dlenght,4);

	if ((y=send(x,bftsnd,4,0))<=0) return -1; //Send command for request line
	t=receive(buff, sizeof(buff));

	if(t<=0) return -1;
	else
	{
		strncpy(line,buff,maxi);
		if (t<maxi)	return t;	 //Copio menos que maxi caracteres
		else		return maxi; //Copio maxi caracteres
	}
	return 0;
}


int Cproto::comparehash(char *hash, char *gout)
{
	//get pass
	char buff[4096],bftsnd[4],germen[30];
	int t=0,dlenght=4100,y; //4100 is a command to get pass

	memset(buff,0,4096); //initialize buff
	memcpy(bftsnd,&dlenght,4);

	if ( (y=send(x,bftsnd,4,0)) != 4 ) return -1; 

	memset(&germen,0,30);
	for( int i=0; i<29; i++)
	{
	   //srand( (unsigned)time( NULL ) );
	   germen[i]=60+rand()%64;
	}
	senddata(germen);
	t=receive(buff,sizeof(buff));
	if(t<0) return -1;
	else
	{
		strcpy(hash,buff);
		strcpy(gout,germen);
		return 1;
	}
	return 0;
}
int Cproto::getdnline(char *line,int maxi)
{
	//get line
	char buff[4096];
	memset(buff,0,4096); //initialize buff
	int t=0;
	
	int dlenght=4097; //4097 is a command to get line
	char bftsnd[4]; //buffer to send
	memcpy(bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for request line
	if (y<=0) return -1; //error sending data
	t=receive(buff,sizeof(buff));
	if(t<=0) return -1;
	else
	{
		strncpy(line,buff,maxi);
		if (t<maxi)
			return t;
		else
			return maxi;
	}
	return 0;
}

int Cproto::sendclose()
{
	//get line
	char buff[4096];
	memset(buff,0,4096); //initialize buff
	int t=0;
	
	int dlenght=4099; //4099 is a command to get encripted pass
	char bftsnd[4]; //buffer to send
	memcpy(bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for request line
	if (y<=0) return -1; //error sending data
	return y;
}

char *Cproto::getkey()
{ //Start negotiation with client
	return key;
}
void Cproto::continue_socket(SOCKET s, char *newkey)
{ //Start negotiation with client
    x=s;
	strncpy(key,newkey,32);
}

void Cproto::start_remote_auth_server()
{
//	Cini inicio;
//	ini->LoadData();
	char user_name[256],user_pass[256],user_germen[256];
	int nRsp;
	recv2(x,user_name,256,0);
	recv2(x,user_pass,256,0);
	recv2(x,user_germen,256,0);
	nRsp=ini->ValidateUser2(user_name,user_pass,user_germen);
	char sendrsp[4];
	memcpy(sendrsp,&nRsp,4);
	send(x,sendrsp,4,0);
	WSASendDisconnect(x,NULL);
}
void Cproto::start_remote_auth_server_false()
{
//	Cini inicio;
//	ini->LoadData();
	char user_name[256],user_pass[256],user_germen[256];
	int nRsp;
	recv2(x,user_name,256,0);
	recv2(x,user_pass,256,0);
	recv2(x,user_germen,256,0);
	nRsp=-1; //ini->ValidateUser2(user_name,user_pass,user_germen);
	char sendrsp[4];
	memcpy(sendrsp,&nRsp,4);
	send(x,sendrsp,4,0);
	WSASendDisconnect(x,NULL);
}

int Cproto::start_remote_auth_client(char *username, char *pass, char *germen)
{
//	Cini inicio;
//	ini->LoadData();
	char user_name[256],user_pass[256],user_germen[256];
	int nRsp=-1;
	strncpy(user_name,username,256);
	strncpy(user_pass,pass,256);
	strncpy(user_germen,germen,256);

	char rserver[256]="";
	unsigned short rport=3360;
	ini->getUpuserserver(rserver,256,rport);
	struct hostent *hentry;
	struct sockaddr_in Server_add;

	SOCKET Sock = socket( AF_INET, SOCK_STREAM, 0 ); //open socket
	// Connect Server
	if ((hentry = gethostbyname(rserver)) == NULL)
	{
		//error: go out. Cannot resolve.
		return -1;
	}
	else
	{
		// Make Socket
		memset((char *)&Server_add, 0, sizeof(Server_add));
		memcpy(&Server_add.sin_addr.s_addr, *(hentry->h_addr_list),sizeof(Server_add.sin_addr.s_addr));
		//host are copied to Server_add
		Server_add.sin_port = htons(rport);
		Server_add.sin_family = AF_INET;
		// Connect Server
		if( connect( Sock, (struct sockaddr *) &Server_add, sizeof( Server_add ) ) == 0 )
		{
			char initdata[5]="ipxf";
			send(Sock,initdata,4,0);
			send(Sock,user_name,256,0);
			send(Sock,user_pass,256,0);
			send(Sock,user_germen,256,0);
			char recvrsp[4];
			recv2(Sock,recvrsp,4,0);
			memcpy(&nRsp,recvrsp,4);
			closesocket(Sock);
		}
		else
			return -1;
	}
	return nRsp;
}



bool Cproto::start_socket(SOCKET s, BOOL encrypt)
{ //Start negotiation with client
	int t=0;
	x=s; //assing received socket to object global
	char buff[4096];
	memset(&buff,0,4096);
	t=recv2(s,buff,3,0); // receive initial command
	//INITIAL COMMAND_
	if (t==3)
	{
        //Command received... decoding initial
		if (buff[0]=='i' && buff[1]=='p') // Initialization command for proxy
		{
			/*
				URCS 2.1 Proxy protocol:

					- 1 Auth byte:
						101: be proxy in:
							- 1 byte with:
								0000:000X : Allow remote commands
								0000:00X0 : Allow Up-users
								0000:0X00 : Allow Upbounce
								0000:X000 : Log data
							- 256 bytes of server name
						102: be remote auth in.
			*/
			char authbyte[1];
			if (recv2(s,authbyte,1,0)!=1) return 0;
			if (authbyte[0]==(char)101)
			{
                //Obtener la estructura:
				char flags[1];
				if (recv2(s,flags,1,0)!=1) return 0;
				if ((flags[0]&0x01)==0x01) prx_uplink=true; else prx_uplink=false;
				if ((flags[0]&0x02)==0x02) prx_upuser=true; else prx_upuser=false;
				if ((flags[0]&0x04)==0x04) prx_upbounce=true; else prx_upbounce=false;
				if ((flags[0]&0x08)==0x08) prx_logdata=true; else prx_logdata=false;
				if (recv2(s,nameserver,256,0)!=256) return 0; 
				prx=1;
				return 1;              			
			}
			else if (authbyte[0]==(char)102)
			{
				//Irse como si fuese un remote-auth
				auth=1;
				return 1;
			}
			return 0; // Conexion no reconocida.
		}
		if (buff[0]!='i' || buff[1]!='u') // Initialization command
		{
			send(s,"Command error",13,0);
			return FALSE;
		}
	}
	else
	{
		send(s,"This client is not supported",28,0);
		return FALSE; // Cannot start socket... command error
	}
	//Primary negotiation completed.
    //the socket now has been initialized.
	return TRUE;
}
bool Cproto::isProxyIn()
{
	return prx;
}
bool Cproto::isAuthIn()
{
	return auth;
}
void Cproto::getServerName(char *name, size_t max_lenght)
{
	strncpy(name,nameserver,max_lenght);
}
bool Cproto::get_prx_uplink()
{
	return prx_uplink;
}
bool Cproto::get_prx_upuser()
{
	return prx_upuser;
}
bool Cproto::get_prx_upbounce()
{
	return prx_upbounce;
}
bool Cproto::get_prx_logdata()
{
	return prx_logdata;
}
