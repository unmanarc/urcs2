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
#include "compiler.h"
#include "scripting.h"
#include "funcs.h"
#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include <io.h>

Ccompiler::Ccompiler(void)
{
}

Ccompiler::~Ccompiler(void)
{
}

int Ccompiler::compile(char *script_file, char *file, int pagesize)
{
	//run an script...
	Csc scripting;
	s_header header;
	int fp;
	if( (fp = _open(file, _O_WRONLY | _O_CREAT | _O_BINARY )) == -1 ) return -2; //cannot open output file.
	if (!scripting.openscript(script_file)) return -3; //cannot open script file for read
	//interpreting as is.
	Cfns fns;
	char proc_line[COMMAND_LINE];
	strncpy(header.file_info,"UMA",3);
	header.page_size=pagesize;
	header.version=EXEC_VERSION;
	_write(fp,&header,sizeof(header));
	while (1)
	{
		memset(&proc_line,0,COMMAND_LINE);
		strncpy(proc_line,scripting.get_next_line(),COMMAND_LINE);
		
		fns.denter(proc_line);
		
		//vars are replaced on run-time
		BOOL islccmd=0;
		if (proc_line[0]==':')
		{
			islccmd=1; //is label, grab as label...
			// label format:
			// -short: 1
			//   +char [8]: labelname
			s_label label;
			label.label_id=1;
			strncpy(label.labelname,proc_line+1,8);
			//copy structure to temporary file.
			_write(fp,&label,sizeof(label));
		}
		else
		{
			//filter strings to "conventional format"
			fns.filterstring(proc_line);
		}
		if (proc_line[0]==';') islccmd=1; //is comment. do nothing.
		if (fns.cmpfirstword(proc_line,"RET")) //RET returns to line where be called.
		{
			//insert ret instruction...
			// ret format:
			// short: 2
			// ret grab page and position of ret command.
			s_ret retn;
			retn.label_id=2;
			_write(fp,&retn,sizeof(retn));
			islccmd=1;
		}
		if (fns.cmpfirstword(proc_line,"JNZ")) //Jump if not Zero
		{
			//verify if not zero
			// jnz format:
			// -short: 3
			//   +char [8]: labelname
			s_jump jump;
			jump.label_id=3;
			strncpy(jump.labelname,proc_line+4,8);
			//search for label in new instance.
			Csc verify;
			if (!verify.openscript(script_file))
			{
				scripting.close_script();
				_close(fp);
				_close(fp);
				return -3; //cannot open script file for read
			}
			else	verify.close_script();
			if (verify.search_label(proc_line+4)<0)
			{
                verify.close_script();
				scripting.close_script();
				_close(fp);
				_close(fp);
				return -4; //label not founded.
			}
			verify.close_script();
			_write(fp,&jump,sizeof(jump));
			islccmd=1;
		}
		if (fns.cmpfirstword(proc_line,"JZ")) //Jump if Zero
		{
			//verify if not zero
			// jz format:
			// -short: 4
			//   +char [8]: labelname
			s_jump jump;
			jump.label_id=4;
			strncpy(jump.labelname,proc_line+3,8);
			//search for label in new instance.
			Csc verify;
			if (!verify.openscript(script_file))
			{
                verify.close_script();
				scripting.close_script();
				_close(fp);
				_close(fp);
				return -3; //cannot open script file for read
			}
			if (verify.search_label(proc_line+3)<0)
			{
                verify.close_script();
				scripting.close_script();
				_close(fp);
				_close(fp);
				return -4; //label not founded.
			}
			verify.close_script();
			_write(fp,&jump,sizeof(jump));
			islccmd=1;
		}
		if (fns.cmpfirstword(proc_line,"JMP")) //Jump
		{
			//verify if not zero
			// jmp format:
			// -short: 5
			//   +char [8]: labelname
			s_jump jump;
			jump.label_id=5;
			strncpy(jump.labelname,proc_line+4,8);
			//search for label in new instance.
			Csc verify;
			if (!verify.openscript(script_file))
			{
                verify.close_script();
				scripting.close_script();
				_close(fp);
				_close(fp);
				return -3; //cannot open script file for read
			}
			if (verify.search_label(proc_line+4)<0)
			{
                verify.close_script();
				scripting.close_script();
				_close(fp);
				_close(fp);
				return -4; //label not founded.
			}
			verify.close_script();
			_write(fp,&jump,sizeof(jump));
			islccmd=1;
		}
		if (fns.cmpfirstword(proc_line,"CALL")) //Jump
		{
			//verify if not zero
			// call format:
			// -short: 6
			//   +char [8]: labelname
			s_jump jump;
			jump.label_id=6;
			strncpy(jump.labelname,proc_line+5,8);
			//search for label in new instance.
			Csc verify;
			if (!verify.openscript(script_file))
			{
                verify.close_script();
				scripting.close_script();
				_close(fp);
				_close(fp);
				return -3; //cannot open script file for read
			}
			if (verify.search_label(proc_line+5)<0)
			{
                verify.close_script();
				scripting.close_script();
				_close(fp);
				_close(fp);
				return -4; //label not founded.
			}
			verify.close_script();
			_write(fp,&jump,sizeof(jump));
			islccmd=1;
		}
		if (fns.cmpfirstword(proc_line,"NOP")) //No operator
		{
			//nop format:
			// short: 0
			short nop=0;
			_write(fp,&nop,sizeof(nop));
			islccmd=1;
		}
		if (fns.cmpfirstword(proc_line,"EXITSCRIPT")) //No operator
		{
			//exit format:
			// short: 7
			short exitscr=7;
			_write(fp,&exitscr,sizeof(exitscr));
			break;
		}
		if (!islccmd)
		{
			s_com comando;
			comando.label_id=7;// short 7 (execute)
			size_t tmu=strlen(proc_line);
			comando.command_lenght=tmu;
//			_write(fp,&comando,sizeof(size_t)+sizeof(short)+comando.command_lenght);
			_write(fp,&comando,sizeof(comando));
			_write(fp,&proc_line,tmu);
		}
	}
	//open temporal file for convertions....
	_close(fp);
	return 1;
}
