#include "StdAfx.h"
#include ".\stackmanager.h"


CstackManager::CstackManager(size_t stackSize)
{
	Stack = (char *)malloc(stackSize);
	stackPointer=0;
}

CstackManager::~CstackManager(void)
{

}

int CstackManager::pop(char *data, int maxlenght)
{
	int retrived=maxleght;
	if (maxlenght>=stackPointer)
	{
		retrived=stackPointer;
		stackPointer=0;
	}
	else stackPointer-=maxlenght;
	memcpy(&data,&Stack+stackPointer,retrived);
	return retrived;
}
int CstackManager::push(char *data, int lenght)
{
	if (lenght+stackPointer>=stackLenght) return -1;
	memcpy(&Stack+stackPointer,&data,lenght);
	stackPointer+=lenght;
	return lenght;
}