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
}
Csc::~Csc()
{
}

BOOL Csc::openscript(char *filename)
{
	if ((fp=fopen(filename,"r"))!=NULL)
	{
		char buffer[COMMAND_LINE];
		fgets(buffer,COMMAND_LINE,fp); //getting first line.
		strncpy(filenamex,filename,_MAX_PATH);
		if (!strncmp(buffer,"startscr",8)) return TRUE;
		else return FALSE;
	}
	else return FALSE;
}
char *Csc::get_next_line()
{
	if (fgets(lnx,COMMAND_LINE,fp)==NULL)
		return "exitscript";
	return lnx;
}
BOOL Csc::setlinepos(int ln)
{
	fclose(fp); //closes old handles...
	if ((fp=fopen(filenamex,"r"))!=NULL)
	{
		char buffer[COMMAND_LINE];
		fgets(buffer,COMMAND_LINE,fp); //getting first line.
		if (!strncmp(buffer,"startscr",8)) 
		{
			for(int p=0;p<ln;p++)
				if (fgets(lnx,COMMAND_LINE,fp)==NULL) return FALSE; //unspected end of file
			return TRUE;
		}
		else return FALSE;
	}
	else return FALSE;
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

