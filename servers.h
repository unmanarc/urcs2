#pragma once

#include "winsock2.h"
#include "time.h"

#define NAME_SERVER_LENGHT 256
#define IP_ADDRESS_LENGHT 128
#define KEY_LENGHT 32

typedef struct
{
	bool busy;
	bool cbsy;
	bool blocked;
	char ip[IP_ADDRESS_LENGHT];
	char nameserver[NAME_SERVER_LENGHT];
	char key[KEY_LENGHT];
	unsigned long transmited_bytes;
	unsigned long received_bytes;
	time_t connected_when;
	
	bool prx_uplink;
	bool prx_upuser;
	bool prx_upbounce;
	bool prx_logdata;

	SOCKET f;
} Servers_struct;

class Cservers
{
public:
	Cservers(void);
	
	bool getprx_uplink(int server);
	void setprx_uplink(int server,bool bsy);

	bool getprx_upuser(int server);
	void setprx_upuser(int server,bool bsy);

	bool getprx_upbounce(int server);
	void setprx_upbounce(int server,bool bsy);

	bool getprx_logdata(int server);
	void setprx_logdata(int server,bool bsy);

	bool isBusy(int server);
	void setBusy(int server,bool bsy);

	bool isClientBusy(int server);
	void setClientBusy(int server,bool bsy);

	bool isBlocked(int server);
	void setBlocked(int server,bool bsy);

	void getIp(int server, char *ip,size_t maxArray);
	void setIp(int server, char *ip);

	void getNameServer(int server, char *ns,size_t maxArray);
	void setNameServer(int server, char *ns);

	void getKey(int server, char *key,size_t maxArray);
	void setKey(int server, char *key);

	SOCKET getSocket(int server);
	void setSocket(int server, SOCKET sock);

	unsigned long getRecvBytes(int server);
	void setRecvBytes(int server, unsigned long sb);

	unsigned long getSentBytes(int server);
	void setSentBytes(int server, unsigned long sb);

	void addSentBytes(int server, unsigned long sb);
	void addRecvBytes(int server, unsigned long sb);

	void freeSlot(int slotId);
	char *lgetIp(int server);
	char *lgetNameServer(int server);
	int getServerSlots();

	time_t getConnectionTime(int server);

	int findpipefree();
	~Cservers(void);
protected:
	Servers_struct svrs[SERVER_SLOTS];

};
