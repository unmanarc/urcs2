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

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


Cproto::Cproto()
{ //Constructor
	prx=0;
}
Cproto::~Cproto()
{ //destructor
	
}
void chrcpy(char *a, char *b, int c)
{
	for (int p=0;p<c;p++)
        a[p]=b[p];
}

int Cproto::senddata(char *data) //send as maximum 4094 bytes of data.
{
	int dlenght=(int)strlen(data); //
	int dsnd=0;
	char bftsnd[4096]; //buffer to send
	if ( dlenght>4092 ) return -1;		//data is too large...
	if ( dlenght<=0 ) return -2;		//data is too short or be corrupted...
	memcpy(&bftsnd,&dlenght,4);
	strcpy(bftsnd+4,data);
	dsnd=send(x,bftsnd,dlenght+4,0);
	if (dsnd<=0) return -1;
    return dsnd;
}
void Cproto::deny_connection(SOCKET s)
{
	char buff[4096];
	memset(&buff,0,4096);
	recv(s,buff,4096,0); // receive initial command
	send(s,"--------------------------------",32,0); //send enc. error. cannot continue.
}

int Cproto::receive(char *data)
{
	int header; //header
	int b;
	GetBlock(&header, 4); //get header
	b=GetBlock(data, header);
	if (b<0) return -1;
	else
	{
		data[b]=0;
	}
	return header;
}
int Cproto::GetBlock(void *p, int len)
{
	char tmpdata[4096];
	int tmpLen=-100;
	tmpLen=recv(x, tmpdata, len, 0); //maximum 4096 bytes
	if (tmpLen<=0)
		return -1;
	memcpy(p,&tmpdata,tmpLen);
	return tmpLen;
}
int Cproto::getproxyline()
{
	char med[80];
	int p=recv(x,med,80,0);
	if (!strcmp(med,"CONNECT")) return 1;
	else return -1;
}

int Cproto::getdline(char *line)
{
	strcpy(line,"");
	return 0;
}
int Cproto::getdnpass(char *line,int max)
{
	//get pass
	char buff[4096];
	memset(buff,0,4096); //initialize buff
	int t=0;
	
	int dlenght=4098; //4098 is a command to get pass
	char bftsnd[4]; //buffer to send
	memcpy(&bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for request line
	if (y<=0) return -1; //error sending data

	t=receive(buff);

	if(t<0) return -1;
	else
	{
		strncpy(line,buff,max);
		if (t<max)
			return t;
		else
			return max;
	}
	return 0;
}
int Cproto::getdnline(char *line,int max)
{
	//get line
	char buff[4096];
	memset(buff,0,4096); //initialize buff
	int t=0;
	
	int dlenght=4097; //4097 is a command to get line
	char bftsnd[4]; //buffer to send
	memcpy(&bftsnd,&dlenght,4);
	int y=send(x,bftsnd,4,0);//Send command for request line
	if (y<=0) return -1; //error sending data
	t=receive(buff);
	if(t<=0) return -1;
	else
	{
		strncpy(line,buff,max);
		if (t<max)
			return t;
		else
			return max;
	}
	return 0;
}

int Cproto::sendclose()
{
	//get line
	char buff[4096];
	memset(buff,0,4096); //initialize buff
	int t=0;
	
	int dlenght=4099; //4097 is a command to get line
	char bftsnd[4]; //buffer to send
	memcpy(&bftsnd,&dlenght,4);
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

BOOL Cproto::start_socket(SOCKET s, BOOL encrypt)
{ //Start negotiation with client
	int t=0;
	x=s; //assing received socket to object global
	char buff[4096];
	memset(&buff,0,4096);
	t=recv(s,buff,3,0); // receive initial command
	//INITIAL COMMAND_
	if (t==3)
	{
        //Command received... decoding initial
		if (buff[0]=='i' && buff[1]=='p') // Initialization command for proxy
		{
			//proxy negotiation... get name, and go out.
			if (recv(s,nameserver,512,0)>0) 
			{
				prx=1;
				return 1;
			}
			else return 0; //if connection fail, go out.
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
	// Negot. encription options.

	// Sending enc. options...
	// as default is not encripted (future, feature)

    t=send(s,"00000000000000000000000000000000",32,0); //this is the code 
	
	if (t<0)
		return 0; //error sending key.
	
    //the socket now has been initialized.
	return TRUE;
}