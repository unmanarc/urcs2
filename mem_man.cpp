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

#include "stdafx.h"
#include "mem_man.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


Cmm::Cmm()
{ //Constructor

}
Cmm::~Cmm()
{ //destructor
	
}
char *Cmm::getmemdatabynumber(int pos) //get data from memory (from position)
{
	strncpy(rtax,local_mem.mem+local_mem.pos[pos],local_mem.lenght[pos]);
	return rtax;
}
char *Cmm::getmemnamebynumber(int pos) //get name from memory (from position)
{
	return local_mem.name[pos];
}
int Cmm::get_slots() //get Current slots
{
	return MEMORY_SLOTS;
}
int Cmm::putmem(char *name, char *data_to_put, int lenght) //put data into mem
{
	//first re-order memory
	int free=0;
	int slot=-1;

	//searching for existence
	int namepos=search_name(name);
	if (namepos==-1) //not exist, searching for one...
	{
		free=defragmem();
		if (free<lenght) return -1; //not enought memory
		slot=getslotfree();//now need slot.
		if (slot==-1) return -2; //not slots available
	}
	else
	{				//exist, releasing and defragmenting
		local_mem.lenght[namepos]=0; //release this memory
		free=defragmem(); //defragment again (without this memory)
		if (free<lenght) return -1; //not enought memory
		slot=getslotfree();//now need slot.
		if (slot==-1) return -2; //not slots available
	}
	//setting memory.
	strncpy(local_mem.name[slot],name,12);
	local_mem.lenght[slot]=lenght;
	local_mem.pos[slot]=MEMORY_FOR_USER-free;
	strncpy(local_mem.mem+local_mem.pos[slot],data_to_put,lenght);
//	defragmem(); //defrag mem now.
	return 0;
}
char *Cmm::getmem(char *name) //get data from memory
{
	//search the name on memory.
	int namepos=search_name(name);
	if (namepos==-1) return ""; //not founded...
	else
	{
		//founded... putting on 
		strncpy(rtax, local_mem.mem+local_mem.pos[namepos],local_mem.lenght[namepos]);
	}
	return rtax;
	
}
int Cmm::getmemsize(char *name) //get data size from memory
{
	//search the name on memory and get lenght
	int out_data;
	int namepos=search_name(name);
	if (namepos==-1) return -1; //not founded...
	else out_data=local_mem.lenght[namepos];
	return out_data;
}
int Cmm::release_mem()  //deletes mem
{
    memset(&local_mem,0,sizeof(mem_of_user));
	for (int r=0; r<MEMORY_SLOTS; r++) local_mem.lenght[r]=0;
	for (int r=0; r<MEMORY_SLOTS; r++) local_mem.pos[r]=0;
	
	return 0;
}
int Cmm::defragmem() //defrag memory and return freemem.
{
	mem_of_user defragmented; //struct to defragmented.
	
	/*Relasing Defragmented memory...*/
    memset(&defragmented,0,sizeof(mem_of_user));
	for (int r=0; r<MEMORY_SLOTS; r++) defragmented.lenght[r]=0;
	for (int r=0; r<MEMORY_SLOTS; r++) defragmented.pos[r]=0;

	int localpos=0;
	for (int r=0; r<MEMORY_SLOTS; r++)
	{
		//copy all memory 
		if (local_mem.lenght[r]>0) //busy)
		{
			//copy it to new struct of data.
			strncpy(defragmented.name[r],local_mem.name[r],12); //copy names
			defragmented.lenght[r]=local_mem.lenght[r]; //lenght of block of mem
			defragmented.pos[r]=localpos; //setting localpos to actual position
			//copy memory
			strncpy(defragmented.mem+defragmented.pos[r],local_mem.mem+local_mem.pos[r],local_mem.lenght[r]);
			localpos=localpos+local_mem.lenght[r];
		}
	}
	//All data depured on: defragmented. copy it to local_mem
	memcpy(&local_mem,&defragmented,sizeof(mem_of_user));
	return MEMORY_FOR_USER-localpos;
}

int Cmm::getslotfree()
{
	for (int r=0; r<MEMORY_SLOTS; r++)
	{
		if (local_mem.lenght[r]==0)
			return r;
	}
	return -1;
}
int Cmm::search_name(char *name)
{
	for (int r=0; r<MEMORY_SLOTS; r++)
	{
		if (!strncmp(local_mem.name[r],name,12))
			return r;
	}
	return -1;
}
