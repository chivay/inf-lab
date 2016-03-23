#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "structure.h"

void newLink(Hospital *hosp, DiseaseDesc *dsc)
{
    // Increase reference counter
    dsc->refs++;
}

void removeLink(Hospital *hosp, DiseaseDesc *dsc)
{
    // Decrease reference counter
    dsc->refs--;

    // If nothing points to description, then free it
    if (dsc->refs == 0)
    {
        hosp->descriptionCounter--;
        free(dsc->text);
        free(dsc);
    }
}

void initHospital(Hospital *hospital)
{
    initList(&(hospital->patients));

    hospital->verbose = false;
    hospital->descriptionCounter = 0;
}

void initPatient(Patient *patient, char *name)
{
    // Initialize list of diseases
    initList(&(patient->diseases));

    // Allocate memory for name (+1 for \0)
    char *dest = malloc(sizeof(char) * strlen(name) + 1);

    strcpy(dest, name);

    patient->name = dest;
}

void initDisease(DiseaseDesc *desc, char *description)
{
    desc->refs = 0;

    // Allocate memory for description (+1 for \0)
    char *dest = malloc(sizeof(char) * strlen(description) + 1);
    strcpy(dest, description);

    desc->text = dest;
}

void deleteHospital(Hospital *hosp)
{
    Node *nd = hosp->patients.first;
    nd = nd->next;

    // Destroy all patients...
    while (nd != NULL)
    {
        deletePatient(hosp, nd->patient);
        nd = nd->next;
    }
    // ..and then list of nodes pointing to them
    deleteList(&(hosp->patients));
}

void deleteDiseases(Hospital *hosp, Patient *patient)
{
    Node *nd = patient->diseases.first;
    nd = nd->next;

    // Unlink patient from all diseases
    while (nd != NULL)
    {
        removeLink(hosp, nd->disease);
        nd = nd->next;
    }

    // Delete list of pointers to diseases
    deleteList(&(patient->diseases));
}

void deletePatient(Hospital *hosp, Patient *patient)
{
    deleteDiseases(hosp, patient);

    free(patient->name);
    free(patient);
}

Patient *findPatient(Hospital *hosp, char *name)
{
    Node *ptr = findPatientNode(hosp, name);
    return (ptr == NULL) ? NULL : ptr->patient;
}

Node *findPatientNode(Hospital *hosp, char *name)
{
    Node *nd = hosp->patients.first;

    // Skip dummy
    nd = nd->next;

    // Find patient with matching name
    while (nd != NULL)
    {
        if (strcmp(name, nd->patient->name) == 0)
            return nd;
        nd = nd->next;
    }

    return NULL;
}

void addPatientToHospital(Hospital *hosp, Patient *pat)
{
    // Create new node
    Node *nd = malloc(sizeof(Node));
    initNode(nd);

    nd->patient = pat;

    // Add new node to list of patients
    addNode(&(hosp->patients), nd);
}

void addDiseaseToPatient(Hospital *hosp, Patient *patient, DiseaseDesc *dis)
{
    // Create new node
    Node *nd = malloc(sizeof(Node));
    initNode(nd);

    nd->disease = dis;
    // Link patient to disease
    newLink(hosp, dis);

    // Add to list of diseases
    addNode(&(patient->diseases), nd);
}

bool diseaseListEmpty(Patient *patient)
{
    return (patient->diseases.first == patient->diseases.last);
}

DiseaseDesc *getLastDisease(Patient *patient)
{
    if (diseaseListEmpty(patient))
        return NULL;
    return patient->diseases.last->disease;
}

Node *getDiseaseNodeId(Patient *patient, int id)
{
    Node *nd = patient->diseases.first;

    // Skip dummy
    nd = nd->next;

    int i = 1;
    // Iterate over list until equal id
    while (nd != NULL && i != id)
    {
        nd = nd->next;
        i++;
    }

    if (id == i)
        return nd; //Sometimes could also return NULL
    else
        return NULL;
}

DiseaseDesc *getDiseaseId(Patient *patient, int id) {
    Node *ptr = getDiseaseNodeId(patient, id);
    return (ptr == NULL) ? NULL : ptr->disease;
}