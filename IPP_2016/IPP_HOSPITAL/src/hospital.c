#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "structure.h"
#include "parse.h"

bool cmdEnterDescription(Hospital *hosp, char *name, char *description)
{
	Patient *patient = findPatient(hosp, name);

	// Patient not found
	if (patient == NULL)
	{
		// Create new patient
		patient = malloc(sizeof(Patient));
		initPatient(patient, name);
		addPatientToHospital(hosp, patient);
	}

	// Create new disease desctiption
	DiseaseDesc *dsc = malloc(sizeof(DiseaseDesc));
	initDisease(dsc, description);
	hosp->descriptionCounter++;

	// Add disease to patient
	addDiseaseToPatient(hosp, patient, dsc);

	reportStatus(STATUS_SUCCESS);
	return true;
}

bool cmdCopyDescription(Hospital *hosp, char *nameDst, char *nameSrc)
{
	Patient *patDst = findPatient(hosp, nameDst);
	Patient *patSrc = findPatient(hosp, nameSrc);

	// Source patient not found
	if (patSrc == NULL)
		return false;

	if (diseaseListEmpty(patSrc))
		return false;

	// Target patient not found
	if (patDst == NULL)
	{
		// Create patient
		patDst = malloc(sizeof(Patient));
		initPatient(patDst, nameDst);
		addPatientToHospital(hosp, patDst);
	}

	addDiseaseToPatient(hosp, patDst, getLastDisease(patSrc));

	reportStatus(STATUS_SUCCESS);
	return true;
}

bool cmdChangeDescription(Hospital *hosp, char *name, char *id, char *description)
{
	Patient *patient = findPatient(hosp, name);

	// Patient not found
	if (patient == NULL)
		return false;

	// Get node on patient's disease list
	Node *nd = getDiseaseNodeId(patient, atoi(id));

	// No disease with index @id
	if (nd == NULL)
		return false;

	// Unlink patient from disease
	removeLink(hosp, nd->disease);

	// Create new disease desctiption
	DiseaseDesc *dsc = malloc(sizeof(DiseaseDesc));
	initDisease(dsc, description);

	newLink(hosp, dsc);
	hosp->descriptionCounter++;

	// Update disease at index @id
	nd->disease = dsc;

	reportStatus(STATUS_SUCCESS);
	return true;
}

bool cmdPrintDescription(Hospital *hosp, char *name, char *id)
{
	Patient *patient = findPatient(hosp, name);

	// Patient not found
	if (patient == NULL)
		return false;

	DiseaseDesc *dsc = getDiseaseId(patient, atoi(id));

	// Disease not found
	if (dsc == NULL)
		return false;
			
	printf("%s\n", dsc->text);

	return true;
}

bool cmdDeletePatient(Hospital *hosp, char *name)
{
	Node *patientNode = findPatientNode(hosp, name);

	// Patient not found
	if (patientNode == NULL)
		return false;

	// Delete disease list
	deleteDiseases(hosp, patientNode->patient);

	// Reinitialize disease list
	initList( &(patientNode->patient->diseases));

	reportStatus(STATUS_SUCCESS);
	return true;
}

void readInput(Hospital *hospital)
{
	char *arg1, *arg2, *arg3;
	commandType command;

	// Read line from STDIN until EOF
	while ( (command = readCommand(&arg1, &arg2, &arg3)) !=  ERR)
	{
		bool success = false;

		switch (command)
		{
			case ND_ENTER:
				success = cmdEnterDescription(hospital, arg1, arg2);
				break;
			case ND_COPY:
				success = cmdCopyDescription(hospital, arg1, arg2);
				break;
			case CHG_DESC:
				success = cmdChangeDescription(hospital, arg1, arg2, arg3);
				break;
			case PRNT_DESC:
				success = cmdPrintDescription(hospital, arg1, arg2);
				break;
			case DEL_PAT:
				success = cmdDeletePatient(hospital, arg1);
				break;
			case ERR:
				success = false;
				break;
		}

		if (!success)
			reportStatus(STATUS_FAIL);

		if (hospital->verbose)
			reportDescriptions(hospital->descriptionCounter);
	}	
}

int main(int argc, char **argv)
{
	// Create and initialize data structure
	Hospital hosp;
	initHospital(&hosp);

	// Check if running in verbose mode
	if ( !readParameters(&hosp, argc, argv) )
	{
		printf("ERROR\n");
		return 1;
	}

	// Read lines from STDIN
	readInput(&hosp);

	deleteHospital(&hosp);
	
	return 0;
}