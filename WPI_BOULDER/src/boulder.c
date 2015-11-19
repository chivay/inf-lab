#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* MAP DIMENSIONS */
#define MAX_HEIGHT (100 + 1)
#define MAX_WIDTH  (200 + 1)

/* MAP SYMBOLS */
#define M_ROCKFORD '&'
#define M_EMPTY    ' '
#define M_GROUND   '.' 
#define M_ROCK     '#'
#define M_STONE    'O'
#define M_DIAMOND  '$'
#define M_EXIT     'X'

/* MOVEMENT KEYS */
#define DIR_UP    'w'
#define DIR_DOWN  's' 
#define DIR_LEFT  'a'
#define DIR_RIGHT 'd'

typedef struct position {
	int x;
	int y;
} position;

position make_position(int x, int y)
{
	position p;
	p.x = x;
	p.y = y;
	return p;
}

/* LINKED LIST */
typedef struct l_node {
	position val;
	struct l_node *next;
} l_node;

void push_front(l_node ** head, position pos)
{
	l_node *new_head;
	new_head = (l_node *) malloc( sizeof(l_node) );

	new_head->val = pos;
	new_head->next = *head;
	*head = new_head;
}
bool is_empty(l_node *head)
{
	return (head == NULL);
}
/* END LINKED LIST */

/* UTILITY FUNCTIONS */
int max(int a, int b)
{
	return a > b ? a : b;
}

int getline(char *s)
{
	char c;
	int i = 0;
	while( (c = getchar()) != '\n' && c != EOF)
		s[i++]=c;
	s[i] = '\n';
	return i+1;
}
/* END UTILITY FUNCTIONS */


/* GLOBAL VARIABLES */
char map[MAX_HEIGHT][MAX_WIDTH];

int map_height,
	map_width;

l_node *stone_list;
l_node *diamond_list;

bool RUNNING;

position player_position;
/* END GLOBAL VARIABLES */


/*
 * Reads map pattern from STDIN until it encounters
 * an empty line
 */
void load_map(void)
{
	char *s;
	int len;
	map_height = 0;
	map_width  = 0;

	s = (char *) malloc(MAX_WIDTH * sizeof(char));

	while ( ( len = getline(s) ) > 1) /* Dopóki nie wczytamy samego \n */
	{
		int i;
		for (i = 0; i < len; i++)
			map[map_height][i] = s[i];

		map_height++;
		map_width = max(map_width, len);
	}
	free(s);
}

void print_map(void)
{
	int i,j;
	for (i = 0; i < map_height; i++)
	{
		char c;
		j = 0;

		while( (c = map[i][j++]) != '\n' ) putchar(c);
		putchar('\n');
	}
}
bool moving_allowed(position src, char direction)
{
	return false;
}

void move_field(position src, position dst)
{
	map[dst.y][dst.x] = map[src.y][src.x];
	map[src.y][src.x] = M_EMPTY;
}

void move_rockford(char direction)
{
	char next_field;
	position new_pos;
	new_pos = player_position;

	switch(direction)
	{
		case DIR_UP:
			new_pos.y--;
			break;
		case DIR_DOWN:
			new_pos.y++;
			break;
		case DIR_LEFT:
			new_pos.x--;
			break;
		case DIR_RIGHT:
			new_pos.x++;
			break;
	}
	
	next_field = map[new_pos.y][new_pos.x];

	switch(next_field)
	{
		case M_EMPTY:								/* OK TO MOVE */
		case M_GROUND:								/* OK TO MOVE */
			move_field(player_position, new_pos);
			new_pos = player_position;
			break;
		case M_ROCK:								/* DO NOTHING, NOT ALLOWED */
			break;
		case M_STONE:								/* OK, ONLY IF NEXT SPACE IS EMPTY*/
			if (moving_allowed(new_pos, direction))
			{

			}
		case M_DIAMOND:								/* OK, REMOVE DIAMOND */

		case M_EXIT:								/* OK, ONLY IF DIAMOND LIST IS EMPTY*/
			if ( is_empty(diamond_list) )
			{
				move_field(player_position, new_pos);
				RUNNING = false;                    
			}
	}
}

void init(void)
{
	player_position.x = 0;
	player_position.y = 0;
	/*
	 * LOOK FOR DIAMONDS
	 * LOOK FOR STONES
	 * DELETE THEM FROM BACKGROUND AS DYNAMIC OBJECTS
	 * */
}

void update()
{
	/* HANDLE GRAVITY */
	

}

void handle_input()
{
	char input = getchar();
	switch(input)
	{
		case DIR_UP:
		case DIR_DOWN:
		case DIR_LEFT:
		case DIR_RIGHT:
			move_rockford(input);
			break;
		case EOF:			 /* NOTHING TO DO. STOP AND EXIT*/
			RUNNING = false;
	}
}

void start(void)
{
	RUNNING = false;
	while(RUNNING)
	{
		update();
		handle_input();
	}
}

int main(void)
{
	load_map();
	init();
	print_map();
	start();
	return 0;
}
