#include <stdio.h>
#include "structure.h"
#include "parse.h"

bool cmdEnterDescription(Hospital *hosp, char *name, char *description)
{
	// char *name = strtok(NULL, DELIMITERS);
	// char *description = strtok(NULL, "");

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

	hosp->descriptionCounter++;
	addDisease(hosp, patient, dsc);

	reportStatus(STATUS_SUCCESS);
	return true;
}

bool cmdCopyDescription(Hospital *hosp, char *nameDst, char *nameSrc)
{
	// char *nameDst = strtok(NULL, DELIMITERS);
	// char *nameSrc = strtok(NULL, DELIMITERS);

	Patient *patDst = findPatient(hosp, nameDst);
	Patient *patSrc = findPatient(hosp, nameSrc);

	// Source patient not found!
	if(patSrc == NULL)
		return false;

	if(diseaseListEmpty(patSrc))
		return false;

	if(patDst == NULL)
	{
		patDst = malloc(sizeof(Patient));
		initPatient(patDst, nameDst);
		addPatient(hosp, patDst);
	}

	DiseaseDesc *lastDisease = getLastDisease(patSrc);
	addDisease(hosp, patDst, lastDisease);
	reportStatus(STATUS_SUCCESS);
	return true;
}

bool cmdChangeDescription(Hospital *hosp, char *name, char *id, char *description)
{
	// char *name = strtok(NULL, DELIMITERS);
	// char *id = strtok(NULL, DELIMITERS);
	// char *description = strtok(NULL, "");

	Patient *patient = findPatient(hosp, name);

	if(patient == NULL)
		return false;

	Node *nd = getDiseaseNodeId(patient, atoi(id));

	if(nd == NULL)
		return false;

	removeLink(hosp, nd->disease);

	DiseaseDesc *dsc = malloc(sizeof(DiseaseDesc));
	initDisease(dsc, description);
	newLink(hosp, dsc);
	hosp->descriptionCounter++;

	nd->disease = dsc;

	reportStatus(STATUS_SUCCESS);
	return true;
}

bool cmdPrintDescription(Hospital *hosp, char *name, char *id)
{
	// char *name = strtok(NULL, DELIMITERS);
	// char *id = strtok(NULL, DELIMITERS);

	Patient *patient = findPatient(hosp, name);

	if(patient == NULL)
		return false;

	DiseaseDesc *dsc = getDiseaseId(patient, atoi(id));
	if(dsc == NULL)
		return false;
			
	printf("%s\n", dsc->text);
	return true;
}

bool cmdDeletePatient(Hospital *hosp, char *name)
{
	// char *name = strtok(NULL, DELIMITERS);

	Node *patientNode = findPatientNode(hosp, name);

	if(patientNode == NULL)
		return false;

	deleteDiseases(hosp, patientNode->patient);
	initList( &(patientNode->patient->diseases));

	reportStatus(STATUS_SUCCESS);
	return true;
}

void readInput(Hospital *hospital)
{
	char line[MAX_LINE_LENGTH];
	char *arg1, *arg2, *arg3;
	commandType command;
	while( (commandType = readCommand(&line, &arg1, &arg2, &arg3)) !=  ERR)
	{
		bool success =  false;
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

		if(hosp->verbose)
			reportDescriptions(hosp->descriptionCounter);
	}	
}

int main(int argc, char **argv)
{
	Hospital h;
	initHospital(&h);

	if( !readParameters(&h, argc, argv) )
	{
		printf("ERROR\n");
		return 1;
	}
	readInput(&h);

	deleteHospital(&h);
	return 0;
}