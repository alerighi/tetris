#include <string.h> /* for memset() */
#include <stdlib.h> /* for rand() */
#include <unistd.h> /* for alarm() */
#include <math.h>

#include "pieces.h"
#include "game.h"
#include "screen.h"
#include "high_score.h"

int level;
int score;

struct piece_s current_piece;
struct piece_s next_piece;

static struct piece_s rand_piece()
{
	struct piece_s p = {	
		.x = 4,
		.y = 3,
		.p = rand() % 7,
		.r = 0
	};

	return p; 
}

void game_pause(bool active) 
{
	if (active) {
		alarm(0);
	} else {
		useconds_t delay = 1000 * (800 * pow(0.9, level));
		ualarm(delay, delay);
	}
}

static void add()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			screen[current_piece.y - i][current_piece.x + j] |= tetris[current_piece.p][current_piece.r][i][j];
}

static void rem()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (tetris[current_piece.p][current_piece.r][i][j])
				screen[current_piece.y - i][current_piece.x + j] = 0;
}

static bool check()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (tetris[current_piece.p][current_piece.r][i][j]
				&& ((current_piece.y - i) >= Y 
					|| (current_piece.y - i) < 0 
					|| (current_piece.x + j) >= X 
					|| (current_piece.x + j) < 0 
					|| screen[current_piece.y - i][current_piece.x + j]))
				return false;
	return true;
}

static void swap_pieces() 
{
	current_piece = next_piece;
	next_piece = rand_piece();
	add();
}

void update_on_alarm()
{
	move_down(false);
	refresh_screen();
}

void start_new_game() 
{
	memset(screen, 0, sizeof(screen));
	current_piece = rand_piece();
	next_piece = rand_piece();
	score = 0;
	level = 1;
	add();
	refresh_screen();
	game_pause(false);
}

void move_left()
{
	rem();
	current_piece.x--;
	if (!check())
		current_piece.x++;
	add();
}

void move_right()
{
	rem();
	current_piece.x++;
	if (!check())
		current_piece.x--;
	add();
}

void rotate()
{
	rem();
	current_piece.r = (current_piece.r + 1) % 4;
	if (!check()) 
		current_piece.r = (current_piece.r - 1) % 4;
	add();
}

static bool game_is_lost()
{
	for (int i = 0; i < X; i++)
		if (screen[3][i]) /* if something is in the 4th line from top, game is lost */
			return true;
	
	return false;
}

static int eliminate_lines()
{
	int lines_eliminated = 0;

	/* search a line to eliminate */
	for (int i = 0; i < Y; i++) {

		/* search if the line is complete */
		int j; 
		for (j = 0; j < X && screen[i][j]; j++);
		
		if (j != X) /* line not complete, search next */
			continue; 

		/* eliminate line i and move all superior lines down */ 
		for (j = i; j > 2; j--)
			for (int k = 0; k < X; k++)
				screen[j][k] = screen[j - 1][k];

		lines_eliminated++;
	}

	return lines_eliminated;
}

void move_down(bool multi)
{
	bool bottom = false;

	/* move down the piece */
	do {
		rem();
		current_piece.y++;
		if (!check()) { /* cannot move further down */
			bottom = true;
			current_piece.y--;
		}
		add();
	} while (multi && !bottom); /* if multi, move until bottom */
	
	/* If the piece reaches bottom */
	if (bottom) {
		score++;

		/* Check and eliminate full lines */
		switch (eliminate_lines()) {
		case 1: score += 40 * level; break;
		case 2: score += 100 * level; break;
		case 3: score += 300 * level; break;
		case 4: score += 1200 * level; break;
		}

		/* calculate new level from score */
		level = 1 + score / 500;
		game_pause(false); /* set new alarm value based on new level */

		/* Checks if the game is lost */
		if (game_is_lost()) {
			game_pause(true);
			prompt_new_game();
			return;
		}

		/* Swaps the current piece whith the next one */
		swap_pieces();
	}
}
