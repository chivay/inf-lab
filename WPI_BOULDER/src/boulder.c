#include <stdio.h>
#include <stdlib.h>

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

#define UP    'w'
#define DOWN  's' 
#define LEFT  'a'
#define RIGHT 'd'

char map[MAX_HEIGHT][MAX_WIDTH];
int map_height,
	map_width;

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


void init(void)
{
}

void run(void)
{
	
}

int main(void)
{
	load_map();
	init();
	print_map();
	run();
	return 0;
}
