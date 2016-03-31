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

// Read next token to first whitespace
char *readToWhitespace();

// Read next token until \n
char *readToEndline();

// Initialize strtok and get type of command
commandType getCommand(char *line);

// Read line of STDIN and parse command and arguments
commandType readCommand(char **arg1, char **arg2, char **arg3);

// Parse arguments used in running the executable
bool readParameters(Hospital *hosp, int argc, char **argv);

// Print status of command
void reportStatus(const char *status);

// Print number of descriptions in memory
void reportDescriptions(int cnt);

#endif