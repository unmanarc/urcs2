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

#define MEMORY_SLOTS 128
#define MEMORY_FOR_USER 32768


typedef struct memory
{
	char mem[MEMORY_FOR_USER]; //memory
	char name[MEMORY_SLOTS][12]; //name of this slot
	int pos[MEMORY_SLOTS]; //pointer to part of memory that have the data
	int lenght[MEMORY_SLOTS]; //memory lenght, if 0 memory slot released
} mem_of_user;


class Cmm
{
public:
	Cmm(); 
	~Cmm();
	int putmem(char *name, char *data_to_put, int lenght); //put data into mem
	char *getmem(char *name); //get data from memory
	
	char *getmemdatabynumber(int pos); //get data from memory (from position)
	char *getmemnamebynumber(int pos); //get name from memory (from position)
	int get_slots(); //get Current slots
    
	int getmemsize(char *name); //get data size from memory
	int release_mem(); //delete all data in memory

protected:
	int defragmem(); //defrag memory and return freemem.
	int getslotfree();
	int search_name(char *name);
	mem_of_user local_mem;
	char rtax[MEMORY_FOR_USER];
};