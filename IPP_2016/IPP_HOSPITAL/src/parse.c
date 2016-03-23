#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "list.h"
#include "structure.h"
#include "parse.h"

commandType getCommand(char *line)
{
	char *command = strtok(line, DELIMITERS);
	commandType result;

	if(strcmp(command, CMD_ND_ENTER) == 0)
		result = ND_ENTER;

	else if(strcmp(command, CMD_ND_COPY) == 0)
		result = ND_COPY;

	else if(strcmp(command, CMD_CHG_DESC) == 0)
		result = CHG_DESC;

	else if(strcmp(command, CMD_PRNT_DESC) == 0)
		result = PRNT_DESC;

	else if(strcmp(command, CMD_DEL_PAT) == 0)
		result = DEL_PAT;

	else
		result = ERR;

	return result;
}

commandType readCommand(char *line, char **arg1, char **arg2, char **arg3)
{
	*arg1 = *arg2 = *arg3 = NULL;
	
	// Return error if nothing to parse
	if( fgets(line, MAX_LINE_LENGTH, stdin) == NULL )
		return ERR;

	// Replace \n with \0
	int lineLength = strlen(line);

	if(line[lineLength - 1] == '\n')
		line[strlen(line) - 1] = '\0';

	commandType command = getCommand(line);

	switch(command)
	{
		case DEL_PAT:
			*arg = 
		case ND_ENTER:
		case ND_COPY:
		case CHG_DESC:
		case PRNT_DESC:
	}
}

bool readParameters(Hospital *hosp, int argc, char  **argv)
{
	if(argc > 2)
		return false;
	if(argc == 2)
	{
		if(strcmp(argv[1], "-v") == 0)
		{
			hosp->verbose = true;
			return true;
		}
		else 
			return false;
	}

	return true;
}

void reportStatus(const char *status)
{
	printf("%s\n", status);
}

void reportDescriptions(int cnt)
{
	fprintf(stderr, "DESCRIPTIONS: %d\n", cnt);
}