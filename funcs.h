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

class Cfns
{
public:
	Cfns(); 
	~Cfns();

	long GetFreeDiskSpaceInKB(LPSTR pFile); //return free disk space in KB
	int strpointer(char *pointer, char point); //Return a position of first matching char
	int strrpointer(char *pointer, char point); //Return a position of first matching char (inverse)
	int charhextointa(char dsi); //convert a one byte representing hex code, to int (0-F -> 0-16)
	int charhextoint(char *dsi); //Convert two bytes in hexcode to int (00-FF -> 000-255)
	int find32(char *strfnd, int pos32); //find the first (->) /32 /10 /13 or /0 (space, enter, return, null)
	void filterstring(char *strfil); //remove the double /32 and upperstring.
	void chrcat(char *strc, char add); // cat a byte to char*
	void denter(char *crecpt); //removes /10 and /13
	void dep32(char *depu);//removes the /32 of a seq.
	void renter(char *crecpt);//ad /13 to /10 expresions
	char *deparg(char *depu, char *arg, BOOL nax); //return the arguments and depure
	char *convert(char *instr, con_v mx[SERVER_CONNECTIONS], int xlogon);//Transform $ lang.
	char *convert_vars(char *instr, con_v mx[SERVER_CONNECTIONS], int xlogon);//Transform % lang.
	char *depstring(char *dep);//convert all /32 to /0
	char *md5sum(char *value); //calculate md5 sum of any string

	BOOL cmpfirstword(char *WordVariable,char *WordStatic); //compare the first word of a string
	BOOL KillProcess(unsigned long pid); //kill a process

	time_t TimetFromFt(FILETIME pft); //Convert a file-time to time_t

protected:
	char returned[COMMAND_LINE];
};
