#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"

bool readParameters(Hospital *hospital, int argc, char  **argv)
{
	if(argc > 2)
		return false;
	if(argc == 2)
	{
		if(strcmp(argv[1], "-v") == 0)
		{
			hospital->verbose = true;
			return true;
		}
		else 
			return false;
	}

	return true;
}

comandType getCommand(char *line)
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

void readInput(Hospital *hospital)
{
	char line[MAX_LINE_LENGTH];

	while(fgets(line, MAX_LINE_LENGTH, stdin) != NULL)
	{
		line[strlen(line) - 1] = '\0';

		commandType command = getCommand(line);

		switch(command)
		{
			case ND_ENTER:
				break;

			case ND_COPY:
				break;

			case CHG_DESC:
				break;

			case PRNT_DESC:
				break;

			case DEL_PAT:
				break;
				
			case ERR:
				break;
		}		

	}
}