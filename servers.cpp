#include "StdAfx.h"
#include ".\servers.h"

#define RETURNED_LENGHT 512
static char returned[RETURNED_LENGHT];

Cservers::Cservers(void)
{
	for (int fnd=0; fnd<SERVER_SLOTS;fnd++) //free server slots
	{
		svrs[fnd].busy=FALSE;
		svrs[fnd].cbsy=FALSE;
		svrs[fnd].blocked=FALSE;
		svrs[fnd].f=0;
		strcpy(svrs[fnd].ip,"0.0.0.0");
		strcpy(svrs[fnd].nameserver,"none");
		svrs[fnd].transmited_bytes=0;
		svrs[fnd].received_bytes=0;
	}
}
Cservers::~Cservers(void)
{
}
bool Cservers::isBusy(int server)
{
	if (server>=SERVER_SLOTS) return false;
	return svrs[server].busy;
}
void Cservers::setBusy(int server,bool bsy)
{
	if (server>=SERVER_SLOTS) return;
	time(&(svrs[server].connected_when));
	svrs[server].busy=bsy;
}
bool Cservers::isClientBusy(int server)
{
	if (server>=SERVER_SLOTS) return false;
	return svrs[server].cbsy;
}
void Cservers::setClientBusy(int server,bool bsy)
{
	if (server>=SERVER_SLOTS) return;
	svrs[server].cbsy=bsy;
}

bool Cservers::isBlocked(int server)
{
	if (server>=SERVER_SLOTS) return false;
	return svrs[server].blocked;
}
void Cservers::setBlocked(int server,bool bsy)
{
	if (server>=SERVER_SLOTS) return;
	svrs[server].blocked=bsy;
}


void Cservers::getIp(int server, char *ip,size_t maxArray)
{
	if (server>=SERVER_SLOTS) return;
	strncpy(ip,svrs[server].ip,maxArray);
}

char *Cservers::lgetIp(int server)
{
	if (server>=SERVER_SLOTS) return "";
	strncpy(returned,svrs[server].ip,RETURNED_LENGHT);
	return returned;
}
char *Cservers::lgetNameServer(int server)
{
	if (server>=SERVER_SLOTS) return "";
	strncpy(returned,svrs[server].nameserver ,RETURNED_LENGHT);
	return returned;
}


void Cservers::setIp(int server, char *ip)
{
	if (server>=SERVER_SLOTS) return;
	strncpy(svrs[server].ip,ip,IP_ADDRESS_LENGHT);
}
void Cservers::getNameServer(int server, char *ns,size_t maxArray)
{
	if (server>=SERVER_SLOTS) return;
	strncpy(ns,svrs[server].nameserver,maxArray);
}
void Cservers::setNameServer(int server, char *ns)
{
	if (server>=SERVER_SLOTS) return;
	strncpy(svrs[server].nameserver,ns,NAME_SERVER_LENGHT);
}
bool Cservers::getprx_uplink(int server)
{
	if (server>=SERVER_SLOTS) return false;
	return svrs[server].prx_uplink;
}
void Cservers::setprx_uplink(int server,bool bsy)
{
	if (server>=SERVER_SLOTS) return;
	svrs[server].prx_uplink=bsy;
}
bool Cservers::getprx_upuser(int server)
{
	if (server>=SERVER_SLOTS) return false;
	return svrs[server].prx_upuser;
}
void Cservers::setprx_upuser(int server,bool bsy)
{
	if (server>=SERVER_SLOTS) return;
	svrs[server].prx_upuser=bsy;
}
bool Cservers::getprx_upbounce(int server)
{
	if (server>=SERVER_SLOTS) return false;
	return svrs[server].prx_upbounce;
}
void Cservers::setprx_upbounce(int server,bool bsy)
{
	if (server>=SERVER_SLOTS) return;
	svrs[server].prx_upbounce=bsy;
}
bool Cservers::getprx_logdata(int server)
{
	if (server>=SERVER_SLOTS) return false;
	return svrs[server].prx_logdata;
}
void Cservers::setprx_logdata(int server,bool bsy)
{
	if (server>=SERVER_SLOTS) return;
	svrs[server].prx_logdata=bsy;
}
void Cservers::getKey(int server, char *key,size_t maxArray)
{
	if (server>=SERVER_SLOTS) return;
	strncpy(key,svrs[server].key,maxArray);
}
void Cservers::setKey(int server, char *key)
{
	if (server>=SERVER_SLOTS) return;
	strncpy(svrs[server].key,key,NAME_SERVER_LENGHT);
}
SOCKET Cservers::getSocket(int server)
{
	if (server>=SERVER_SLOTS) return 0;
	return svrs[server].f;
}
void Cservers::setSocket(int server, SOCKET sock)
{
	if (server>=SERVER_SLOTS) return;
	svrs[server].f=sock;
}
unsigned long Cservers::getRecvBytes(int server)
{
	if (server>=SERVER_SLOTS) return 0;
	return svrs[server].received_bytes;
}
void Cservers::setRecvBytes(int server, unsigned long sb)
{
	if (server>=SERVER_SLOTS) return;
	svrs[server].received_bytes=sb;
}
void Cservers::addRecvBytes(int server, unsigned long sb)
{
	if (server>=SERVER_SLOTS) return;
	svrs[server].received_bytes+=sb;
}
unsigned long Cservers::getSentBytes(int server)
{
	if (server>=SERVER_SLOTS) return 0;
	return svrs[server].transmited_bytes;
}
void Cservers::setSentBytes(int server, unsigned long sb)
{
	if (server>=SERVER_SLOTS) return;
	svrs[server].transmited_bytes=sb;
}
void Cservers::addSentBytes(int server, unsigned long sb)
{
	if (server>=SERVER_SLOTS) return;
	svrs[server].transmited_bytes+=sb;
}

time_t Cservers::getConnectionTime(int server)
{
	if (server>=SERVER_SLOTS) return 0;
	return svrs[server].connected_when;
}
int Cservers::findpipefree()
{
	for (int fnd=0; fnd<SERVER_SLOTS;fnd++)
	{
		if (!svrs[fnd].busy) return fnd;
	}
	return -1;
}
void Cservers::freeSlot(int slotId)
{
	if (slotId>=SERVER_SLOTS) return;
	closesocket(svrs[slotId].f);
	svrs[slotId].busy=FALSE;
	svrs[slotId].cbsy=FALSE;
	svrs[slotId].blocked=FALSE;
	svrs[slotId].f=0;
	strcpy(svrs[slotId].ip,"0.0.0.0");
	strcpy(svrs[slotId].nameserver,"none");
	svrs[slotId].transmited_bytes=0;
	svrs[slotId].received_bytes=0;
}
int Cservers::getServerSlots()
{
	return SERVER_SLOTS;
}