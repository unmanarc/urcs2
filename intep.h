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

class Cintep
{
public:
	Cintep(); 
	void start_intep(con_v mx[SERVER_CONNECTIONS], int local_user, char *newkey); //Start interpreter

	int	printprompt(con_v mx[SERVER_CONNECTIONS], int local_user); //print the prompt
	int run(con_v mx[SERVER_CONNECTIONS],ers_svr svrs[SERVER_SLOTS],int local_user); //Run command
	void firstinstall(con_v mx[SERVER_CONNECTIONS], int xlogon); //for first install

	BOOL passed;
	~Cintep();

protected:

	void prg_run(con_v mx[SERVER_CONNECTIONS],ers_svr svrs[SERVER_SLOTS], int xlogon);
	void prg_echo(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: echo
	void prg_sleep(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: Sleep
	void prg_md5(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: MD5 string
	void prg_cls(con_v mx[SERVER_CONNECTIONS], int xlogon); //ClearScreen

	void prg_msgbox(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_ls(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: LS - List files
	void prg_exec(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: Exec - Run files
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
	void prg_lscmd(con_v mx[SERVER_CONNECTIONS], int xlogon); //help
	void prg_passwd(con_v mx[SERVER_CONNECTIONS], int xlogon); //change password
	void prg_shutdown(con_v mx[SERVER_CONNECTIONS], int xlogon); //Shutdown PC


	//file stream manipulator.
	void prg_fopen(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_fclose(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_fputs(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_fgets(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_fgetc(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_fputc(con_v mx[SERVER_CONNECTIONS], int xlogon);

	void prg_cmp(con_v mx[SERVER_CONNECTIONS], int xlogon);

	void prg_freeenv(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: removes entry from vars.
	void prg_setenv(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: set enviroment var.
	void prg_promptenv(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: Prompt and save to enviroment var.

	void prg_prompt(con_v mx[SERVER_CONNECTIONS], int xlogon); //Change prompt
	void prg_mkuser(con_v mx[SERVER_CONNECTIONS], int xlogon); //MAke user
	void prg_rmuser(con_v mx[SERVER_CONNECTIONS], int xlogon); //deletes user

	void prg_net_send(con_v mx[SERVER_CONNECTIONS], int xlogon);
	//void prg_net_sendto(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_receive(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_opensocket(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_set_timeout(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_closesocket(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_lookup(con_v mx[SERVER_CONNECTIONS], int xlogon);

	void prg_prx_who(con_v mx[SERVER_CONNECTIONS],ers_svr svrs[SERVER_SLOTS], int xlogon);
	void prg_prx_close(con_v mx[SERVER_CONNECTIONS],ers_svr svrs[SERVER_SLOTS], int xlogon);
	void prg_prx_connect(con_v mx[SERVER_CONNECTIONS],ers_svr svrs[SERVER_SLOTS], int xlogon);
	
	void prg_downloadfrom(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_uploadto(con_v mx[SERVER_CONNECTIONS], int xlogon);

	Cproto nproto;
	Cfns fns;
	Cini data_g;
	int z;

};