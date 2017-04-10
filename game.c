#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <math.h>

#include "game.h"
#include "screen.h"

/* Include the file whit the definitions of the pieces */
#include "tetris_pieces.h"

#define SCORE_FILENAME ".tetris_score"

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

static int check(void);
static void rand_piece(struct piece_s *p);
static void rem(void);
static void add(void);
static void set_alarm(void);
static void cancel_alarm(void);
static void get_score_filename(char *filename);


static void rand_piece(struct piece_s *p)
{
	p->x = 4;
	p->y = 3;
	p->p = rand() % 7;
	p->r = 0;
}

void set_alarm()
{
	ualarm(2000*(150-pow(level,2)),2000*(150-pow(level,2)));

}

void cancel_alarm()
{
	alarm(0);
}

void pause_game()
{
	cancel_alarm();
	while (getch() != 'p');
	set_alarm();
}

void swap_pieces() 
{
	current_piece = next_piece;
	rand_piece(&next_piece);
	add();
}

void update_signal(int signal)
{
	move_down(0);
	refresh_screen();
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
	set_alarm();
}

static void add(void)
{
	int i, e;
	for (i = 0; i < 4; i++)
		for (e = 0; e < 4; e++)
			if (tetris[current_piece.p][current_piece.r][i][e] == '*')
				screen[current_piece.y-i][current_piece.x+e] = current_piece.p + 1;
}

static void rem(void)
{
	int i, e;
	for (i = 0; i < 4; i++)
		for (e = 0; e < 4; e++)
			if (tetris[current_piece.p][current_piece.r][i][e]=='*')
				screen[current_piece.y-i][current_piece.x+e] = 0;
}

static int check(void)
{
	int i, e;
	for (i = 0; i < 4; i++)
		for (e = 0; e < 4; e++)
			if (tetris[current_piece.p][current_piece.r][i][e] == '*' 
				&& ((current_piece.y - i) >= Y 
					|| (current_piece.y - i) < 0 
					|| (current_piece.x + e) >= X 
					|| (current_piece.x + e) < 0 
					|| screen[current_piece.y - i][current_piece.x + e]))
							return 0;
	return 1;
}

void get_next(char str[4][4])
{
	int i, e;
	for ( i = 0; i < 4; i++) 
	{
		for (e = 0; e < 4; e++) 
		{
			if (tetris[next_piece.p][next_piece.r][i][e] == '*')
				str[i][e] = next_piece.p + 1;
			else
				str[i][e] = 0;
		}
	}
}

void move_left(void)
{
	rem();
	current_piece.x -= 1;
	if (!check())
		current_piece.x += 1;
	add();
}

void move_right(void)
{
	rem();
	current_piece.x += 1;
	if (!check())
		current_piece.x -= 1;
	add();
}

void rotate(void)
{
	rem();
	current_piece.r += 1;
	if (current_piece.r > 3)
		current_piece.r = 0;
	if (!check())
	{
		current_piece.r -= 1;
		if (current_piece.r < 0)
			current_piece.r = 3;
	}
	add();
}

void move_down(int multi)
{
	int i, bottom = 0;
	do 
	{
		rem();
		current_piece.y += 1;
		if (!check())
		{
			bottom = 1;
			current_piece.y -= 1;
		}
		add();
	} 
	while(multi && !bottom);
	
	/* If the piece reaches bottom */
	if (bottom)
	{
		score += 1;

		/* Check and eliminate full lines */
		if ((i = eliminate_line()))
		{
			score += pow(i, 2) * 4;
			level = 1 + score / 50;
			set_alarm();
		}

		/* Checks if the game is lost */
		if (game_is_lost())
		{
			cancel_alarm();
			prompt_new_game();
			return;
		}

		/* Swaps the current piece whith the next one */
		swap_pieces();
	}
}

int eliminate_line()
{
	int i, j, e;
	int ret = 0;
	for (i = 0; i < Y; i++)
	{
		for (j = 0; j < X; j++)
			if (!screen[i][j])
				break;

		/* se il ciclo termina completamente allora elimino la riga */
		if (j == X) /* trovata linea */
		{
			for (j = i; j > 2; j--)
				for (e = 0; e < X; e++)
					screen[j][e]=screen[j - 1][e];
			ret++;
		}
	}
	return ret;
}

void get_score_filename(char *filename)
{
	sprintf(filename, "%s/%s", getenv("HOME"), SCORE_FILENAME);
}

void load_score() 
{
	char filename[1024];
	FILE *score_fp;
	
	get_score_filename(filename);
	if ((score_fp = fopen(filename, "r")))
	{
		fscanf(score_fp, "%d", &high_score);
		fclose(score_fp);
	} 
}

void save_score() 
{
	char filename[1024];
	FILE *score_fp;
	
	get_score_filename(filename);
	if ((score_fp = fopen(filename, "w+"))) 
	{
		fprintf(score_fp, "%d\n", high_score);
		fclose(score_fp);
	} 
}

