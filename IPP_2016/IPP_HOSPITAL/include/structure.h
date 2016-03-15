#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdbool.h>


struct TNode;
struct TList;

struct THospital;
struct TPatient;
struct TDiseaseDesc;

typedef struct TNode Node;
typedef struct TList List;

typedef struct TDiseaseDesc DiseaseDesc;
typedef struct TPatient Patient;
typedef struct THospital Hospital;

struct TNode
{
	Node *next;
	Node *prev;

	union {
		Patient *patient;
		DiseaseDesc *disease;
	};
};

struct TList 
{
	Node *first;
	Node *last;
};

void initNode(Node *node);
void initList(List *lst);

void addNode(List *lst, Node *node);
void deleteNode(Node *node);
void destroyList(List *lst);


struct TDiseaseDesc
{
	const char *text;
	int refs;
};

struct TPatient
{
	const char *name;
	List diseases;
};

struct THospital
{
	List patients;

	bool verbose;
};

void initHospital(Hospital *hospital);

Patient* findPatient(Hospital *hosp, char *name);

#endif