#include <curses.h>
#include <stdlib.h>
#include <string.h>

#include "screen.h"
#include "game.h"
#include "pieces.h"
#include "high_score.h"

/* Block character definition */
static const chtype BLOCK = ' ' | A_REVERSE;

static WINDOW *game_win;
static WINDOW *score_win;
static WINDOW *title_win;

char screen[Y][X];

static const char *const TITLE  =
	"111111  222222  333333  4444444  55  666666\n"
	"  11    22        33    44   44  55  66    \n"
	"  11    22222     33    444444   55  666666\n"
	"  11    22        33    44   44  55      66\n"
	"  11    222222    33    44    44 55  666666\n";

static const char *const CONTROLS =
	"\nControls:                     "
	"\n   arrow left/right: move     "
	"\n   arrow up: rotate           "
	"\n   arrow down: move down fast "
	"\n   space: put down            "
	"\n   p: pause                   "
	"\n   r: restart                 "
	"\n   q: quit                    ";

static void print_title(void) 
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

static void print_matrix(void)
{
	for (int y = 0; y < Y; y++) {
		for (int x = 0; x < X; x++) {
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

static void print_score(void)
{
	mvwprintw(score_win, 0, 0, "Level: %d                      ", level);
	mvwprintw(score_win, 1, 0, "Score: %d                      ", score);
	mvwprintw(score_win, 2, 0, "High score: %d                 ", high_score);
	mvwprintw(score_win, 3, 0, "Next piece:                    ");
	wattron(score_win, COLOR_PAIR(next_piece.p + 1)); 	

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			chtype ch = tetris[next_piece.p][next_piece.r][i][j] ? BLOCK : ' ';
			mvwaddch(score_win, 5 + 2 - i, 3 + j * 2, ch);
			mvwaddch(score_win, 5 + 2 - i, 4 + j * 2, ch);
		}
	}

	wattroff(score_win, COLOR_PAIR(next_piece.p + 1));	
	mvwprintw(score_win, 8, 0, CONTROLS);
	
	wrefresh(score_win);
}

void refresh_screen(void) 
{
	print_score();
	print_matrix();
}

void prompt_new_game(void)
{
	refresh_screen();
	wclear(score_win);
	wprintw(score_win, "Sorry, you lost :( score %d\n", score);
	
	if (score > high_score) {
		wprintw(score_win, "Congratulations! New record!\n");
		high_score = score;
	}
	
	wprintw(score_win, "Start a new game ? (y/n)");
	wrefresh(score_win);

	chtype ch;
	while ((ch = getch()) != 'y') {
		if (ch == 'n')
			exit(EXIT_SUCCESS);
	}

	start_new_game();
}

static void draw_windows(void) 
{
	int center = (COLS / 2);

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

	int min_lines = game_win_height + title_win_height;
	int min_columns = game_win_width + 30;

	if (LINES < min_lines || COLS < min_columns) {
		endwin();
		fprintf(stderr, "Your terminal is too small. Resize your terminal and retry!\n");
		exit(EXIT_FAILURE);
	}

	score_win = newwin(score_win_height, score_win_width, score_win_start_y, score_win_start_x);
	wrefresh(score_win);

	title_win = newwin(title_win_height ,title_win_width, title_win_start_y, title_win_start_x);
	print_title();

	game_win = newwin(game_win_height, game_win_width, game_win_start_y, game_win_start_x);
	box(game_win, 0, 0);
	wrefresh(game_win);
}

static void destroy_windows(void) 
{
	delwin(title_win);
	delwin(game_win);
	delwin(score_win);
	clear();
	refresh();
}

static void handle_sigwinch(int __attribute__((unused)) signal) 
{
	destroy_windows();
	draw_windows();
}

static bool has_256_colors(void) 
{
	char *term = getenv("TERM");
	return !strcmp(term, "xterm-256color") || !strcmp(term, "xterm");	
}

void init_curses(void)
{
	initscr();
	cbreak();              /* unbuffered input */
	keypad(stdscr, true);  /* for special keys */
	noecho();              /* do not echo character on screen */
	curs_set(false);       /* do not show cursor */
	refresh();

	/* ensure correct ncurses termination on exit */
	atexit((void (*)(void)) endwin);		

	/* register terminal resize signal handler */
	signal(SIGWINCH, handle_sigwinch);

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

_Noreturn void input_loop(void)
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
			exit(EXIT_SUCCESS);
		}
		refresh_screen();
	}
}
