#include "screen.h"

void init(){
	for (int i=0;i<Y;i++){
		memset(screen[i],' ',X);
	}
}

void printMatrix(){
  int e,i;
	wclear(game);
	for (e=0;e<Y;e++){
		for (i=0;i<X;i++){
			wprintw(game, "%c%c",screen[e][i],screen[e][i]);
		}
	}
	wrefresh(game);
}

int eliminateLine(){
	int i,j,e,ret=0;
	for (i=0;i<Y;i++){
		for (j=0;j<X;j++)
			if (screen[i][j]!='#')
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
		if (screen[3][i]=='#'){
			return 1;
		}
	}
	return 0;
}

void printScore(int score, int level, char next[4][4]){
  int i,e;
  wclear(score_win);
  wprintw(score_win, "Level: %d\n", level);
  wprintw(score_win, "Score: %d\n\n", score);
  wprintw(score_win, "Next element:\n\n");
  for (i=0;i<4;i++){
    wprintw(score_win, "   ");
    for (e=0;e<4;e++)
        wprintw(score_win, "%c%c", next[i][e], next[i][e]);
    wprintw(score_win, "\n");
	}
  wrefresh(score_win);
}


int newGame(){
	int ret=0;
	char c;
	nodelay(stdscr, FALSE);
  wclear(score_win);
	wprintw(score_win,"Giovare un'altra partita(y/n)?");
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

void initCurses(){
	initscr();             /* inizzializza lo schermo */
	cbreak(); 						 /* input senza buffer */
	keypad(stdscr, TRUE);  /* tasti freccia, ecc */
	nodelay(stdscr, TRUE); /* input senza delay */
	noecho();              /* non stampare il carattere a video */
	curs_set(0);					 /* non mostrare il cursore */

	refresh();

	/* stampa il titolo */
	title = newwin(TITLE_W_SIZE_Y,TITLE_W_SIZE_X,TITLE_W_START_Y,TITLE_W_START_X);
	wprintw(title, TITLE);
	wrefresh(title);

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
