#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <stdbool.h>
#include "list.h"

struct THospital;
struct TPatient;
struct TDiseaseDesc;

typedef struct TDiseaseDesc DiseaseDesc;
typedef struct TPatient Patient;
typedef struct THospital Hospital;


struct TDiseaseDesc
{
	char *text;
	int refs;
};

struct TPatient
{
	char *name;
	List diseases;
};

struct THospital
{
	List patients;
	bool verbose;
	int descriptionCounter;
};

void newLink(Hospital *hosp, DiseaseDesc *dsc);
void removeLink(Hospital *hosp, DiseaseDesc *dsc);

void initHospital(Hospital *hospital);
void initPatient(Patient *patient, char *name);
void initDisease(DiseaseDesc *desc, char *description);
void deleteHospital(Hospital *hosp);
void deletePatient(Hospital *hosp, Patient *patient);

Patient* findPatient(Hospital *hosp, char *name);
Node * findPatientNode(Hospital *hosp, char *name);

void addPatient(Hospital *hosp, Patient *pat);
void addDisease(Hospital *hosp, Patient *patient, DiseaseDesc *dsc);

bool diseaseListEmpty(Patient *patient);
void deleteDiseases(Hospital *hosp, Patient *patient);
DiseaseDesc* getLastDisease(Patient *patient);
Node* getDiseaseNodeId(Patient *patient, int id);
DiseaseDesc* getDiseaseId(Patient *patient, int id);
#endif