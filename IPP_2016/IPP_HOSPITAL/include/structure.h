#ifndef STRUCTURE_H
#define STRUCTURE_H


struct TPatient;
struct TDiseaseDesc;
struct TNode;
struct TList;

typedef struct TNode Node;
typedef struct TPatient Patient;
typedef struct TDiseaseDesc DiseaseDesc;
typedef struct TList List;

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



struct Patient
{
	List diseases;
};

struct Hospital
{
	List patients;	
};

#endif