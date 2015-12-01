/*
 * Programma in c che implementa il gioco del tetris
 * di Alessandro Righi
 */

/* TODO: - riscrivere funzioni per usare la struttura pezzo
 *       - usare un header per il Programma
 *       - gestione punteggio
 *       - preview pezzo successivo
 *       - pezzi del tetris colorati
 *       - bugfix vari
 */

#include "tetris.h"

int main(int argc, char *argv[]){
	int c;
  int i;
	int count=0;
	int bottom=0;
	int delay=1;
  int score=0;
  int level=1;
  char next[4][4];
	pezzo_t *p_cur = malloc(sizeof(pezzo_t));
  pezzo_t *p_next = malloc(sizeof(pezzo_t));

	srand(time(NULL));

	/* init ncurses */
	initCurses();

	init();
	/* main loop */

	pezzoRand(p_cur);
  pezzoRand(p_next);
	add (p_cur);
	while (c = getch()){
		switch (c){
			case KEY_LEFT:
				move_left(p_cur);
				break;
			case KEY_RIGHT:
				move_right(p_cur);
				break;
			case KEY_UP:
				rotate(p_cur);
				break;
			case KEY_DOWN:
				while (!move_down(p_cur));
        bottom=1;
				break;
      case 'p': /* pause */
        while (getch()!='p')
          usleep(100000);
        break;
      case 'r':
        init();
        score=0;
        level=1;
        pezzoRand(p_cur);
        pezzoRand(p_next);
        break;
		}

		count++;
		if (count>=150-pow(level,2)){
			count=0;
			bottom = move_down(p_cur);
		}

		if (bottom){
			bottom=0;
      score++;	pezzoRand(p_cur);
      pezzoRand(p_next);
      if (i=eliminateLine()){
			     score += i;
           count=0;
      }
      if (!(score%((int)pow(level+3,2))))
        level++;
			if (lost()){
				if (newGame()){
					init();
          score=0;
          level=1;
				} else {
					quit();
				}
			}
      memcpy(p_cur, p_next, sizeof(pezzo_t));
			pezzoRand(p_next);
			add(p_cur);
		}
		usleep(2000);
    getElement(p_next, next);
    printScore(score, level, next);

		printMatrix();
	}

	return 0;
}
