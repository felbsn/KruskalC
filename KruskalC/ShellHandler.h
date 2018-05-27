#pragma once


#include <string.h>
#include "CVector.h"

#ifndef SHELLHANDLER_H
#define SHELLHANDLER_H

typedef struct CVectorString_s
{
	int size;
	int capacity;
	char ** data;
} CVectorString;

struct Command_s
{
	char command[48];
	int(*func)(CVectorString);
};
typedef struct Command_s Command;

typedef struct CVectorCommand_s
{
	int size;
	int capacity;
	Command * data;

} CVectorCommand;





enum StateCodes
{
	exitState,
	runState,
	restartState
};


int CommandIsGreater(Command lhs , Command rhs);
int CommandComparator(char* val ,Command cmd );



void ShellAddCommand(const char* cmdStr ,int (*func)(CVectorString));
void ShellAddCommandLoop(const char* cmdStr ,int (*func)(CVectorString));
void InitInlineShell();
int RunConsoleQuery();



#endif
