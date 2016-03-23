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

#define STATUS_FAIL "IGNORED"
#define STATUS_SUCCESS "OK"

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

char* readToWhitespace();
char* readToEndline();

commandType getCommand(char *line);
commandType readCommand(char **arg1, char **arg2, char **arg3);

bool readParameters(Hospital *hosp, int argc, char  **argv);
void readInput(Hospital *hosp);

void reportStatus(const char *status);
void reportDescriptions(int cnt);

#endif