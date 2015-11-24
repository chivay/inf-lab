#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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


/* Simple structure to define coordinates on map */
typedef struct {
	int x;
	int y;
} position;


/* Returns position structure with given values */
position make_position(int x, int y)
{
	position p;
	p.x = x;
	p.y = y;
	return p;
}


/* UTILITY FUNCTIONS */
int max(int a, int b)
{
	return a > b ? a : b;
}

/* Reads line from STDIN until end of line
 * Returns number of characters read */
int getline(char *s)
{
	char c;
	int i = 0;
	while( (c = getchar()) != '\n' && c != EOF && i < MAX_WIDTH)
		s[i++] = c;
	s[i] = '\n';
	return i+1;
}

/* Computes new coordinates for given direction of movement */
position get_position(position pos, char direction)
{
	switch(direction)
	{
		case DIR_UP:
			pos.y--;
			break;
		case DIR_DOWN:
			pos.y++;
			break;
		case DIR_LEFT:
			pos.x--;
			break;
		case DIR_RIGHT:
			pos.x++;
			break;
	}
	return pos;
}
/* END UTILITY FUNCTIONS */


/* GLOBAL VARIABLES */

/* Array contaning map data */
char map[MAX_HEIGHT][MAX_WIDTH];

/* Map dimensions */
int map_height,
	map_width;

/* Controls end of execution */
bool RUNNING;

/* Rockford position (current and from previous iteration) */
position player_position;
position previous_position;

/* Number of uncollected diamonds */
int diamond_counter;

/* END GLOBAL VARIABLES */


/*
 * Reads map data from STDIN until it encounters
 * an empty line
 */
void load_map(void)
{
	char *s;
	int len;
	map_height = 0;
	map_width  = 0;

	s = (char *) malloc(MAX_WIDTH * sizeof(char));

	/* Load until only \n is read */
	while ( ( len = getline(s) ) > 1)
	{
		int i;
		for (i = 0; i < len; i++)
			map[map_height][i] = s[i];

		map_height++;
		map_width = max(map_width, len);
	}

	free(s);
}

/* Print map to STDOUT */
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

/* Check if it is possible to push a stone */
bool movement_allowed(position stone_pos, char direction)
{
	switch(direction)
	{
		case DIR_UP:
		case DIR_DOWN:
			return false;     /* Pushing stone up or down not allowed */		
			break;
		case DIR_LEFT:
			stone_pos.x--;
			break;
		case DIR_RIGHT:
			stone_pos.x++;
			break;
	}
	return (map[stone_pos.y][stone_pos.x] == M_EMPTY);
}

/* Moves source field to target field, marking source as empty */
void move_field(position src, char direction)
{
	position dst = get_position(src, direction);

	map[dst.y][dst.x] = map[src.y][src.x];
	map[src.y][src.x] = M_EMPTY;
}

/* Handle Rockford's movement based on chosen direction */
void move_rockford(char direction)
{
	position new_pos;

	/* Compute new position */
	new_pos = get_position(player_position, direction);

	/* Determine what to do based on target field type */
	switch(map[new_pos.y][new_pos.x])
	{
		case M_EMPTY:								/* OK TO MOVE */
		case M_DIRT:								/* OK TO MOVE */
			move_field(player_position, direction);
			player_position = new_pos;
			break;

		case M_ROCK:								/* DO NOTHING, NOT ALLOWED */
			break;

		case M_STONE:								/* OK, ONLY IF NEXT SPACE IS EMPTY*/
			if (movement_allowed(new_pos, direction))
			{
				move_field(new_pos, direction);
				move_field(player_position, direction);
				player_position = new_pos;

			}
			break;

		case M_DIAMOND:								/* OK, REMOVE DIAMOND */
			diamond_counter--;

			move_field(player_position, direction);
			player_position = new_pos;
			break;

		case M_EXIT:								/* OK, ONLY IF DIAMOND LIST IS EMPTY*/
			if (diamond_counter == 0)
			{
				move_field(player_position, direction);
				/* Rockford disappears */
				map[new_pos.y][new_pos.x] = M_EXIT;
				RUNNING = false;                    
			}
			break;
	}
}


/* Initialize variables */
void init(void)
{
	int x, y;

	diamond_counter = 0;
	RUNNING = true;
	
	/* Looks for diamonds and Rockford */
	for (y = 0; y < map_height; y++)
	{
		for (x = 0; x < map_width; x++)
		{
			switch(map[y][x])
			{
				case M_ROCKFORD:
					player_position.x = x;
					player_position.y = y;

					previous_position = player_position;
					break;
				case M_STONE:
					break;
				case M_DIAMOND:
					diamond_counter++;
					break;
			}
		}
	}
}


/*
 * Update position of diamonds and stones in
 * given column
 */
void updateColumn(int column)
{
	int i;
	for (i = map_height - 1; i >= 0; i--)
	{
		char field = map[i][column];
		if(field == M_DIAMOND || field == M_STONE)
		{
			position p = make_position(column, i);
			/* If there is space below, move down */
			while(map[p.y + 1][column] == M_EMPTY)
			{
				move_field(p, DIR_DOWN);
				p.y++;
			}
		}
	}
}

/* Stabilize map */
void updateGlobal()
{
	int i,j;
	for(i = map_height-1; i >= 0; i--)
	{
		for(j = 0; j < map_width; j++)
		{
			char field = map[i][j];

			if(field == M_DIAMOND || field == M_STONE)
			{
				position p = make_position(j, i);

				/* If there is space below, move down */
				while(map[p.y + 1][p.x] == M_EMPTY)
				{
					move_field(p, DIR_DOWN);
					p.y++;
				}
			}
		}
	}

}

/* Update columns near Rockford */
void update()
{
	/* Horizontal movement */
	if(previous_position.x != player_position.x)
	{
		/* Update column on left and right */;
		updateColumn(player_position.x + 1);
		updateColumn(player_position.x - 1);
	}

	/* Vertival movement */
	if(previous_position.y != player_position.y)
	{
		updateColumn(player_position.x);
	}

	previous_position = player_position;
}

/* React to user's input */
void handle_input(void)
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
	updateGlobal();
	while(RUNNING)
	{
		update();
		handle_input();
	}
	updateGlobal();
}

int main(void)
{
	load_map();
	init();
	start();
	print_map();

	return 0;
}