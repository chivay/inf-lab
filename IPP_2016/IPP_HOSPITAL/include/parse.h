#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>
#include "structure.h"

#define MAX_LINE_LENGTH 100000
#define DELIMITERS " \t"

#define CMD_ND_ENTER  "NEW_DISEASE_ENTER_DESCRIPTION"
#define CMD_ND_COPY   "NEW_DISEASE_COPY_DESCRIPTION"
#define CMD_CHG_DESC  "CHANGE_DESCRIPTION"
#define CMD_PRNT_DESC "PRINT_DESCRIPTION"
#define CMD_DEL_PAT   "DELETE_PATIENT_DATA"

enum COMMAND_TYPE
{ 
	ND_ENTER,  // NEW_DISEASE_ENTER_DESCRIPTION name disease description
	ND_COPY,   // NEW_DISEASE_COPY_DESCRIPTION name1 name2
	CHG_DESC,  // CHANGE_DESCRIPTION name n disease description
	PRNT_DESC, // PRINT_DESCRIPTION name n
	DEL_PAT,   // DELETE_PATIENT_DATA name 
	ERR
};

typedef enum COMMAND_TYPE commandType;

bool readParameters(Hospital *hospital, int argc, char  **argv);
void readInput(Hospital *hospital);

#endif