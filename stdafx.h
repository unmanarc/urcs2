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
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define SERVER_SLOTS 128
#define SERVER_CONNECTIONS 128
#define MEMORY_FOR_USER 32770 
#define HASH_DEFAULT ""

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <afxwin.h>         // MFC core and standard components (importante para la libreria MD5)
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "winsock2.h"

// TODO: reference additional headers your program requires here
typedef struct conections
{
	BOOL busy; // this connection is busy?
	BOOL proxy; // this connection is a proxy or normal user?
	char c_Cwd[256]; //Current working directory for this user
	int user_range; // range of user
	// -1: blocked user
	// 0: Admin 
	// 1: User 
	// 2: Chat user
	// others: reserved for nexts uses...
	char cmdline[1024];
	char localdir[_MAX_PATH];
	char ip_from[256];
	char from[255]; 
	char c_User[512];
	char c_Pass[512];
	char mem[MEMORY_FOR_USER];
	
	time_t since;
	SOCKET socket;
}con_v;

typedef struct
{
	BOOL busy;
	BOOL cbsy;
	char ip[17];
	char nameserver[512];
	char key[32];
	SOCKET f;

}ers_svr;

