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
#include "StdAfx.h"
#include "executer.h"
#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include <io.h>


Cexecuter::Cexecuter(void)
{
}

Cexecuter::~Cexecuter(void)
{
}
int Cexecuter::openexec(char *filename)
{
	if( (fh = _open(filename , _O_RDONLY )) == -1 )
		return -1; //failed to open file
	else
	{
		//read header...
		int lec=0;
		if (( lec = _read( fh, &head, sizeof(head) ) ) > 0)
		{
			//readed...
			if (strcmp(head.file_info,"UMA")) return -2; //unknown filetype
			if (head.version>EXEC_VERSION) return -3; //uncompatible format
			page=(void *)malloc(head.page_size); //ready for read first page...
			memset(&page,0,head.page_size); //setting to 0
			//bye
			return 1;
		}
	}
}
void Cexecuter::closeexec()
{
	_close( fh );
}
void Cexecuter::gotolabel(char *labelname)
{
	//first search label on paged memory.

}

