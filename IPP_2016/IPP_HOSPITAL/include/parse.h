#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>
#include "structure.h"

#define MAX_LINE_LENGTH 100000

// NEW_DISEASE_ENTER_DESCRIPTION name disease description
// NEW_DISEASE_COPY_DESCRIPTION name1 name2
// CHANGE_DESCRIPTION name n disease description
// PRINT_DESCRIPTION name n
// DELETE_PATIENT_DATA name 

bool readParameters(Hospital *hospital, int argc, char  **argv);
void readInput();

#endif