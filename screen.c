#include <curses.h>
#include <stdlib.h>
#include <string.h>

#include "screen.h"

static WINDOW *game;
static WINDOW *game_o;
static WINDOW *score_win;
static WINDOW *title;

char screen[Y][X];
static chtype block = ' ' | A_REVERSE;


void print_title(char *str) {
  int i;
  for (i = 0; i < (int)strlen(str); i++) {
    if (str[i] == '\n')  waddch(title, '\n');
    else if (str[i] != ' ') {
      wattron(title, COLOR_PAIR(str[i]-48));
      waddch(title, block);
      wattroff(title, COLOR_PAIR(str[i]-48));
    }
    else waddch(title, ' ');
  }
  wrefresh(title);
}

void print_matrix(){
  int y, x;
  for (y = 0; y < Y; y++) {
    for (x = 0; x < X; x++) {
      if (screen[y][x]) {
        wattron(game, COLOR_PAIR(screen[y][x]));
        mvwaddch(game, y, x*2, block);
        mvwaddch(game, y, 1+x*2, block);
        wattroff(game, COLOR_PAIR(screen[y][x]));
      } else {
        mvwaddch(game, y, x*2, ' ');
        mvwaddch(game, y, 1+x*2, ' ');
      }
    }
  }
  wrefresh(game);
}

int eliminate_line(){
  int i,j,e,ret=0;
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

int lost(){
  int i;
  for (i=0;i<X;i++){
    if (screen[3][i]){
      return 1;
    }
  }
  return 0;
}

void print_score(int score, int level, char next[4][4]){
  int i,e;
  mvwprintw(score_win, 0, 0, "Level: %d          ", level);
  mvwprintw(score_win, 1, 0, "Score: %d          ", score);
  mvwprintw(score_win, 3, 0, "Next piece:      ");
  for (i = 0; i < 4; i++) {
    for (e = 0; e < 4; e++) {
      wattron(score_win, COLOR_PAIR(next[i][e]));
      mvwaddch(score_win, 5+i, 3+e*2, next[i][e] ? block : ' ');
      mvwaddch(score_win, 5+i, 4+e*2, next[i][e] ? block : ' ');
      wattroff(score_win, COLOR_PAIR(next[i][e]));
    }
  }
  mvwprintw(score_win, 8, 0, COMANDI);
  wrefresh(score_win);
}


int new_game(){
  int ret=0;
  char c;
  nodelay(stdscr, FALSE);
  wclear(score_win);
  wprintw(score_win,"Start another game ? (y/n)");
  wrefresh(score_win);
  while ((c = getch())){
    if (c=='y'){
      ret=1;
      break;
    }
    if (c=='n'){
      ret=0;
      break;
    }
  }
  nodelay(stdscr, TRUE);
  return ret;
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

  if ((GAME_O_W_SIZE_Y+TITLE_W_SIZE_Y) > LINES ||
  (GAME_O_W_SIZE_X+30) > COLS) {
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
  title = newwin(TITLE_W_SIZE_Y,TITLE_W_SIZE_X,TITLE_W_START_Y,TITLE_W_START_X);
  print_title(TITLE);

  /* finestra contenitore per il gioco */
  game_o = newwin(GAME_O_W_SIZE_Y, GAME_O_W_SIZE_X, GAME_O_W_START_Y, GAME_O_W_START_X);
  box(game_o, 0, 0);
  wrefresh(game_o);

  /* finestra del gioco */
  game = newwin(GAME_W_SIZE_Y, GAME_W_SIZE_X, GAME_W_START_Y, GAME_W_START_X);
  wrefresh(game);

  /* finestra dei punteggi */
  score_win = newwin(SCORE_W_SIZE_Y, SCORE_W_SIZE_X, SCORE_W_START_Y, SCORE_W_START_X);
  wrefresh(score_win);
}
