#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "structure.h"

/*
 * HOSPITAL FUNCTIONS
 */

void newLink(DiseaseDesc *dsc)
{
	dsc->refs++;
}

void removeLink(DiseaseDesc **dsc)
{
	(*dsc)->refs--;

	if( (*dsc)->refs == 0)
	{
		free((*dsc)->text);
		free(*dsc);
		*dsc = NULL;
	}
}

void initHospital(Hospital *hospital)
{
	initList( &(hospital->patients) );
	hospital->verbose = false;
}

void initPatient(Patient *patient, char *name)
{
	initList( &(patient->diseases) );

 	int len = strlen(name);
 	char *dest = malloc(sizeof(char) * len + 1);
 	strcpy(dest, name);

 	patient->name = dest;
}

void initDisease(DiseaseDesc *desc, char *description)
{
 	desc->refs = 0;

 	int len = strlen(description);
 	char *dest = malloc(sizeof(char) * len + 1);
 	strcpy(dest, description);

 	desc->text = dest;
}

void deleteHospital(Hospital *hosp)
{
 	Node *nd = hosp->patients.first;
 	nd = nd->next;
 	while(nd != NULL)
 	{
 		deletePatient(nd->patient);
 		nd = nd->next;
 	}
 	deleteList( &(hosp->patients) );
}

void deletePatient(Patient *patient)
{
	Node *nd = patient->diseases.first;
	nd = nd->next;
	while(nd != NULL)
	{
		removeLink( &(nd->disease) );
		nd = nd->next;
	}

	deleteList( &(patient->diseases) );
	free(patient->name);
}

Patient* findPatient(Hospital *hosp, char *name)
{
	Node *ptr = findPatientNode(hosp, name);
	return (ptr == NULL) ? NULL : ptr->patient ;
}

Node* findPatientNode(Hospital *hosp, char *name)
{
 	Node *nd = hosp->patients.first;

 	// Skip dummy
 	nd = nd->next;

 	while(nd != NULL)
 	{
 		if( strcmp(name, nd->patient->name) == 0)
 			return nd;

 		nd = nd->next;
 	}

 	return NULL;	
}

void addPatient(Hospital *hosp, Patient *pat)
{
 	Node *nd = malloc(sizeof(Node));
 	initNode(nd);

 	nd->patient = pat;
 	addNode( &(hosp->patients), nd);
}

void addDisease(Patient *patient, DiseaseDesc *dis)
{
 	Node *nd = malloc(sizeof(Node));
 	initNode(nd);

 	nd->disease = dis;
 	newLink(dis);
 	addNode( &(patient->diseases), nd);
}

bool diseaseListEmpty(Patient *patient)
{
	return (patient->diseases.first == patient->diseases.last);
}

DiseaseDesc* getLastDisease(Patient *patient)
{
	return patient->diseases.last->disease;
}

Node* getDiseaseNodeId(Patient *patient, int id)
{
	// Dummy
	Node *nd = patient->diseases.first;

	nd = nd->next;
	int i = 1;

	while(nd != NULL && i != id)
	{
		nd = nd->next;
		i++;
	}

	if(id == i)
		return nd; //sometimes could also return NULL
	else
		return NULL;
}

DiseaseDesc* getDiseaseId(Patient *patient, int id)
{
	Node *ptr = getDiseaseNodeId(patient, id);
	return (ptr == NULL) ? NULL : ptr->disease;
}