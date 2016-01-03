#include <stdio.h>
#include <stdlib.h>

struct Node {
  struct Node *U;
  struct Node *D;
  struct Node *L;
  struct Node *R;

  int size;
};

struct Node;
typedef struct Node Node;

void init_node(Node* n)
{
	n->L = n;
	n->R = n;

	n->U = n;
	n->D = n;

	n->size = 0;
}

Node* create_node()
{
	Node *nd = malloc(sizeof(Node));
	init_node(nd);
	return nd;
}

void read(Node *root)
{
	char c;
	do
	{
		int index = 0;
		/* Read lines */
		while( (c = getchar()) != '\n')
		{
			index++;
			if(root->size < index)
			{
				/* Append new header to last element */
				Node *nd = create_node();
				root->L->R = nd;
				nd->L = root->L;
				nd->R = root;
			}
		}
	}
	while(c != EOF);
}
void solve()
{
	
}

void cleanup()
{

}

int main()
{
	Node root;
	init_node(&root);

	read(&root);
	solve(&root);
	return 0; 
}