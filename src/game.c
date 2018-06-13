#include <string.h> /* for memset() */
#include <stdbool.h>
#include <math.h>

#include "pieces.h"
#include "game.h"
#include "screen.h"
#include "score.h"

static struct piece current_piece;
static struct piece next_piece;

/* delay to wait for input (milliseconds) */
int delay; 

static void add_current_piece(void)
{
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (tetris[current_piece.piece][current_piece.rotation][y][x])
				screen[current_piece.position.y - y][current_piece.position.x + x] = current_piece.piece + 1;
		}
	}
}

static void remove_current_piece(void)
{
	for (int y = 0; y < 4; y++) { 
		for (int x = 0; x < 4; x++) {
			if (tetris[current_piece.piece][current_piece.rotation][y][x])
				screen[current_piece.position.y - y][current_piece.position.x + x] = 0;
		}
	}
}

static bool check_piece_overlap(void)
{
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (tetris[current_piece.piece][current_piece.rotation][y][x]
				&& (current_piece.position.y - y >= Y 
					|| current_piece.position.y - y < 0 
					|| current_piece.position.x + x >= X 
					|| current_piece.position.x + x < 0 
					|| screen[current_piece.position.y - y][current_piece.position.x + x]))
				return true;
		}
	}
	return false;
}

void start_new_game(void) 
{
	memset(screen, 0, sizeof(screen));
	current_piece = get_random_piece();
	next_piece = get_random_piece();
	score = 0;
	level = 1;
	add_current_piece();
}

static int eliminate_lines(void)
{
	int lines_eliminated = 0;

	/* search a line to eliminate */
	for (int y = 0; y < Y; y++) {

		/* search if the line is complete */
		int x; 
		for (x = 0; x < X && screen[y][x]; x++);
		
		if (x != X) /* line not complete, search next */
			continue; 

		/* eliminate line i and move all superior lines down */ 
		for (int h = y; h > 2; h--) {
			for (int k = 0; k < X; k++) {
				screen[h][k] = screen[h - 1][k];
			}
		}

		lines_eliminated++;
	}
	
	/* calculate points based on number of lines eliminated */
	const int points_per_line[] = { 1, 40, 100, 300, 1200 };

	return points_per_line[lines_eliminated];
}

static void handle_piece_bottom(void)
{
	score += eliminate_lines();

	/* calculate new level from score */
	level = 1 + score / 700;

	/* Swaps the current piece whith the next one */
	current_piece = next_piece;
	next_piece = get_random_piece();

	if (check_piece_overlap()) {
		prompt_new_game();
		return;
	}

	add_current_piece();
}

static bool do_move_down(void)
{
	bool bottom = false;

	remove_current_piece();
	current_piece.position.y++;
	if (check_piece_overlap()) { /* cannot move down */
		bottom = true;
		current_piece.position.y--;
	}
	add_current_piece();
	
	delay = 800 * pow(0.9, level);

	return bottom;
}

void move_down(void) 
{
	if (do_move_down())
		handle_piece_bottom();
}

void move_bottom(void)
{
	while (!do_move_down())
		/* loop */;
	handle_piece_bottom();
}

void move_left(void)
{
	remove_current_piece();
	current_piece.position.x--;
	if (check_piece_overlap())
		current_piece.position.x++;
	add_current_piece();
}

void move_right(void)
{
	remove_current_piece();
	current_piece.position.x++;
	if (check_piece_overlap())
		current_piece.position.x--;
	add_current_piece();
}

void rotate(void)
{
	remove_current_piece();
	current_piece.rotation = (current_piece.rotation + 1) % 4;
	if (check_piece_overlap()) 
		current_piece.rotation = (current_piece.rotation - 1) % 4;
	add_current_piece();
}

void get_next_piece(char next[4][4])
{
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			next[y][x] = tetris[next_piece.piece][next_piece.rotation][y][x] ? next_piece.piece + 1 : 0; 
		}
	}
}
