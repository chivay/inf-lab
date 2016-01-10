/*
 * EXACT COVER PROBLEM
 * PROBLEM DOKŁADNEGO POKRYCIA
 *
 *  Author: Hubert Jasudowicz
 * Version: 1.0
 */

 /* Visual representation of data structure in memory:
  *
  *  [root] -- [head1] -- [head2] -- [head3] -- .. -> [points to root]
  *               |          |          |
  *             [ a ]  --  [ b ]  -- [ c ]
  *               |          |          |
  *               |        [ d ]        |
  *             [ e ]  -------------  [ f ]
  *               |          |          |
  *              ...        ...        ...
  */

#include <stdio.h>
#include <stdlib.h>

/* Structure for representing sparse matrix in memory
 * using double-linked lists in 2 dimensions
 */
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

/* Solution structure containing size of solution
 * and array with solution
 */
struct Solution {
	int size;
	char *row;
};

typedef struct Solution Solution; 

/* Sets up inital value for new node @n */
void init_node(Node* n)
{
	/* All pointers point to node itself */
	n->L = n;
	n->R = n;
	n->U = n;
	n->D = n;

	/* Header is NULL */
	n->C = NULL;

	n->header_index = -1;
	n->c = 0;
}


/* Allocates memory for new node, initializes it
 * and returns pointer to new node
 */
Node* create_node()
{
	Node *nd = malloc(sizeof(Node));
	init_node(nd);
	return nd;
}

/* Removes node @x from list horizontally(L/R)*/
void remove_node_h(Node *x)
{
	x->R->L = x->L;
	x->L->R = x->R;
}

/* Removes node @x from list vertically (U/D) */
void remove_node_v(Node *x)
{
	x->D->U = x->U;
	x->U->D = x->D;
}

/* Inserts node @x into list horizontally(L/R)
 * using Dancing Links technique
 */
void insert_node_h(Node *x)
{
	x->R->L = x;
	x->L->R = x;
}

/* Inserts node @x into list vertically(U/D)
 * using Dancing Links technique
 */
void insert_node_v(Node *x)
{
	x->D->U = x;
	x->U->D = x;
}

/* Inserts node @c horizontally(L/R) between @a and @b */
void insert_between_h(Node *a, Node *b, Node *c)
{
	a->R = c;
	b->L = c;

	c->R = b;
	c->L = a;
}

/* Inserts node @c vertically(U/D) between @a and @b */
void insert_between_v(Node *a, Node *b, Node *c)
{
	a->D = c;
	b->U = c;

	c->U = a;
	c->D = b;
}

/* Removes column with node @n from data matrix */
void cover_column(Node *n)
{
	Node *i;
	Node *header = n->C;

	/* Remove header node*/
	remove_node_h(header);

	/* Iterate over column */
	for (i = header->D; i != header; i = i->D) {
		Node *j;
		/*Iterate over rows */
		for (j = i->R; j != i; j = j->R)
			remove_node_v(j);
	}

}

/* Puts column with node @n back into matrix */
void uncover_column(Node *n)
{
	Node *i;
	Node *header = n->C;

	/* Iterate over column */
	for (i = header->U; i != header; i = i->U) {
		Node *j;
		/*Iterate over rows */
		for (j = i->L; j != i; j = j->L)
			insert_node_v(j);
	}

	/* Put header back into place*/
	insert_node_h(header);
}

/* Add row with @row to partial solution */
void add_row(Solution *sol, Node *row)
{
	Node *i = row;
	/* Rewrite first element */
	sol->row[ i->C->header_index ] = row->c;

	/* Rewrite other elements*/
	for (i = i->R; i != row; i = i->R)
		sol->row[ i->C->header_index ] = i->c;
}

/* Remove row with @row from solution */
void remove_row(Solution *sol, Node *row)
{
	Node *i = row;
	/* Remove first element */
	sol->row[ i->C->header_index ] = 0;

	/* Remove other elements */
	for (i = i->R; i != row; i = i->R)
		sol->row[ i->C->header_index ] = 0;
}

/* Prints solution from @sol if it is valid */
void print_solution(Solution *sol)
{
	/* Return if solution is incomplete */
	int i;
	for (i = 0; i < sol->size; i++)
		if (sol->row[i] == 0)
			return;

	/* Otherwise print it */
	for (i = 0; i < sol->size; i++)
		putchar(sol->row[i]);
	putchar('\n');
}

void read(Node *root, int *columns)
{
	char c;
	Node *current_col = root;
	Node *prev_in_row = NULL;

	/* Read until end of file */
	while ( (c = getchar()) != EOF) {

		/* New line encountered */
		if (c == '\n') {
			current_col = root;
			prev_in_row = NULL;
		}
		else {
			/* If column size is too small */
			if (current_col->R == root) {
				/* Append new header to header list */
				Node *nd = create_node();
				insert_between_h(root->L, root, nd);

				nd->header_index = nd->L->header_index + 1;
				nd->C = nd;
			}

			current_col = current_col->R;
		
			/* If element is present in subset */
			if (c != ' ') { 
				Node *nd = create_node();
				nd->c = c;
				nd->C = current_col;
				insert_between_v(current_col->U, current_col, nd);

				/* If first element in row*/
				if (prev_in_row == NULL)
					prev_in_row = nd;
				else
				{
					insert_between_h(prev_in_row,prev_in_row->R,nd);
					prev_in_row = nd;
				}
			}	
		} 
	}
	/* Store total number of columns in @columns */
	*columns = root->L->header_index + 1;
}

/* Solves exact cover problem using X algorithm */
void solve(Node *root, Solution *sol)
{
	Node *r;
	/* Choose column */
	Node *c = root->R;

	/* No columns left */
	if(root->R == root) {
		print_solution(sol);
		return;
	}

	/* Hide chosen column from matrix*/
	cover_column(c);

	/* Iterate over rows in column*/
	for(r = c->D; r != c ; r = r->D) {
		Node *j;

		/* Add row with @r to solution */
		add_row(sol, r);

		/* Hide columns colliding with chosen row */
		for(j = r->R; j != r; j = j->R)
			cover_column(j);

		/* Recursively choose rest of columns */
		solve(root, sol);
		
		/* Remove row with @r from solution set */
		remove_row(sol, r);

		/* Restore removed columns */
		for(j = r->L; j != r; j = j->L)
			uncover_column(j);
	}
	/* Restore chosen column */
	uncover_column(c);
}


/* Frees all memory allocated in read function */
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

	/* Initialize root node */
	init_node(&root);

	/* Read data from input */
	read(&root, &(sol.size) );

	/* Allocate memory for solutions */
	sol.row = calloc(sol.size, sizeof(char));

	/* Compute solutions */
	solve(&root, &sol);

	/* Free data structure memory*/
	cleanup(&root);

	/* Free solution array */
	free(sol.row);
	return 0; 
}