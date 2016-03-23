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

// Increase reference counter of disease description
void newLink(Hospital *hosp, DiseaseDesc *dsc);

// Decrease reference counter, free memory if possible
void removeLink(Hospital *hosp, DiseaseDesc *dsc);

// Initialize Hospital structure
void initHospital(Hospital *hospital);

// Initialize Patient structure with copy of @name
void initPatient(Patient *patient, char *name);

// Initialize DiseaseDescription with copy of @description
void initDisease(DiseaseDesc *desc, char *description);

// Destroy Hospital and all related structures
void deleteHospital(Hospital *hosp);

// Destroy Patient and all related structures
void deletePatient(Hospital *hosp, Patient *patient);

// Find Patient with name @name
Patient *findPatient(Hospital *hosp, char *name);

// Find Node in list of Patient with name @name
Node *findPatientNode(Hospital *hosp, char *name);

// Add Patient @pat to list of patients in @hosp
void addPatientToHospital(Hospital *hosp, Patient *pat);

// Add Disease @dsc to list of diseases in @patient
void addDiseaseToPatient(Hospital *hosp, Patient *patient, DiseaseDesc *dsc);

// Check if disease list is empty
bool diseaseListEmpty(Patient *patient);

// Unlink patient form diseases and delete list of diseases
void deleteDiseases(Hospital *hosp, Patient *patient);

// Get last disease of Patient @patient
DiseaseDesc *getLastDisease(Patient *patient);

// Get id-th Node in disease list
Node *getDiseaseNodeId(Patient *patient, int id);

// Get DiseaseDescription of id-th node in disease list
DiseaseDesc *getDiseaseId(Patient *patient, int id);

#endif