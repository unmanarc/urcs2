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

#define VIRTUAL_DATA 256
#define SERVER_SLOTS 128
#define SERVER_CONNECTIONS 128
#define FILE_SLOTS 256
#define COMMAND_LINE 4092

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
#include "mem_man.h"

// TODO: reference additional headers your program requires here
typedef struct vcpu
{
	BOOL b_a;
	BOOL b_b;
	BOOL b_c;
	BOOL b_d;
	unsigned short s_a;
	unsigned short s_b;
	unsigned short s_c;
	unsigned short s_d;
	int i_a;
	int i_b;
	int i_c;
	int i_d;
	char data[VIRTUAL_DATA];
}virtual_cpu;

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
	char cmdline[COMMAND_LINE];
	char localdir[_MAX_PATH];
	char ip_from[256];
	char from[255]; 
	char c_User[512];
	char c_Pass[512];
	FILE *files[FILE_SLOTS];
	Cmm m_mem;
	virtual_cpu cpu;
	
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


