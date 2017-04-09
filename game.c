#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "game.h"
#include "screen.h"

/* Include the file whit the definitions of the pieces */
#include "tetris_pieces.h"

#define score_filename ".tetris_score"

int level;
int score;
int high_score;

struct piece_s 
{
	int p;
	int r;
	int y;
	int x;
};

static struct piece_s current_piece;
static struct piece_s next_piece;

/* Static functions */
static int check(void);
static void rand_piece(struct piece_s *p);
static void rem(void);
static void add(void);

/* Functions implementations */
static void rand_piece(struct piece_s *p)
{
	p->x=4;
	p->y=3;
	p->p=rand()%7;
	p->r=0;
}

void swap_pieces() 
{
	current_piece = next_piece;
	rand_piece(&next_piece);
	add();
}

void start_new_game() 
{
	memset(screen, 0, sizeof(screen));
	rand_piece(&current_piece);
	rand_piece(&next_piece);
	score = 0;
	level = 1;
	add();
	refresh_screen();
	ualarm(2000*(150-pow(level,2)),2000*(150-pow(level,2)));
}

static void add(void)
{
	int i,e;
	for (i=0; i<4; i++)
		for (e=0; e<4; e++)
			if (tetris[current_piece.p][current_piece.r][i][e]=='*')
				screen[current_piece.y-i][current_piece.x+e]=current_piece.p+1;
}

static void rem(void)
{
	int i,e;
	for (i=0; i<4; i++)
		for (e=0; e<4; e++)
			if (tetris[current_piece.p][current_piece.r][i][e]=='*')
				screen[current_piece.y-i][current_piece.x+e]=0;
}

static int check(void)
{
	int i,e;
	for (i=0; i<4; i++)
		for (e=0; e<4; e++)
			if (tetris[current_piece.p][current_piece.r][i][e]=='*' 
				&& ((current_piece.y-i) >= Y 
					|| (current_piece.y-i) < 0 
					|| (current_piece.x+e) >= X 
					|| (current_piece.x+e) < 0 
					|| screen[current_piece.y-i][current_piece.x+e]))
							return 0;
	return 1;
}

void get_next(char str[4][4])
{
	int i,e;
	for (i=0;i<4;i++) 
	{
		for (e=0;e<4;e++) 
		{
			if (tetris[next_piece.p][next_piece.r][i][e]=='*')
				str[i][e]=next_piece.p+1;
			else
				str[i][e]=0;
		}
	}
}


void move_left(void)
{
	rem();
	current_piece.x--;
	if (!check())
		current_piece.x++;
	add();
}

void move_right(void)
{
	rem();
	current_piece.x++;
	if (!check())
		current_piece.x--;
	add();
}

void rotate(void)
{
	rem();
	current_piece.r++;
	if (current_piece.r>3)
		current_piece.r=0;
	if (!check())
	{
		current_piece.r--;
		if (current_piece.r<0)
			current_piece.r=3;
	}
	add();
}

int move_down()
{
	int ret=0, i;
	rem();
	current_piece.y++;
	if (!check())
	{
		ret=1;
		current_piece.y--;
	}
	add();
  
	/* If the piece reaches bottom */
	if (ret)
	{
		score++;

		/* Check and eliminate full lines */
		if ((i=eliminate_line()))
		{
			score += pow(i, 2) * 4;
			level = 1 + score / 50;
			ualarm(2000*(150-pow(level,2)),2000*(150-pow(level,2)));
		}

		/* Checks if the game is lost */
		if (game_is_lost())
		{
			prompt_new_game();
			return ret;
		}

		/* Swaps the current piece whith the next one */
		swap_pieces();
	}
	refresh_screen();
  
	return ret;
}

void load_score() 
{
	char filename[100];
	sprintf(filename, "%s/%s", getenv("HOME"), score_filename);
	FILE *score_fp = fopen(filename, "r");
	if (score_fp) 
	{
		int _ = fscanf(score_fp, "%d", &high_score);
		fclose(score_fp);
	} 
}

void save_score() 
{
	char filename[100];
	sprintf(filename, "%s/%s", getenv("HOME"), score_filename);
	FILE *score_fp = fopen(filename, "w+");
	if (score_fp) 
	{
		fprintf(score_fp, "%d\n", high_score);
		fclose(score_fp);
	} 
}

