/*
 * Programma in c che implementa il gioco del tetris
 * di Alessandro Righi
 */

/* TODO: - realizzare struttura gioco
 *       - sistemare il codice
 *       - sistemare file e aggiungere funzioni
 *       - pezzi del tetris colorati
 *       - bugfix vari
 *       - commenti !!!
 */

#include "tetris.h"


int main(int argc, char *argv[]){
  int i;
  int count=0;
  int bottom=0;
  int level = 1;
  int score = 0;
  pezzo_t pezzo_corrente;
  pezzo_t pezzo_prossimo;

  pezzo_rand(&pezzo_corrente);
  pezzo_rand(&pezzo_prossimo);

  char next[4][4];

  srand(time(NULL));

  /* init ncurses */
  init_curses();

  /* main loop */

  add (&pezzo_corrente);
  while (true){
    switch (getch()){
      case KEY_LEFT:
        move_left(&pezzo_corrente);
        break;
      case KEY_RIGHT:
        move_right(&pezzo_corrente);
        break;
      case KEY_UP:
        rotate(&pezzo_corrente);
        break;
      case KEY_DOWN:
        while (!move_down(&pezzo_corrente));
        bottom=1;
        break;
      case 'p': /* pause */
        while (getch()!='p')
          usleep(100000);
        break;
      case 'r':
        memset(screen, 0, sizeof(screen));
        pezzo_rand(&pezzo_corrente);
        pezzo_rand(&pezzo_prossimo);
        score = 0;
        level = 1;
        break;
    }

    count++;
    if (count>=150-pow(level,2)){
      count=0;
      bottom = move_down(&pezzo_corrente);
    }

    if (bottom){
      bottom=0;
      score++;
      pezzo_rand(&pezzo_corrente);
      if ((i=eliminate_line())){
           score += i*14;
           count=0;
      }
      level = score / 50;
      if (lost()){
        if (new_game()){
          memset(screen, 0, sizeof(screen));
          pezzo_rand(&pezzo_corrente);
          pezzo_rand(&pezzo_prossimo);
          score = 0;
          level = 1;
        } else {
          quit();
        }
      }
      memcpy(&pezzo_corrente, &pezzo_prossimo, sizeof(pezzo_t));
      pezzo_rand(&pezzo_prossimo);
      add(&pezzo_corrente);
    }
    usleep(2000);
    get_element(&pezzo_prossimo, next);
    print_score(score, level, next);

    print_matrix();
  }

  return 0;
}
