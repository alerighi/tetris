#include <curses.h>
#include <stdlib.h>
#include <string.h>

#include "screen.h"
#include "game.h"

/* Block character definition */
#define BLOCK (' ' | A_REVERSE)

/* window positioning definitions */
#define CENTER COLS/2

#define TITLE_W_SIZE_X 44
#define TITLE_W_SIZE_Y 5
#define TITLE_W_START_X CENTER-TITLE_W_SIZE_X/2-5
#define TITLE_W_START_Y 1

#define GAME_W_START_X CENTER-X*2
#define GAME_W_START_Y TITLE_W_SIZE_Y+3
#define GAME_W_SIZE_X X*2
#define GAME_W_SIZE_Y Y

#define GAME_BORDER_W_START_X GAME_W_START_X-1
#define GAME_BORDER_W_START_Y GAME_W_START_Y-1
#define GAME_BORDER_W_SIZE_X GAME_W_SIZE_X+2
#define GAME_BORDER_W_SIZE_Y GAME_W_SIZE_Y+2

#define SCORE_W_START_X GAME_W_START_X+GAME_W_SIZE_X+3
#define SCORE_W_START_Y GAME_W_START_Y
#define SCORE_W_SIZE_X COLS-(SCORE_W_START_X)
#define SCORE_W_SIZE_Y GAME_W_SIZE_Y-4

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
  "\nControls:                   "
  "\n   arrows: move             "
  "\n   p: pause                 "
  "\n   r: restart               "
  "\n   q: quit                  ";

/* Static functions definitions */
static void print_matrix(void);
static void print_score(void);
static void print_title(void);

void refresh_screen(void) {
  print_score();
  print_matrix();
}

static void print_title() {
  int i;
  for (i = 0; i < (int) strlen(TITLE); i++) {
    if (TITLE[i] == '\n') {
      waddch(title_win, '\n');
    } else if (TITLE[i] != ' ') {
      wattron(title_win, COLOR_PAIR(TITLE[i]-48));
      waddch(title_win, BLOCK);
      wattroff(title_win, COLOR_PAIR(TITLE[i]-48));
    } else {
      waddch(title_win, ' ');
    }
  }
  wrefresh(title_win);
}

void print_matrix(){
  int y, x;
  for (y = 0; y < Y; y++) {
    for (x = 0; x < X; x++) {
      if (screen[y][x]) {
        wattron(game_win, COLOR_PAIR(screen[y][x]));
        mvwaddch(game_win, y, x*2, BLOCK);
        mvwaddch(game_win, y, 1+x*2, BLOCK);
        wattroff(game_win, COLOR_PAIR(screen[y][x]));
      } else {
        mvwaddch(game_win, y, x*2, ' ');
        mvwaddch(game_win, y, 1+x*2, ' ');
      }
    }
  }
  wrefresh(game_win);
}

int eliminate_line(){
  int i, j, e;
  int ret = 0;
  for (i=0;i<Y;i++){
    for (j=0;j<X;j++)
      if (!screen[i][j])
        break;

    /* se il ciclo termina completamente allora elimino la riga */
    if (j==X){ /* trovata linea */
      for (j=i;j>2;j--)
      for (e=0;e<X;e++)
      screen[j][e]=screen[j-1][e];
      ret++;
    }
  }
  return ret;
}

int game_is_lost(){
  int i;
  for (i=0;i<X;i++){
    if (screen[3][i]){
      return 1;
    }
  }
  return 0;
}

void print_score(){
  int i,e;
  char next[4][4];
  get_next(next);
  mvwprintw(score_win, 0, 0, "Level: %d          ", level);
  mvwprintw(score_win, 1, 0, "Score: %d          ", score);
  mvwprintw(score_win, 3, 0, "Next piece:        ");
  for (i = 0; i < 4; i++) {
    for (e = 0; e < 4; e++) {
      wattron(score_win, COLOR_PAIR(next[i][e]));
      mvwaddch(score_win, 5+2-i, 3+e*2, next[i][e] ? BLOCK : ' ');
      mvwaddch(score_win, 5+2-i, 4+e*2, next[i][e] ? BLOCK : ' ');
      wattroff(score_win, COLOR_PAIR(next[i][e]));
    }
  }
  mvwprintw(score_win, 8, 0, CONTROLS);
  wrefresh(score_win);
}

void prompt_new_game(){
  char c;
  nodelay(stdscr, FALSE);
  wclear(score_win);
  wprintw(score_win,"Start a new game ? (y/n)");
  wrefresh(score_win);
  while ((c = getch())){
    if (c=='y'){
      nodelay(stdscr, TRUE);
      start_new_game();
      break;
    }
    if (c=='n'){
      quit();
    }
  }
}

void quit(){
  endwin();
  exit(0);
}

void init_curses(){
  initscr();             /* inizzializza lo schermo */
  cbreak();              /* input senza buffer */
  keypad(stdscr, TRUE);  /* tasti freccia, ecc */
  nodelay(stdscr, TRUE); /* input senza delay */
  noecho();              /* non stampare il carattere a video */
  curs_set(0);           /* non mostrare il cursore */
  refresh();

  if ((GAME_BORDER_W_SIZE_Y+TITLE_W_SIZE_Y) > LINES ||
  (GAME_BORDER_W_SIZE_X+30) > COLS) {
    endwin();
    printf("Your terminal is too small. Please, resize your terminal !\n");
    exit(1);
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
  /* stampa il titolo */
  title_win = newwin(TITLE_W_SIZE_Y,TITLE_W_SIZE_X,TITLE_W_START_Y,TITLE_W_START_X);
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
