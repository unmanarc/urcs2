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
#include "scripting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


Csc::Csc()
{ //Constructor
	local_line=0;
}
Csc::~Csc()
{
}

char *Csc::getlinen(char *scri, int line, int lenght)
{
	//search for x numbers of \n
	char *result;
	result=(char *)malloc(lenght);
	int ca=0;
	int l=0;
	int y=0;
	while(l<line)
	{
		if (scri[y]=='\n') l++;
        y++;
		if (y==lenght) return NULL;
	}
	ca=y;
	l=0;
	y=0;
	while(l<line)
	{
		if (scri[y]=='\n') l++;
        y++;
		if (y==lenght) return NULL;
	}
	strncpy(result,scri+ca,y-ca);
	return result;
}

BOOL Csc::setlocalscript(char *script, int lenght)
{
	scr=(char *)malloc(lenght);
	strncpy(scr,script,lenght);
	tot=lenght;
	local_line=0;
	return TRUE;
}

char *Csc::get_next_local_line()
{
	if (getlinen(scr,local_line,tot)==NULL) return NULL;
	else return getlinen(scr,local_line,tot);
	local_line++;
}
BOOL Csc::setlocallinepos(int ln)
{
	if (getlinen(scr,ln,tot)==NULL) return NULL;
	local_line=ln;
	return TRUE;
}
int Csc::search_local_label(char *label)
{
	int linetoget=0;
	while (getlinen(scr,linetoget,tot)!=NULL)
	{
		if (!strcmp(label,getlinen(scr,linetoget,tot)))
			return linetoget;
	}
	return -1;
}
BOOL Csc::openscript(char *filename)
{
	if ((fp=fopen(filename,"r"))!=NULL)
	{
		char buffer[COMMAND_LINE];
		fgets(buffer,COMMAND_LINE,fp); //getting first line.
		strncpy(filenamex,filename,_MAX_PATH);
		if (!strncmp(buffer,"startscr",8))
		{
			local_line=0;
			return TRUE;
		}
		else return FALSE;
	}
	else return FALSE;
}
char *Csc::get_next_line()
{
	local_line++;
	if (fgets(lnx,COMMAND_LINE,fp)==NULL)
		return "exitscript";
	return lnx;
}
int Csc::getlinepos()
{
	return local_line;
}
BOOL Csc::setlinepos(int ln)
{
//	fclose(fp); //closes old handles...
	fpos_t ty=0;
	fsetpos(fp,&ty);
//	if ((fp=fopen(filenamex,"r"))!=NULL)
//	{
		char buffer[COMMAND_LINE];
		fgets(buffer,COMMAND_LINE,fp); //getting first line.
		if (!strncmp(buffer,"startscr",8)) 
		{
			for(int p=0;p<ln;p++)
				if (fgets(lnx,COMMAND_LINE,fp)==NULL) return FALSE; //unspected end of file
			local_line=ln;
			return TRUE;
		}
		else return FALSE;
//	}
//	else return FALSE;
}

int Csc::search_label(char *label)
{
	FILE *fc;
	char nlabel[COMMAND_LINE];
	strcpy(nlabel,":");
	strncat(nlabel,label,COMMAND_LINE-1);
   	if ((fc=fopen(filenamex,"r"))!=NULL)
	{
		char buffer[COMMAND_LINE];
		fgets(buffer,COMMAND_LINE,fc); //getting first line.
		if (!strncmp(buffer,"startscr",8)) 
		{
			int ct=0;
			while(fgets(lnx,COMMAND_LINE,fc)!=NULL) 
			{
				fnca.denter(lnx);
				if (!strcmp(nlabel,lnx))
				{
					fclose(fc);
					return ct;
				}
				ct++;
			}
			return -3;
		}
		else return -2;
	}
	else return -1;

}
int Csc::close_script()
{
	return fclose(fp);
}

