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

#include "funcs.h"


class Csc
{
public:
	Csc();
	BOOL openscript(char *filename);
	char *get_next_line();
	BOOL setlinepos(int ln);
	int search_label(char *label);
	int close_script();
	~Csc();
protected:
	FILE *fp;
	char filenamex[_MAX_PATH];
	char lnx[COMMAND_LINE];
	Cfns fnca;
};