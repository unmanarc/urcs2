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

	void prg_echo(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: echo
	void prg_sleep(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: Sleep
	void prg_md5(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: MD5 string

	void prg_ls(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: LS - List files
	void prg_exec(con_v mx[SERVER_CONNECTIONS], int xlogon); //program: Exec - Run files
	void prg_cp(con_v mx[SERVER_CONNECTIONS], int xlogon); // Copy files
	void prg_mv(con_v mx[SERVER_CONNECTIONS], int xlogon); //Moves file
	void prg_rm(con_v mx[SERVER_CONNECTIONS], int xlogon); //Remove files
	void prg_ps(con_v mx[SERVER_CONNECTIONS], int xlogon); //Process list
	void prg_cd(con_v mx[SERVER_CONNECTIONS], int xlogon); //Change Directory.
	void prg_cat(con_v mx[SERVER_CONNECTIONS], int xlogon); //show file
	void prg_who(con_v mx[SERVER_CONNECTIONS], int xlogon); //List connected users...
	void prg_kill(con_v mx[SERVER_CONNECTIONS], int xlogon); //Kill task
	void prg_lscmd(con_v mx[SERVER_CONNECTIONS], int xlogon); //help
	void prg_passwd(con_v mx[SERVER_CONNECTIONS], int xlogon); //change password

	void prg_prompt(con_v mx[SERVER_CONNECTIONS], int xlogon); //Change prompt
	void prg_mkuser(con_v mx[SERVER_CONNECTIONS], int xlogon); //MAke user
	void prg_rmuser(con_v mx[SERVER_CONNECTIONS], int xlogon); //deletes user

	void prg_net_send(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_receive(con_v mx[SERVER_CONNECTIONS], int xlogon);
	void prg_net_opensocket(con_v mx[SERVER_CONNECTIONS], int xlogon);
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