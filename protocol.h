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

class Cproto
{
public:
	Cproto(Cini *inix); 
	// Protocol functions:
	bool start_socket(SOCKET s, BOOL encrypt); //Start negotiation with client
	void deny_connection(SOCKET s);
	char *getkey();
	void continue_socket(SOCKET s, char *newkey);
	int sendclose();
	//variables...
	char in_ip[256];

	bool isProxyIn();
	bool isAuthIn();
	void getServerName(char *name, size_t max_lenght);
	bool get_prx_uplink();
	bool get_prx_upuser();
	bool get_prx_upbounce();
	bool get_prx_logdata();

	//proxy protocol.:

	int setpushmode(BOOL pshmode);
	char *l_pop(int max);
	int l_flush();

	//For server rutines:
	int sendfile(char *remotefilename, char *localfilename);
	int recvfile(char *remotefilename, char *localfilename);
	int senddatahex(char *data, int lenght); //send as maximum 4094 bytes of data hex.
	int senddata(char *data); //Send NULL-TERMINATED string to socket
	int sendline(char *data); //send NULL-TERMINATED string to socket and append \n
	int senddatacenter(char *data, short line); //Send data centered.
	int getdline(char *line); //Get 10-13 terminated line.
	int getdnline(char *line,int maxi); //Get 10-13 terminated line with maximum
	int getdnpass(char *line,int maxi); //Get 10-13 terminated pass with maximum
	int comparehash(char *hash, char *gout);
	int getnchar(char *chr, int chars); //Get n characters from host
	int setcolor(unsigned short color);
	int setdefaultcolor();
	int setbackground(unsigned short color);
	int setposxy(int posx, int posy);
	int clreol(void);//Borrar hasta el final de línea
	int delline(void);//Eliminar la línea actual
	int insline(void);//Insertar una nueva línea

	int settitle(char *title);
	int drawRectangle(unsigned short x,unsigned short y,unsigned short w,unsigned short h,unsigned short line_type,unsigned short line_color,unsigned short filled_color);
	
	int setAction(char *action);
	int setActionStatus(bool success);

	
	int receive(char *data, size_t maxdatasize);
	int getproxyline();
	int GetBlock(void *p, int len);
	int cls();

	void start_remote_auth_server();
	void start_remote_auth_server_false();
	int start_remote_auth_client(char *username, char *pass, char *germen);

	~Cproto();

protected:
	int l_push(char *data);
	SOCKET x;
	char key[32];
	FILE *tempfile;
	BOOL pushmode;
	fpos_t w_pos;
	fpos_t r_pos;
	bool prx,auth;

	bool prx_uplink;
	bool prx_upuser;
	bool prx_upbounce;
	bool prx_logdata;

	char nameserver[256];
	Cini *ini;

};