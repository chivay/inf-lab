#include <stdlib.h>
#include "structure.h"

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

