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

#define EXEC_VERSION 1

#define VIRTUAL_DATA 256
#define SERVER_SLOTS 256
#define SERVER_CONNECTIONS 256
#define FILE_SLOTS 256
#define COMMAND_LINE 4092
#define MAX_MODULES 128

#define HASH_DEFAULT ""
#define BANNER "URCS - Unmanarc Remote Control Server - 2.13 - Platinium"
#define INSTALL_URCS_01 "Welcome to URCS setup program"
#define INSTALL_URCS_02 "Welcome to URCS Global Config"
#define INSTALL_URCS_03 "Welcome to URCS Install Setup"
#define INSTALL_URCS_04 "Welcome to URCS Install Setup"
#define INSTALL_URCS_05 "Welcome to URCS Proxy install"
#define INSTALL_URCS_06 "Installing URCS, please wait."


#define SHELLBANNER1 "    /\\                                    Unmanarc Remote Control Server - URCS"
#define SHELLBANNER2 "  / | \\                                                        2.13 - Platinium"
#define SHELLBANNER3 "/   |  \\                                              http://urcs.unmanarc.com/"
#define SHELLBANNER4 "---------                                 -------------------------------------"


//MACROS para texcolor, textbackground, etc.
#define BLACK		 0	//NEGRO
#define BLUE		 1	//AZUL MARINO
#define GREEN		 2	//VERDE PERICO
#define CYAN		 3	//VERDE CLARO
#define RED			 4	//ROJO
#define MAGENTA		 5	//MORADO O VIOLETA
#define BROWN		 6	//CAFÉ
#define LIGHTGRAY	 7	//GRIS CLARO
#define DARKGRAY	 8	//GRIS OBSCURO
#define LIGHTBLUE	 9	//AZUL MARINO CLARO
#define LIGHTGREEN	 10	//VERDE CLARO
#define LIGHTCYAN	 11	//AZUL CLARO
#define LIGHTRED	 12	//ROJO CLARO
#define LIGHTMAGENTA 13	//ROSA MEXICANO
#define YELLOW		 14	//AMARILLO
#define WHITE		 15	//BLANCO


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

int recv2(SOCKET s,char *buf,int len, int flags);

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

typedef struct proxyinf
{
	char proxy[512];
	int port;
	BOOL have_proxy;
}s_proxyinf;

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
	char title[512];
	FILE *files[FILE_SLOTS];
	Cmm m_mem;
	virtual_cpu cpu;
	bool remote;

	time_t since;
	SOCKET socket;

}con_v;


