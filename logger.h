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
#include "ini_man.h"

class Clog
{
public:
	Clog(Cini *inix,Cfns *fnsx); 
	void logg(char *ip,char *module, char *data, char *username);
	~Clog();
protected:
	void write_to_log(char *data);
	Cini *ini;
	Cfns *fns;
};
