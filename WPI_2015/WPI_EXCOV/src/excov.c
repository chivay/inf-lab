/**
 * @file excov.c
 * @brief Program solving exact cover problem.
 *
 * Program solves exact cover problem using dancing links 
 * technique as described in a paper by Donald Knuth. 
 *
 * @author Hubert Jasudowicz  
 * @bug No known bugs.
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

/* libc includes */
#include <stdio.h>
#include <stdlib.h>

/** 
 * @brief Structure for representing element in sparse matrix
 * 
 * This structure represents node in sparse matrix as seen in
 * ASCII art above. It stores links to nodes in 4 directions
 * as well as column node, character and header_index for printing solution.
 */
struct Node {
  struct Node *U; /**< Link to node above. */
  struct Node *D; /**< Link to node below. */
  struct Node *L; /**< Link to node on the left. */
  struct Node *R; /**< Link to node on the right. */

  struct Node *C; /**< Link to header node */

  int header_index; /**< Stores index of column node, used only in column nodes */
  char c; /**< Stores character to print in solution, unused in root and column nodes */
};
struct Node;
typedef struct Node Node;

/**
 * @brief Structure for storing partial solutions
 *
 * Structure is passed down into recursion, contains size of solution
 * and array with partial solution on given level of recursion.
 */
struct Solution {
	int size;  /**< Size of solution array */
	char *row; /**< Array of characters to print */
};
typedef struct Solution Solution; 

/**
 * @brief Sets up inital values for new node.
 * 
 * Initializes node passed as parameter, setting links
 * to itself, and zeroes other elements in structure.
 *
 * @param n Node to be initialized.
 * @return Void.
 */
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

 /**
 * @brief Creates new node
 * 
 * Allocates memory for new node, initializes it
 * and returns pointer to new node
 *
 * @return Pointer to newly created node.
 */
Node* create_node()
{
	Node *nd = malloc(sizeof(Node));
	init_node(nd);
	return nd;
}

/**
 * @brief Removes node @p x from list horizontally(L/R)
 * 
 * Unlinks node passed as parameter from linked list.
 *
 * @param x Node to be removed.
 * @return Void.
 */
void remove_node_h(Node *x)
{
	x->R->L = x->L;
	x->L->R = x->R;
}

/**
 * @brief Removes node @p x from list vertically (U/D)
 * 
 * Unlinks node passed as parameter from linked list.
 *
 * @param x Node to be removed.
 * @return Void.
 */
void remove_node_v(Node *x)
{
	x->D->U = x->U;
	x->U->D = x->D;
}

/**
 * @brief Inserts node @x into list horizontally(L/R)
 * 
 * Inserts node @p x back into matrix using Dancing Links.
 *
 * @param x Node to be inserted.
 * @return Void.
 */
void insert_node_h(Node *x)
{
	x->R->L = x;
	x->L->R = x;
}

/**
 * @brief Inserts node @x into list vertically(U/D)
 * 
 * Inserts node @p x back into matrix using Dancing Links.
 *
 * @param x Node to be inserted.
 * @return Void.
 */
void insert_node_v(Node *x)
{
	x->D->U = x;
	x->U->D = x;
}

/**
 * @brief Inserts node @p c horizontally(L/R) between @p a and @p b
 * 
 * Inserts node @p c horizontally(L/R) between @p a and @p b so that
 * @p c is to the right of @p a and @p b is to the right of @c. 
 *
 * @param a Node to be on the left.
 * @param b Node to be on the right.
 * @param c Node to be in the middle.
 * @return Void.
 */
void insert_between_h(Node *a, Node *b, Node *c)
{
	a->R = c;
	b->L = c;

	c->R = b;
	c->L = a;
}

/**
 * @brief Inserts node @p c vertically (U/D) between @p a and @p b
 * 
 * Inserts node @p c vertically (U/D) between @p a and @p b so that
 * @p c is below @p a and @p b is below @p c. 
 *
 * @param a Node to be above.
 * @param b Node to be below.
 * @param c Node to be in the middle.
 * @return Void.
 */
void insert_between_v(Node *a, Node *b, Node *c)
{
	a->D = c;
	b->U = c;

	c->U = a;
	c->D = b;
}

/**
 * @brief Hides column from data matrix
 * 
 * Hides column with node @p n from data matrix, by removing header node
 * and then for each node in column, hiding row containing that node. 
 * The operation is reversible.
 *
 * @param n Node in column being covered
 * @return Void.
 */
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

/**
 * @brief Uncovers column with node @n
 * 
 * For each node in column, restores rows containing that nodes.
 * Then puts header node back into place.
 *
 * @param n Node in column being uncovered
 * @return Void.
 */
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

/**
 * @brief Adds row to partial solution.
 * 
 * Function goes over nodes in row with node @p row
 * and rewrites characters into solution structure. 
 *
 * @param sol Pointer to Solution structure.
 * @param row Pointer to node in row being added.
 * @return Void.
 */
void add_row(Solution *sol, Node *row)
{
	Node *i = row;
	/* Rewrite first element */
	sol->row[ i->C->header_index ] = row->c;

	/* Rewrite other elements*/
	for (i = i->R; i != row; i = i->R)
		sol->row[ i->C->header_index ] = i->c;
}

/**
 * @brief Removes row with @row from solution
 * 
 * Function goes over nodes in row with node @p row
 * and zeroes out places corresponding to indexes of nodes.
 *
 * @param sol Pointer to Solution structure.
 * @param row Pointer to node in row being removed.
 * @return Void.
 */
void remove_row(Solution *sol, Node *row)
{
	Node *i = row;
	/* Remove first element */
	sol->row[ i->C->header_index ] = 0;

	/* Remove other elements */
	for (i = i->R; i != row; i = i->R)
		sol->row[ i->C->header_index ] = 0;
}

/**
 * @brief Prints solution if it is valid
 * 
 * Checks if solution is fully filled and if
 * it is, prints it to STDOUT.
 *
 * @param sol Pointer to Solution structure.
 * @return Void.
 */
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

/**
 * @brief Reads input from STDIN.
 * 
 * Reads input form STDIN and builds data structure while reading.
 * Each line of input represents row in data matrix, only non space characters
 * are stored in memory to save space.
 *
 * @param root Pointer to root node in data structure.
 * @param columns Pointer to integer to store total number of columns.
 * @return Void.
 */
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

/**
 * @brief Solves exact cover problem using X algorithm
 * 
 * Function recursively finds and prints all solutions to exact cover problem
 * using X algorithm described by Donald Knuth.
 *
 * @param root Pointer to root node in data structure.
 * @param columns Pointer to integer to store total number of columns.
 * @return Void.
 */
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

/**
 * @brief Frees all memory
 * 
 * Iterates over all nodes in data structure and 
 * frees all memory allocated by malloc in read function.
 *
 * @param root Pointer to root node in data structure.
 * @return Void.
 */
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