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
// stdafx.cpp : source file that includes just the standard includes
// urcs.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
int recv2(SOCKET s,char *buf,int len, int flags)
{
	int bytesRecv = SOCKET_ERROR,totalRecv = 0;
	while( bytesRecv == SOCKET_ERROR || totalRecv<len) 
	{
		bytesRecv = recv( s, buf+totalRecv, len-totalRecv, flags );
		if ( bytesRecv == 0 || bytesRecv == WSAECONNRESET || bytesRecv==0xFFFFFFFF ) // Conexion Cerrada.
			return -1; 
		if ( bytesRecv != SOCKET_ERROR) totalRecv+= bytesRecv;
	}
	return len;
}