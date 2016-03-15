#include <stdlib.h>
#include <stdbool.h>
#include "structure.h"


/*
 * LIST FUNCTIONS
 */

void initNode(Node *node)
{
	node->next = NULL;
	node->prev = NULL;
}

void initList(List *lst)
{
	Node *dummy = malloc(sizeof(Node));
	dummy->next = NULL;
	dummy->prev = NULL;

	lst->first = dummy;
	lst->last = dummy;
}

void addNode(List *lst, Node *node)
{
	lst->last->next = node;
	node->prev = lst->last;

	lst->last = node;
}

void deleteNode(Node *node)
{
	node->prev->next = node->next;
	if(node->next != NULL)
		node->next->prev = node->prev;

	free(node);
}

void destroyList(List *lst)
{
	while(lst->first->next != NULL)
		deleteNode(lst->first->next);
	free(lst->first);
	lst->first = NULL;
	lst->last = NULL;
}

/*
 * HOSPITAL FUNCTIONS
 */
 void initHospital(Hospital *hospital)
 {
 	initList( &(hospital->patients) );
 	hospital->verbose = false;
 }
 void initDisease(){}