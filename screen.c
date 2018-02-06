#include <curses.h>
#include <stdlib.h>
#include <string.h>

#include "screen.h"
#include "game.h"
#include "pieces.h"
#include "high_score.h"

/* Block character definition */
#define BLOCK (' ' | A_REVERSE)

/* window positioning definitions */
#define CENTER (COLS/2)

#define TITLE_W_SIZE_X 44
#define TITLE_W_SIZE_Y 5
#define TITLE_W_START_X (CENTER-TITLE_W_SIZE_X/2-5)
#define TITLE_W_START_Y 1

#define GAME_W_START_X (CENTER-X*2)
#define GAME_W_START_Y (TITLE_W_SIZE_Y+3)
#define GAME_W_SIZE_X (X*2)
#define GAME_W_SIZE_Y (Y)

#define GAME_BORDER_W_START_X (GAME_W_START_X-1)
#define GAME_BORDER_W_START_Y (GAME_W_START_Y-1)
#define GAME_BORDER_W_SIZE_X (GAME_W_SIZE_X+2)
#define GAME_BORDER_W_SIZE_Y (GAME_W_SIZE_Y+2)

#define SCORE_W_START_X (GAME_W_START_X+GAME_W_SIZE_X+3)
#define SCORE_W_START_Y (GAME_W_START_Y)
#define SCORE_W_SIZE_X (COLS-SCORE_W_START_X)
#define SCORE_W_SIZE_Y (GAME_W_SIZE_Y-1)

static WINDOW *game_win;
static WINDOW *game_border_win;
static WINDOW *score_win;
static WINDOW *title_win;

char screen[Y][X];

static const char *TITLE  =
	"111111  222222  333333  4444444  55  666666\n"
	"  11    22        33    44   44  55  66    \n"
	"  11    22222     33    444444   55  666666\n"
	"  11    22        33    44   44  55      66\n"
	"  11    222222    33    44    44 55  666666\n";

static const char *CONTROLS =
	"\nControls:                     "
	"\n   arrow left/right: move     "
	"\n   arrow up: rotate           "
	"\n   arrow down: move down fast "
	"\n   space: put down            "
	"\n   p: pause                   "
	"\n   r: restart                 "
	"\n   q: quit                    ";

static void print_title() 
{
	for (const char *c = TITLE; *c != '\0'; c++) {
		if (*c == '\n' || *c == ' ') {
			waddch(title_win, (unsigned int) *c);
		} else {
			wattron(title_win, COLOR_PAIR(*c - 48));
			waddch(title_win, BLOCK);
			wattroff(title_win, COLOR_PAIR(*c - 48));
		} 
	}

	wrefresh(title_win);
}

static void print_matrix()
{
	for (int y = 0; y < Y; y++) {
		for (int x = 0; x < X; x++) {
			if (screen[y][x]) {
				wattron(game_win, COLOR_PAIR(screen[y][x]));
				mvwaddch(game_win, y, x*2, BLOCK);
				mvwaddch(game_win, y, 1 + x*2, BLOCK);
				wattroff(game_win, COLOR_PAIR(screen[y][x]));
			} else {
				mvwaddch(game_win, y, x*2, ' ');
				mvwaddch(game_win, y, 1 + x*2, ' ');
			}
		}
	}

	wrefresh(game_win);
}

static void print_score()
{
	mvwprintw(score_win, 0, 0, "Level: %d                      ", level);
	mvwprintw(score_win, 1, 0, "Score: %d                      ", score);
	mvwprintw(score_win, 2, 0, "High score: %d                 ", high_score);
	mvwprintw(score_win, 3, 0, "Next piece:                    ");
	wattron(score_win, COLOR_PAIR(next_piece.p + 1)); 	

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mvwaddch(score_win, 5 + 2 - i, 3 + j*2, tetris[next_piece.p][next_piece.r][i][j] ? BLOCK : ' ');
			mvwaddch(score_win, 5 + 2 - i, 4 + j*2, tetris[next_piece.p][next_piece.r][i][j] ? BLOCK : ' ');
		}
	}

	wattroff(score_win, COLOR_PAIR(next_piece.p + 1));	
	mvwprintw(score_win, 8, 0, CONTROLS);
	
	wrefresh(score_win);
}

void refresh_screen() 
{
	print_score();
	print_matrix();
}

void prompt_new_game()
{
	refresh_screen();
	wclear(score_win);
	wprintw(score_win, "Sorry, you lost :( score %d\n", score);
	
	if (score > high_score) {
		wprintw(score_win, "Congratulations! New record!\n");
		high_score = score;
		save_score();
	}
	
	wprintw(score_win, "Start a new game ? (y/n)");
	wrefresh(score_win);

	int c;
	while ((c = getch()) != 'y') {
		if (c == 'n')
			quit();
	}

	start_new_game();
}

void quit()
{
	endwin();
	save_score();
	exit(EXIT_SUCCESS);
}

static void draw_windows() 
{
	title_win = newwin(TITLE_W_SIZE_Y ,TITLE_W_SIZE_X, TITLE_W_START_Y, TITLE_W_START_X);
	print_title();

	game_border_win = newwin(GAME_BORDER_W_SIZE_Y, GAME_BORDER_W_SIZE_X, GAME_BORDER_W_START_Y, GAME_BORDER_W_START_X);
	box(game_border_win, 0, 0);
	wrefresh(game_border_win);

	game_win = newwin(GAME_W_SIZE_Y, GAME_W_SIZE_X, GAME_W_START_Y, GAME_W_START_X);
	wrefresh(game_win);

	score_win = newwin(SCORE_W_SIZE_Y, SCORE_W_SIZE_X, SCORE_W_START_Y, SCORE_W_START_X);
	wrefresh(score_win);
}

static void destroy_windows() 
{
	delwin(title_win);
	delwin(game_border_win);
	delwin(game_win);
	delwin(score_win);
	clear();
	refresh();
}

void redraw_screen() 
{
	destroy_windows();
	draw_windows();
}

bool has_256_colors() 
{
	char *term = getenv("TERM");
	return !strcmp(term, "xterm-256color") || !strcmp(term, "xterm");	
}

void init_curses()
{
	initscr();
	cbreak();              /* unbuffered input */
	keypad(stdscr, true);  /* for special keys */
	noecho();              /* do not echo character on screen */
	curs_set(false);       /* do not show cursor */
	refresh();

	if ((GAME_BORDER_W_SIZE_Y+TITLE_W_SIZE_Y) > LINES 
		|| (GAME_BORDER_W_SIZE_X + 30) > COLS) {
		endwin();
		fprintf(stderr, "Your terminal is too small. Resize your terminal and retry!\n");
		exit(EXIT_FAILURE);
	}

	use_default_colors();
	start_color();

	init_pair(PIECE_I, COLOR_CYAN, -1);
	init_pair(PIECE_O, COLOR_YELLOW, -1);
	if (has_256_colors())
		init_pair(PIECE_L, 203, -1);
	else
		init_pair(PIECE_L, COLOR_WHITE, -1);
	init_pair(PIECE_J, COLOR_BLUE, -1);
	init_pair(PIECE_T, COLOR_MAGENTA, -1);
	init_pair(PIECE_S, COLOR_GREEN, -1);
	init_pair(PIECE_Z, COLOR_RED, -1);

	refresh();
	draw_windows();
}

void input_loop()
{
	while (true) {
		switch (getch()) {
		case KEY_LEFT:
			move_left();
			break;
		case KEY_RIGHT:
			move_right();
			break;
		case KEY_UP:
			rotate();
			break;
		case KEY_DOWN: /* fast down */
			move_down(false);
			break;
		case ' ': /* instant down */
			move_down(true);
			break;
		case 'p': /* pause */
			game_pause(true);
			while (getch() != 'p');
			game_pause(false);
			break;
		case 'r': /* restart */
			start_new_game();
			break;
		case 'q': /* quit */
			quit();
		}
		refresh_screen();
	}
}
