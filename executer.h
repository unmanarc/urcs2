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
#pragma once
typedef struct flabel
{
	short label_id; //must be 1
	char labelname[8];
} s_label;

typedef struct jump
{
	short label_id; //must be 2-6
	char labelname[8];
} s_jump;

typedef struct com
{
	short label_id;
	size_t command_lenght;
} s_com;

typedef struct ret
{
	short label_id; //must be 1
} s_ret;


typedef struct header
{
	char file_info[3]; //must be "UMA"
	short version; // version of executable.
	int page_size; //size of each page.
} s_header;

class Cexecuter
{
public:
	Cexecuter(void);
	~Cexecuter(void);
	int openexec(char *filename);
	void closeexec();
	void gotolabel(char *labelname);

protected:
	int fh;
	s_header head;
	char *page;
};
