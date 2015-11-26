#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef TUI
	#include <ncurses.h>
#endif


/* MAP DIMENSIONS */
#define MAX_HEIGHT (100 + 1)
#define MAX_WIDTH  (200 + 1)

/* MAP SYMBOLS */
#define M_ROCKFORD '&'
#define M_EMPTY    ' '
#define M_DIRT     '.' 
#define M_ROCK     '#'
#define M_STONE    'O'
#define M_DIAMOND  '$'
#define M_EXIT     'X'

/* MOVEMENT KEYS */
#define DIR_UP    'w'
#define DIR_DOWN  's' 
#define DIR_LEFT  'a'
#define DIR_RIGHT 'd'


/* Map row */
typedef char map_t[MAX_WIDTH];

/* Simple structure to define coordinates on map */
typedef struct {
	int x;
	int y;
} position;

/* Structure containing game state */
typedef struct {

	/* Pointer to map */
	map_t *map;

	/* Map dimensions*/
	int map_height;
	int map_width;

	/* Flag to terminate execution*/
	bool running;

	/* Rockford's position (current and from previous iteration) */
	position previous_position;
	position player_position;

	/* Number of not collected diamonds */
	int diamond_counter;

	/* Terminal dimension (used only in TUI mode) */
	int term_rows;
	int term_cols;

} game_state;



/* UTILITY FUNCTIONS */

/* Returns maximum of greater value */
int max(int a, int b)
{
	return a > b ? a : b;
}

/* Reads line from STDIN into char* until end of line
 * Returns number of characters read
 */
int read_line(char *s)
{
	char c;
	int i = 0;
	while( (c = getchar()) != '\n' && c != EOF && i < MAX_WIDTH)
		s[i++] = c;
	s[i] = '\n';
	return i+1;
}

/* Computes new coordinates for  @direction of movement
 * and stores result in @dst
 */
void  calc_position(position *pos, position *dst, char direction)
{
	*dst = *pos;

	switch(direction)
	{
		case DIR_UP:
			dst->y--;
			break;

		case DIR_DOWN:
			dst->y++;
			break;

		case DIR_LEFT:
			dst->x--;
			break;

		case DIR_RIGHT:
			dst->x++;
			break;
	}
}
/* END UTILITY FUNCTIONS */


/* Reads map data from STDIN until
 * an empty line is encountered
 */
void load_map(game_state *g)
{
	char *s;

	int len;

	/* Reset map dimensions */
	g->map_height = 0;
	g->map_width  = 0;

	/* Allocate memory for line to read */
	s = (char *) malloc(MAX_WIDTH * sizeof(char));

	/* Read until only \n is read */
	while ( ( len = read_line(s) ) > 1)
	{
		int i;

		/* Rewrite line to map in memory */
		for (i = 0; i < len; i++)
			g->map[g->map_height][i] = s[i];

		/* Update map dimensions */
		g->map_height++;
		g->map_width = max(g->map_width, len);
	}

	/* Free allocated memory */
	free(s);
}

/* Prints map to STDOUT  */
void print_map(game_state *g)
{
	char c;
	int i,j;
	for (i = 0; i < g->map_height; i++)
	{
		#ifdef TUI
			move(i+1, 0);
		#endif 
		j = 0;

		while( (c = g->map[i][j++]) != '\n' ){
			#ifdef TUI
				addch(c);
			#else
				putchar(c);
			#endif 
		}

		#ifndef TUI
			putchar('\n');
		#endif 
	}
}


/* Checks if it is possible to push a stone
 * from position @stone_pos in direction @direction
 */
bool movement_allowed(game_state *g, position *stone_pos, char direction)
{
	position dst = *stone_pos;
	switch(direction)
	{
		/* Pushing stone up or down not allowed */		
		case DIR_UP:
		case DIR_DOWN:
			return false;    
			break;

		case DIR_LEFT:
			dst.x--;
			break;

		case DIR_RIGHT:
			dst.x++;
			break;

	}
	/* If destination field is empty then, movement allowed */
	return ( g->map[dst.y][dst.x] == M_EMPTY);
}

/* Moves source field to target field, marking source as empty */
void move_field(game_state *g, position *src, char direction)
{
	position dst;
	calc_position(src, &dst, direction);

	/* Move field in given direction */
	g->map[dst.y][dst.x] = g->map[src->y][src->x];

	/* Mark source field as empty */
	g->map[src->y][src->x] = M_EMPTY;
}

/* Handles movement into field containing dirt or empty */
void movement_dirt(game_state *g, position *new_pos, char direction)
{
	move_field(g, &(g->player_position), direction);
	g->player_position = *new_pos;
}

/* Handles movement into field containing stone */
void movement_stone(game_state *g, position *new_pos, char direction)
{
	if (movement_allowed(g, new_pos, direction))
	{
		move_field(g, new_pos, direction);
		move_field(g, &(g->player_position), direction);
		g->player_position = *new_pos;

	}
}
/* Handles movement into field containing diamond */
void movement_diamond(game_state *g, position *new_pos, char direction)
{
	g->diamond_counter--;

	move_field(g, &(g->player_position), direction);
	g->player_position = *new_pos;
}

/* Handles movement into field containing exit */
void movement_exit(game_state *g, position *new_pos, char direction)
{
	/* If there are no diamonds left */
	if (g->diamond_counter == 0)
	{
		move_field(g, &(g->player_position), direction);

		/* Rockford disappears, X on map stays */
		g->map[new_pos->y][new_pos->x] = M_EXIT;
		/* End of execution*/
		g->running = false;                    
	}
}

/* Handles Rockford's movement based on chosen direction */
void move_rockford(game_state *g, char direction)
{
	position new_pos;

	/* Compute new position */
	calc_position( &g->player_position, &new_pos, direction);

	/* Determine what to do based on target field type */
	switch( g->map[new_pos.y][new_pos.x] )
	{
		case M_EMPTY:								/* OK TO MOVE */
		case M_DIRT:								/* OK TO MOVE */
			movement_dirt(g, &new_pos, direction);
			break;

		case M_STONE:								/* OK, ONLY IF NEXT SPACE IS EMPTY*/
			movement_stone(g, &new_pos, direction);
			break;

		case M_DIAMOND:								/* OK, REMOVE DIAMOND */
			movement_diamond(g, &new_pos, direction);
			break;

		case M_EXIT:								/* OK, ONLY IF DIAMOND LIST IS EMPTY*/
			movement_exit(g, &new_pos, direction);
			break;
	}
}

/* Sets all elements in map array to 0 */
void reset_map(game_state *g)
{
	int i,j;
	for (i = 0; i < MAX_HEIGHT; i++)
		for (j = 0; j < MAX_WIDTH; j++)
			g->map[i][j] = 0;
}

/* Initializes variables */
void init(game_state *g)
{
	int x, y;

	g->diamond_counter = 0;
	g->running = true;
	
	/* Looks for diamonds and Rockford */
	for (y = 0; y < g->map_height; y++)
		for (x = 0; x < g->map_width; x++)
		{
			switch( g->map[y][x] )
			{
				case M_ROCKFORD:
					g->player_position.x = x;
					g->player_position.y = y;

					g->previous_position = g->player_position;
					break;

				case M_DIAMOND:
					g->diamond_counter++;
					break;
			}
		}
}


/* Updates position of diamonds and stones in
 * given column
 */
void update_column(game_state *g, int column)
{
	/* TODO : OPTYMALIZACJA */
	int i;
	for (i = g->map_height - 1; i >= 0; i--)
	{
		char field = g->map[i][column];

		if(field == M_DIAMOND || field == M_STONE)
		{
			position p;
			p.x = column;
			p.y = i;

			/* If there is space below, move down */
			while( g->map[p.y + 1][column] == M_EMPTY)
			{
				move_field(g, &p, DIR_DOWN);
				p.y++;
			}
		}
	}
}

/* Stabilizes map */
void update_global(game_state *g)
{
	int i;
	for(i = 0; i < g->map_width; i++)
		update_column(g, i);
}

/* Updates columns near Rockford */
void update(game_state *g)
{
	/* Horizontal movement */
	if(g->previous_position.x != g->player_position.x)
	{
		/* Update column on left and right */;
		update_column(g, g->player_position.x + 1);
		update_column(g, g->player_position.x - 1);
	}

	/* Vertical movement */
	if(g->previous_position.y != g->player_position.y)
	{
		update_column(g, g->player_position.x);
	}

	/* Update positions */
	g->previous_position = g->player_position;
}

/* Reacts to user's input */
void handle_input(game_state *g, int input)
{
	switch (input)
	{
		case DIR_UP:
		case DIR_DOWN:
		case DIR_LEFT:
		case DIR_RIGHT:
			move_rockford(g, input);
			break;

		/* Nothing to do. Stop and exit */
		case EOF:
			g->running = false;
			break;

	}
}


/* Main loop of program */
void start(game_state *g)
{
	update_global(g);

	while(g->running)
	{

		int input;
		update(g);

		#ifdef TUI
			clear();
			print_map(g);
			refresh();

			input = getch();
		#else
			input = getchar();
		#endif

		handle_input(g, input);

	}
	update_global(g);
}

int main(void)
{
	game_state g;
	char map[MAX_HEIGHT][MAX_WIDTH];
	g.map = map;

	reset_map(&g);	
	load_map(&g);
	init(&g);

	
	#ifdef TUI
		freopen("/dev/tty", "rw", stdin);

		initscr();
		cbreak();
		noecho();

		getmaxyx(stdscr, g.term_rows, g.term_cols);
	#endif

	start(&g);

	#ifdef TUI
		endwin();
	#else
		print_map(&g);
	#endif

	return 0;
}