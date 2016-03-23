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

    union
    {
        struct TPatient *patient;
        struct TDiseaseDesc *disease;
    };
};

struct TList
{
    Node *first;
    Node *last;
};


// Initialize Node by setting all pointers to NULL
void initNode(Node *node);

// Initialize list and create dummy node
void initList(List *lst);

// Add node @node to list @lst
void addNode(List *lst, Node *node);

// Delete @node from list
void deleteNode(Node *node);

// Delete all nodes in list @lst and reset pointers to NULL
void deleteList(List *lst);

#endif