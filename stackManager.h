#pragma once

class CstackManager
{
public:
	CstackManager(int stackSize);

	int pop(char *data,int lenght, int maxlenght);
	int push(char *data, int lenght);

	~CstackManager(void);
private:
	char *Stack;
	int stackPointer;
	int stackLenght;

};
