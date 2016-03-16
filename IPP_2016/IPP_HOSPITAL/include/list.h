#ifndef LIST_H
#define LIST_H

struct TPatient;
struct TDiseaseDesc;

typedef struct TNode Node;
typedef struct TList List;

struct TNode
{
	Node *next;
	Node *prev;

	union {
		struct TPatient *patient;
		struct TDiseaseDesc *disease;
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
void deleteList(List *lst);

#endif