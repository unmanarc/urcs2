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
#include "logger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


Clog::Clog(Cini *inix,Cfns *fnsx)
{ //Constructor
	ini=inix;
	fns=fnsx;
}
Clog::~Clog()
{ //Destructor

}
void Clog::logg(char *ip,char *module, char *data, char *username)
{
	size_t tot=strlen(ip)+strlen(module)+strlen(username)+strlen(data)+512;
	time_t now;
	time(&now);
	char *formed;
	char date[256];
	strncpy(date,ctime(&now),sizeof(date));
	fns->denter2(date);
	formed=(char *)malloc(tot);
	_snprintf(formed,tot,"[%s %s:%-12s@%-15s]: %s\n",date,module,username,ip,data);
	write_to_log(formed);
}
void Clog::write_to_log(char *data)
{
	char llfile[_MAX_PATH];
	_snprintf(llfile,sizeof(llfile),"%s\\urcs2.log",ini->windir);
    FILE *fp;
	if ((fp=fopen(llfile,"a+"))==NULL) return;
	fputs(data,fp);
	fclose(fp);
}