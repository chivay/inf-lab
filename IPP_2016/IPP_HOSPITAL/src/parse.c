#include <stdbool.h>
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

void readInput(){}