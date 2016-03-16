#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "structure.h"
#include "parse.h"


bool cmdEnterDescription(Hospital *hosp)
{
	char *name = strtok(NULL, DELIMITERS);
	char *description = strtok(NULL, "");

	Patient *patient = findPatient(hosp, name);

	// Patient not found
	if(patient == NULL)
	{
		patient = malloc(sizeof(Patient));
		initPatient(patient, name);
		addPatient(hosp, patient);
	}

	DiseaseDesc *dsc = malloc(sizeof(DiseaseDesc));
	initDisease(dsc, description);
	addDisease(patient, dsc);

	reportStatus(STATUS_SUCCESS);
	return true;
}

bool cmdCopyDescription(Hospital *hosp)
{
	char *nameDst = strtok(NULL, DELIMITERS);
	char *nameSrc = strtok(NULL, DELIMITERS);

	Patient *patDst = findPatient(hosp, nameDst);
	Patient *patSrc = findPatient(hosp, nameSrc);

	// Patient not found!
	if(patDst == NULL || patSrc == NULL)
		return false;

	if(diseaseListEmpty(patSrc))
		return false;

	DiseaseDesc *lastDisease = getLastDisease(patSrc);
	addDisease(patDst, lastDisease);
	newLink(lastDisease);

	reportStatus(STATUS_SUCCESS);
	return true;
}

bool cmdChangeDescription(Hospital *hosp)
{
	char *name = strtok(NULL, DELIMITERS);
	char *id = strtok(NULL, DELIMITERS);
	char *description = strtok(NULL, "");

	Patient *patient = findPatient(hosp, name);

	if(patient == NULL)
		return false;

	Node *nd = getDiseaseNodeId(patient, atoi(id));

	if(nd == NULL)
		return false;

	removeLink( &(nd->disease) );

	DiseaseDesc *dsc = malloc(sizeof(DiseaseDesc));
	initDisease(dsc, description);

	nd->disease = dsc;

	reportStatus(STATUS_SUCCESS);
	return true;
}

bool cmdPrintDescription(Hospital *hosp)
{
	char *name = strtok(NULL, DELIMITERS);
	char *id = strtok(NULL, DELIMITERS);

	Patient *patient = findPatient(hosp, name);

	if(patient == NULL)
		return false;

	DiseaseDesc *dsc = getDiseaseId(patient, atoi(id));
	if(dsc == NULL)
		return false;
			
	printf("%s\n", dsc->text);
	return true;
}

bool cmdDeletePatient(Hospital *hosp)
{
	char *name = strtok(NULL, DELIMITERS);

	Node *patientNode = findPatientNode(hosp, name);

	if(patientNode == NULL)
		return false;

	deletePatient(patientNode->patient);
	deleteNode(patientNode);
	free(patientNode);

	reportStatus(STATUS_SUCCESS);
	return true;
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

void readInput(Hospital *hosp)
{
	char line[MAX_LINE_LENGTH];

	while(fgets(line, MAX_LINE_LENGTH, stdin) != NULL)
	{
		line[strlen(line) - 1] = '\0';

		commandType command = getCommand(line);

		bool success;

		switch(command)
		{
			case ND_ENTER:
				success = cmdEnterDescription(hosp);
				break;

			case ND_COPY:
				success = cmdCopyDescription(hosp);
				break;

			case CHG_DESC:
				success = cmdChangeDescription(hosp);
				break;

			case PRNT_DESC:
				success = cmdPrintDescription(hosp);
				break;

			case DEL_PAT:
				success = cmdDeletePatient(hosp);
				break;
				
			case ERR:
				success = false;
				break;
		}

		if(!success)
			reportStatus(STATUS_FAIL);

	}
}

void reportStatus(const char *status)
{
	printf("%s\n", status);
}