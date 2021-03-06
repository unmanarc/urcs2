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
#include "protocol.h"
#include "ini_man.h"
#include "funcs.h"
#include "core_s.h"
#include "logger.h"

#define GRP_PUBLIC ",SLEEP,SU,MD5,CLS,CLREOL,DELLINE,INSLINE,ECHO,PROMPTENV,PROMPTENVPASS,FREEENV,GOTOXY,BACKCOLOR,TEXTCOLOR,SETENV,CUT,CMP,STRSTR,PUSH,FLUSH,POP,ADD,MUL,DIV,RUN,HELP,"
#define GRP_ROUTE ",PRX_ATTACH,PRX_CONNECT,PRX_WHO,NET_LOOKUP,SLEEP,SU,MD5,CLS,CLREOL,DELLINE,INSLINE,ECHO,PROMPTENV,PROMPTENVPASS,FREEENV,GOTOXY,BACKCOLOR,TEXTCOLOR,SETENV,CUT,CMP,STRSTR,PUSH,FLUSH,POP,ADD,MUL,DIV,RUN,HELP,"
#define GRP_USER ",PRX_CLOSE,LS,WINEXEC,CP,MKDIR,RMDIR,MV,RM,PS,DF,CD,CAT,TOUCH,FOPEN,FGETS,FGETC,FPUTS,FPUTC,FCLOSE,MSGBOX,WHO,HELP,PASSWD,UPLOADTO,DOWNLOADFROM,UPLOAD,DOWNLOAD,BANNER,PRX_ATTACH,PRX_CONNECT,PRX_WHO,NET_LOOKUP,SLEEP,SU,MD5,CLS,CLREOL,DELLINE,INSLINE,ECHO,PROMPTENV,PROMPTENVPASS,FREEENV,GOTOXY,BACKCOLOR,TEXTCOLOR,SETENV,CUT,CMP,STRSTR,PUSH,FLUSH,POP,ADD,MUL,DIV,RUN,CMD,NET_DOWNLOAD,"
#define GRP_TCP ",PRX_CLOSE,PRX_ATTACH,PRX_CONNECT,PRX_WHO,NET_LOOKUP,SLEEP,SU,MD5,CLS,CLREOL,DELLINE,INSLINE,ECHO,PROMPTENV,PROMPTENVPASS,FREEENV,GOTOXY,BACKCOLOR,TEXTCOLOR,SETENV,CUT,CMP,STRSTR,PUSH,FLUSH,POP,ADD,MUL,DIV,RUN,HELP,NET_SEND,NET_OPENSOCKET,NET_SET_TIMEOUT,NET_CLOSESOCKET,NET_LOOKUP,NET_SENDTO,NET_RECVFROM,PRX_BOUNCE,"


class Cintep
{
public:
	Cintep(); 
	void start_intep(con_v mx[SERVER_CONNECTIONS], int local_user, char *newkey,Cproto * proto,Cfns * fnsx,Cini * data_gx,Clog * _logx); //Start interpreter

	int	printprompt(con_v mx[SERVER_CONNECTIONS], int local_user); //print the prompt
	int run(con_v mx[SERVER_CONNECTIONS], Cservers *svrs,int local_user,HINSTANCE hisd[MAX_MODULES]);//Run command
	int firstinstall(con_v mx[SERVER_CONNECTIONS], int xlogon); //for first install

	BOOL passed;
	~Cintep();

protected:
	void prg_su(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: change user (su)

	void prg_compile(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: compile scripts into UMA
	void prg_exec2(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: execute UMA files

	void prg_run(con_v mx[SERVER_CONNECTIONS],Cservers *svrs, int xlogon,HINSTANCE hisd[MAX_MODULES]);
	void prg_echo(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: echo
	void prg_sleep(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: Sleep
	void prg_md5(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: MD5 string
	void prg_cls(con_v mx[SERVER_CONNECTIONS], int xlogon); //ClearScreen

	void prg_clreol(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_delline(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_insline(con_v mx[SERVER_CONNECTIONS], int xlogon);

	void prg_msgbox(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_ls(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: LS - List files
	void prg_exec(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: Exec - Run files
	void prg_cmd(con_v mx[SERVER_CONNECTIONS], int xlogon);  //cmd run files as console
		void ExecAndGetOutput( LPCSTR szCommand );
		int ReadFromPipeNoWait( HANDLE hPipe, char* pDest, int nMax );

	void prg_cp(con_v mx[SERVER_CONNECTIONS], int xlogon); // Copy files
	void prg_mkdir(con_v mx[SERVER_CONNECTIONS], int xlogon); //Makes an directory
	void prg_rmdir(con_v mx[SERVER_CONNECTIONS], int xlogon); //Remove an directory
	void prg_mv(con_v mx[SERVER_CONNECTIONS], int xlogon); //Moves file
	void prg_rm(con_v mx[SERVER_CONNECTIONS], int xlogon); //Remove files
	void prg_ps(con_v mx[SERVER_CONNECTIONS], int xlogon); //Process list
	void prg_cd(con_v mx[SERVER_CONNECTIONS], int xlogon); //Change Directory.
	void prg_cat(con_v mx[SERVER_CONNECTIONS], int xlogon); //show file
	void prg_df(con_v mx[SERVER_CONNECTIONS], int xlogon); //Disk free.
	void prg_who(con_v mx[SERVER_CONNECTIONS], int xlogon); //List connected users...
	void prg_kill(con_v mx[SERVER_CONNECTIONS], int xlogon); //Kill task
	void prg_killall(con_v mx[SERVER_CONNECTIONS], int xlogon); //Kill multiple tasks

	void prg_lscmd(con_v mx[SERVER_CONNECTIONS], int xlogon); //help

	void prg_reg_getkey(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_reg_putkey(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_reg_delkey(con_v mx[SERVER_CONNECTIONS], int xlogon);

	void prg_shutdown(con_v mx[SERVER_CONNECTIONS], int xlogon); //Shutdown PC

	//file stream manipulator.
	void prg_touch(con_v mx[SERVER_CONNECTIONS], int xlogon); //creates empty files
	void prg_fopen(con_v mx[SERVER_CONNECTIONS], int xlogon); //open files
	void prg_fclose(con_v mx[SERVER_CONNECTIONS], int xlogon); //close files
	void prg_fputs(con_v mx[SERVER_CONNECTIONS], int xlogon); //put string to file
	void prg_fgets(con_v mx[SERVER_CONNECTIONS], int xlogon); //get line from file
	void prg_fgetc(con_v mx[SERVER_CONNECTIONS], int xlogon); //get character from file
	void prg_fputc(con_v mx[SERVER_CONNECTIONS], int xlogon); //put character to file

	void prg_cmp(con_v mx[SERVER_CONNECTIONS], int xlogon); //compare 2 strings.
	void prg_add(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: add
	void prg_div(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: divide
	void prg_mul(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: multiply


	void prg_flush(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_push(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_pop(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_freeenv(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: removes entry from vars.
	void prg_setenv(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: set enviroment var.
	void prg_promptenv(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: Prompt and save to enviroment var.
	void prg_cut(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: modify enviroment variable to cut some field of data.
	void prg_strstr(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: //strstr search some ocurrence of any formated string into another formated string.
	void prg_promptenvpass(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: Prompt and save to enviroment var (password).
	void prg_textcolor(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_backcolor(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_gotoxy(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: gotoxy

	void prg_install(con_v mx[SERVER_CONNECTIONS], int xlogon); //installing system
	void prg_mkuser(con_v mx[SERVER_CONNECTIONS], int xlogon); //MAke user
	void prg_rmuser(con_v mx[SERVER_CONNECTIONS], int xlogon); //deletes user

	void prg_chuser(con_v mx[SERVER_CONNECTIONS], int xlogon);  //Changes User information
	void prg_mklevel(con_v mx[SERVER_CONNECTIONS], int xlogon); //make new level
	void prg_rmlevel(con_v mx[SERVER_CONNECTIONS], int xlogon); //remove level
	void prg_chlevel(con_v mx[SERVER_CONNECTIONS], int xlogon); //change level commands.
	void prg_lslevel(con_v mx[SERVER_CONNECTIONS], int xlogon); //List level


	void prg_prompt(con_v mx[SERVER_CONNECTIONS], int xlogon); //Change prompt
	void prg_log(con_v mx[SERVER_CONNECTIONS], int xlogon); //active/desactive logs
	void prg_passwd(con_v mx[SERVER_CONNECTIONS], int xlogon); //change password
	void prg_banner(con_v mx[SERVER_CONNECTIONS], int xlogon); //change banner
	void prg_sname(con_v mx[SERVER_CONNECTIONS], int xlogon); //change server_name
	void prg_sport(con_v mx[SERVER_CONNECTIONS], int xlogon); //change server_port
	void prg_mname(con_v mx[SERVER_CONNECTIONS], int xlogon); //change mother_port

	void prg_restart(con_v mx[SERVER_CONNECTIONS], int xlogon); //restart server
	void prg_restart_mother(con_v mx[SERVER_CONNECTIONS], int xlogon); //restart connection with mother
	void prg_upgrade(con_v mx[SERVER_CONNECTIONS], int xlogon); //upgrade server with mother server version

	void prg_net_send(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_receive(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_opensocket(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_set_timeout(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_download(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_closesocket(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_lookup(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_sendto(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_recvfrom(con_v mx[SERVER_CONNECTIONS], int xlogon);

	void prg_prx_who(con_v mx[SERVER_CONNECTIONS],Cservers *svrs, int xlogon);
	void prg_prx_close(con_v mx[SERVER_CONNECTIONS],Cservers *svrs, int xlogon);
	void prg_prx_connect(con_v mx[SERVER_CONNECTIONS],Cservers *svrs, int xlogon);
	void prg_prx_attach(con_v mx[SERVER_CONNECTIONS],Cservers *svrs, int xlogon);
	void prg_prx_uplink(con_v mx[SERVER_CONNECTIONS],Cservers *svrs, int xlogon);
	void prg_prx_bounce(con_v mx[SERVER_CONNECTIONS],Cservers * svrs, int xlogon);
	void prg_prx_detail(con_v mx[SERVER_CONNECTIONS],Cservers * svrs, int xlogon);

	
	void prg_downloadfrom(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_uploadto(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_download(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_upload(con_v mx[SERVER_CONNECTIONS], int xlogon);

	void prg_probe(con_v mx[SERVER_CONNECTIONS], int xlogon);

	BOOL bye;
	Clog * _log;
	Cproto * nproto;
	Cfns * fns;
	Cini * data_g;
	int z;
	BOOL lol;

};