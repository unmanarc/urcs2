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

class Cproto
{
public:
	Cproto(); 
	// Protocol functions:
	BOOL start_socket(SOCKET s, BOOL encrypt); //Start negotiation with client
	void deny_connection(SOCKET s);
	char *getkey();
	void continue_socket(SOCKET s, char *newkey);
	int sendclose();
	//variables...
	char in_ip[256];

	//proxy protocol.:
	BOOL prx;
	char nameserver[512];

	int setpushmode(BOOL pshmode);
	char *l_pop(int max);
	int l_flush();

	//For server rutines:
	int sendfile(char *remotefilename, char *localfilename);
	int recvfile(char *remotefilename, char *localfilename);


	int senddata(char *data); //Send NULL-TERMINATED string to socket
	int getdline(char *line); //Get 10-13 terminated line.
	int getdnline(char *line,int max); //Get 10-13 terminated line with maximum
	int getdnpass(char *line,int max); //Get 10-13 terminated pass with maximum
	int getnchar(char *chr, int chars); //Get n characters from host
	int receive(char *data);
	int getproxyline();
	int GetBlock(void *p, int len);
	int cls();

	~Cproto();

protected:
	int l_push(char *data);
	SOCKET x;
	char key[32];
	FILE *tempfile;
	BOOL pushmode;
	fpos_t w_pos;
	fpos_t r_pos;

};