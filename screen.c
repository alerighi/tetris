#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "screen.h"
#include "game.h"
#include "tetris_pieces.h"

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
#define SCORE_W_SIZE_Y (GAME_W_SIZE_Y-4)

/* Winodws variables */
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

/* Static functions definitions */
static void print_matrix(void);
static void print_score(void);
static void print_title(void);

void refresh_screen() 
{
	print_score();
	print_matrix();
}

static void print_title() 
{
	unsigned long i;
	for (i = 0; i < strlen(TITLE); i++) {
		if (TITLE[i] == '\n' || TITLE[i] == ' ') {
			waddch(title_win, TITLE[i]);
		} else {
			wattron(title_win, COLOR_PAIR(TITLE[i] - 48));
			waddch(title_win, BLOCK);
			wattroff(title_win, COLOR_PAIR(TITLE[i] - 48));
		} 
	}
	wrefresh(title_win);
}

void print_matrix()
{
	int y, x;
	for (y = 0; y < Y; y++) {
		for (x = 0; x < X; x++) {
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

int game_is_lost()
{
	int i;
	for (i = 0; i < X; i++)
		if (screen[3][i])
			return 1;
	return 0;
}

void print_score()
{
	int i, e;
	mvwprintw(score_win, 0, 0, "Level: %d                      ", level);
	mvwprintw(score_win, 1, 0, "Score: %d                      ", score);
	mvwprintw(score_win, 2, 0, "High score: %d                 ", high_score);
	mvwprintw(score_win, 3, 0, "Next piece:                    ");
	wattron(score_win, COLOR_PAIR(next_piece.p + 1)); 	
	for (i = 0; i < 4; i++) {
		for (e = 0; e < 4; e++) {
			mvwaddch(score_win, 5 + 2 - i, 3 + e*2, tetris[next_piece.p][next_piece.r][i][e] ? BLOCK : ' ');
			mvwaddch(score_win, 5 + 2 - i, 4 + e*2, tetris[next_piece.p][next_piece.r][i][e] ? BLOCK : ' ');
		}
	}
	wattroff(score_win, COLOR_PAIR(next_piece.p + 1));	
	mvwprintw(score_win, 8, 0, CONTROLS);
	wrefresh(score_win);
}

void prompt_new_game()
{
	char c;
	refresh_screen();
	wclear(score_win);
	wprintw(score_win,"Sorry, you lost :( score %d\n", score);
	
	if (score > high_score) {
		wprintw(score_win,"Congratulations! New record!\n");
		high_score = score;
		save_score();
	}
	
	wprintw(score_win,"Start a new game ? (y/n)");
	wrefresh(score_win);
	while ((c = getch())) {
		switch (c)  {
		case 'y':
			start_new_game();
			return;
		case 'n': 
			quit();
		}
	}
}

void _Noreturn quit()
{
	endwin();
	save_score();
	exit(EXIT_SUCCESS);
}

void draw_windows() 
{
	/* stampa il titolo */
	title_win = newwin(TITLE_W_SIZE_Y ,TITLE_W_SIZE_X, TITLE_W_START_Y, TITLE_W_START_X);
	print_title();

	/* finestra contenitore per il gioco */
	game_border_win = newwin(GAME_BORDER_W_SIZE_Y, GAME_BORDER_W_SIZE_X, GAME_BORDER_W_START_Y, GAME_BORDER_W_START_X);
	box(game_border_win, 0, 0);
	wrefresh(game_border_win);

	/* finestra del gioco */
	game_win = newwin(GAME_W_SIZE_Y, GAME_W_SIZE_X, GAME_W_START_Y, GAME_W_START_X);
	wrefresh(game_win);

	/* finestra dei punteggi */
	score_win = newwin(SCORE_W_SIZE_Y, SCORE_W_SIZE_X, SCORE_W_START_Y, SCORE_W_START_X);
	wrefresh(score_win);
}

/* cancella tutte le finestre */
void destroy_windows() 
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

void init_curses()
{
	initscr();
	cbreak();              /* unbuffered input */
	keypad(stdscr, TRUE);  /* for special keys */
	noecho();              /* do not echo character on screen */
	curs_set(0);           /* do not show cursor */
	refresh();

	if ((GAME_BORDER_W_SIZE_Y+TITLE_W_SIZE_Y) > LINES 
		|| (GAME_BORDER_W_SIZE_X + 30) > COLS) {
		endwin();
		printf("Your terminal is too small. Please, resize your terminal !\n");
		exit(EXIT_FAILURE);
	}

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
}

void _Noreturn input_loop()
{
	while (true) {
		switch (getch()) {
		case KEY_LEFT:
			move_left();
			break;
		case KEY_RIGHT:
			move_right();
			break;
		case KEY_UP: /* rotate */
			rotate();
			break;
		case KEY_DOWN: /* fast down */
			move_down(0);
			break;
		case ' ': /* instant down */
			move_down(1);
			break;
		case 'p': /* pause */
			pause_game();
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

