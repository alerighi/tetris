/*
 * tetris - a small ncursess tetris game
 * Copyright (C) 2016-2019 Alessandro Righi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "screen.h"
#include "game.h"
#include "score.h"

/* filled block character */
static const chtype BLOCK = ' ' | A_REVERSE;

static WINDOW *game_win, *score_win, *title_win;

char screen[Y][X];

static const char TITLE[] =
	"111111  222222  333333  4444444  55  666666\n"
	"  11    22        33    44   44  55  66    \n"
	"  11    22222     33    444444   55  666666\n"
	"  11    22        33    44   44  55      66\n"
	"  11    222222    33    44    44 55  666666\n";

static const char CONTROLS[] =
	"LEFT/RIGHT (h/l): move left/right\n"
	"UP (k): rotate piece             \n"
	"DOWN (j): move down              \n"
	"SPACE: fast down                 \n"
	"P: pause game                    \n"
	"Q: quit                          \n";

static void
print_title(void)
{
	const char *c;

	for (c = TITLE; *c != '\0'; c++) {
		if (*c == '\n' || *c == ' ') {
			waddch(title_win, (chtype) *c);
		} else {
			wattron(title_win, COLOR_PAIR(*c - 48));
			waddch(title_win, BLOCK);
			wattroff(title_win, COLOR_PAIR(*c - 48));
		}
	}

	wrefresh(title_win);
}

static void
print_matrix(void)
{
	int y, x;

	for (y = 0; y < Y; y++) {
		for (x = 0; x < X; x++) {
			if (screen[y][x]) {
				wattron(game_win, COLOR_PAIR(screen[y][x]));
				mvwaddch(game_win, y + 1, x * 2 + 1, BLOCK);
				mvwaddch(game_win, y + 1, x * 2 + 2, BLOCK);
				wattroff(game_win, COLOR_PAIR(screen[y][x]));
			} else {
				mvwaddch(game_win, y + 1, x * 2 + 1, ' ');
				mvwaddch(game_win, y + 1, x * 2 + 2, ' ');
			}
		}
	}

	wrefresh(game_win);
}

static void
print_score(void)
{
	int y, x;
	char next[4][4];

	werase(score_win);
	mvwprintw(score_win, 1, 0, "Level: %d                      ", level);
	mvwprintw(score_win, 2, 0, "Score: %d                      ", score);
	mvwprintw(score_win, 3, 0, "High score: %d                 ", high_score);

	mvwaddch(score_win, 5, 0, ACS_ULCORNER);
	mvwaddch(score_win, 5, 13, ACS_URCORNER);
	mvwaddch(score_win, 10, 0, ACS_LLCORNER);
	mvwaddch(score_win, 10, 13, ACS_LRCORNER);

	for (x = 1; x < 13; x++) {
		mvwaddch(score_win, 10, x, ACS_HLINE);
	}

	for (y = 6; y < 10; y++) {
		mvwaddch(score_win, y, 0, ACS_VLINE);
		mvwaddch(score_win, y, 13, ACS_VLINE);
	}
	mvwprintw(score_win, 5, 2, "next piece");

	get_next_piece(next);
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			chtype ch = next[y][x] ? BLOCK : ' ';
			wattron(score_win, COLOR_PAIR(next[y][x]));
			mvwaddch(score_win, 9 - y, 3 + x * 2, ch);
			mvwaddch(score_win, 9 - y, 4 + x * 2, ch);
			wattroff(score_win, COLOR_PAIR(next[y][x]));
		}
	}

	mvwprintw(score_win, 12, 0, CONTROLS);

	wrefresh(score_win);
}

static void
refresh_screen(void)
{
	print_score();
	print_matrix();
}

void
prompt_new_game(void)
{
	chtype ch;

	refresh_screen();
	wclear(score_win);
	wprintw(score_win, "Sorry, you lost :( score %d\n", score);

	if (score > high_score) {
		wprintw(score_win, "Congratulations! New record!\n");
		high_score = score;
	}

	wprintw(score_win, "Start a new game ? (y/n)");
	wrefresh(score_win);

	while ((ch = getch()) != 'y') {
		if (ch == 'n')
			exit(EXIT_SUCCESS);
	}

	start_new_game();
}

static void
draw_windows(void)
{
	int center = COLS / 2;

	int title_win_width = 44;
	int title_win_height = 5;
	int title_win_start_x = center - title_win_width / 2 - 5;
	int title_win_start_y = 1;

	int game_win_start_x = center - X * 2 - 1;
	int game_win_start_y = title_win_height + 2;
	int game_win_width = X * 2 + 2;
	int game_win_height = Y + 2;

	int score_win_start_x = game_win_start_x + game_win_width + 3;
	int score_win_start_y = game_win_start_y;
	int score_win_width = COLS - score_win_start_x;
	int score_win_height = game_win_height - 1;

	score_win = newwin(score_win_height, score_win_width, score_win_start_y, score_win_start_x);

	title_win = newwin(title_win_height ,title_win_width, title_win_start_y, title_win_start_x);
	print_title();

	game_win = newwin(game_win_height, game_win_width, game_win_start_y, game_win_start_x);
	box(game_win, 0, 0);
}

static void
destroy_windows(void)
{
	delwin(title_win);
	delwin(game_win);
	delwin(score_win);
	clear();
	refresh();
}

static void
init_graphics(void)
{
	initscr();
	cbreak();              /* unbuffered input */
	keypad(stdscr, 1);     /* for special keys */
	noecho();              /* do not echo character on screen */
	curs_set(0);           /* do not show cursor */
	timeout(100);	       /* wait 100ms for input */

	/* ensure correct ncurses termination on exit */
	atexit((void (*)(void)) endwin);

	/* color initialization */
	use_default_colors();
	start_color();
	init_pair(1, COLOR_CYAN, -1);
	init_pair(2, COLOR_YELLOW, -1);
	init_pair(3, 203, -1);
	init_pair(4, COLOR_BLUE, -1);
	init_pair(5, COLOR_MAGENTA, -1);
	init_pair(6, COLOR_GREEN, -1);
	init_pair(7, COLOR_RED, -1);

	refresh();
	draw_windows();
	refresh_screen();
}

static void
main_loop(void)
{
	init_graphics();
	for (;;) {
		delay -= 100;
		if (delay <= 0)
			move_down();
		switch (getch()) {
		case 'h':
		case KEY_LEFT:
			move_left();
			break;
		case 'l':
		case KEY_RIGHT:
			move_right();
			break;
		case 'k':
		case KEY_UP:
			rotate();
			break;
		case 'j':
		case KEY_DOWN:
			move_down();
			break;
		case ' ':
			move_bottom();
			break;
		case 'p':
			while (getch() != 'p');
			break;
		case 'q':
			return;
		case KEY_RESIZE:
			destroy_windows();
			draw_windows();
		}
		refresh_screen();
	}
}

int
main(void)
{
	srand((unsigned) time(NULL));
	init_score();
	start_new_game();
	main_loop();
}

