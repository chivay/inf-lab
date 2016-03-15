#include <stdio.h>
#include "structure.h"
#include "parse.h"

int main(int argc, char **argv)
{
	Hospital h;
	initHospital(&h);

	if( !readParameters(&h, argc, argv) )
	{
		printf("ERROR\n");
		return 1;
	}
	
	readInput();

	return 0;
}