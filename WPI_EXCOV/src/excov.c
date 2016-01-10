#include <stdio.h>
#include <stdlib.h>

struct Node {
  struct Node *U;
  struct Node *D;
  struct Node *L;
  struct Node *R;

  /* Header node */
  struct Node *C;

  int header_index;
  char c;
};

struct Node;
typedef struct Node Node;

struct Solution {
	int size;
	char *row;
};

typedef struct Solution Solution; 

void init_node(Node* n)
{
	n->L = n;
	n->R = n;

	n->U = n;
	n->D = n;

	n->C = NULL;

	n->header_index = -1;
	n->c = 0;
}

Node* create_node()
{
	Node *nd = malloc(sizeof(Node));
	init_node(nd);
	return nd;
}

void remove_node_h(Node *x)
{
	x->R->L = x->L;
	x->L->R = x->R;
}

void remove_node_v(Node *x)
{
	x->D->U = x->U;
	x->U->D = x->D;
}

void insert_node_h(Node *x)
{
	x->R->L = x;
	x->L->R = x;
}

void insert_node_v(Node *x)
{
	x->D->U = x;
	x->U->D = x;
}

/* Insert node @c between @a and @b */
void insert_between_h(Node *a, Node *b, Node *c)
{
	a->R = c;
	b->L = c;

	c->R = a;
	c->L = b;
}

void insert_between_v(Node *a, Node *b, Node *c)
{
	a->D = c;
	b->U = c;

	c->U = a;
	c->D = b;
}

void cover_column(Node *n)
{
	Node *i;
	Node *header = n->C;
	remove_node_h(header);

	for(i = header->D; i != header; i = i->D) {
		remove_node_v(i);
	}

}

void uncover_column(Node *n)
{
	Node *i;
	Node *header = n->C;

	for(i = header->U; i != header; i = i->U) {
		insert_node_v(i);
	}

	insert_node_h(header);
}

void add_row(Solution *sol, Node *row)
{
	Node *i = row;
	/* Rewrite first element */
	sol->row[ i->C->header_index ] = row->c;

	for(i = i->R; i != row; i = i->R)
		sol->row[ i->C->header_index ] = i->c;
}

void remove_row(Solution *sol, Node *row)
{
	Node *i = row;
	/* Remove first element */
	sol->row[ i->C->header_index ] = 0;

	for(i = i->R; i != row; i = i->R)
		sol->row[ i->C->header_index ] = 0;
}

void print_solution(Solution *sol)
{
	/* Return if solution is incomplete */
	int i;
	for (i = 0; i < sol->size; i++)
		if(sol->row[i] == 0)
			return;
	for (i = 0; i < sol->size; i++)
		putchar(sol->row[i]);
	putchar('\n');
}

void read(Node *root, int *columns)
{
	char c;
	Node *current_col = root;
	Node *prev_in_row = NULL;

	while( (c = getchar()) != EOF) {
		/* New line encountered */
		if(c == '\n') {
			current_col = root;
			prev_in_row = NULL;
		}
		else {
			/* If column size is too small */
			if(current_col->R == root) {
				/* Append new header to header list */
				Node *nd = create_node();

				insert_between_h(root->L, root, nd);
				nd->header_index = nd->L->header_index + 1;
			}
			current_col = current_col->R;
			
			/* If element is present in subset */
			if(c != ' ') { 
				Node *nd = create_node();
				nd->c = c;
				nd->C = current_col;
				insert_between_v(current_col->U, current_col, nd);

				/* First element in row*/
				if(prev_in_row == NULL)
					prev_in_row = nd;
				else
				{
					insert_between_h(prev_in_row,prev_in_row->R,nd);
					prev_in_row = nd;
				}
			}	
		} 
	}
	*columns = root->L->header_index + 1;
}

void solve(Node *root, Solution *sol)
{
	Node *r;
	Node *c = root->R;

	/* No columns left */
	if(root->R == root) {
		print_solution(sol);
		return;
	}
	cover_column(c);
	for(r = c->D; r != c ; r = r->D) {
		Node *j;

		/* Add row with r to solution */
		add_row(sol, r);

		for(j = r->R; j != r; j = j->R)
			cover_column(j);
		solve(root, sol);
		
		/* Remove r from solution set */
		remove_row(sol, r);

		for(j = r->L; j != r; j = j->L)
			uncover_column(j);
	}
	uncover_column(c);
}

void cleanup(Node *root)
{
	Node *i;

	/* Iterate over columns */
	for(i = root->R; i != root;) {
		Node *j, *next_column;
		next_column = i->R;
		/* Iterate over rows in column */
		for(j = i->D; j != i;)
		{
			Node *next_row = j->D;
			free(j);
			j = next_row;
		}
		free(i);
		i = next_column;
	}
}

int main()
{
	Node root;
	Solution sol;

	init_node(&root);
	read(&root, &(sol.size) );

	sol.row = calloc(sol.size, sizeof(char));

	solve(&root, &sol);
	cleanup(&root);
	free(sol.row);
	return 0; 
}