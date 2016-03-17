#include <stdlib.h>
#include "list.h"

void initNode(Node *node)
{
	node->next = NULL;
	node->prev = NULL;

	node->patient = NULL;
	node->disease = NULL;
}

void initList(List *lst)
{
	Node *dummy = malloc(sizeof(Node));
	initNode(dummy);

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

void deleteList(List *lst)
{
	while(lst->first->next != NULL)
		deleteNode(lst->first->next);
	free(lst->first);
	lst->first = NULL;
	lst->last = NULL;
}