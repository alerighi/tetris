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
  int c;
  int i;
  int count=0;
  int bottom=0;

  game_t game_o;

  char next[4][4];

  srand(time(NULL));

  /* init ncurses */
  initCurses();

  /* main loop */

  game_o = start_new_game();

  add (&game_o.p_cur);
  while ((c = getch())){
    switch (c){
      case KEY_LEFT:
        move_left(&game_o.p_cur);
        break;
      case KEY_RIGHT:
        move_right(&game_o.p_cur);
        break;
      case KEY_UP:
        rotate(&game_o.p_cur);
        break;
      case KEY_DOWN:
        while (!move_down(&game_o.p_cur));
        bottom=1;
        break;
      case 'p': /* pause */
        while (getch()!='p')
          usleep(100000);
        break;
      case 'r':
        game_o = start_new_game();
        break;
    }

    count++;
    if (count>=150-pow(game_o.level,2)){
      count=0;
      bottom = move_down(&game_o.p_cur);
    }

    if (bottom){
      bottom=0;
      game_o.score++;
      pezzoRand(&game_o.p_cur);
      pezzoRand(&game_o.p_next);
      if ((i=eliminateLine())){
           game_o.score += i*10;
           count=0;
      }
      if (!(game_o.score%((int)pow(game_o.level+3,2))))
        game_o.level++;
      if (lost()){
        if (newGame()){
          game_o = start_new_game();
        } else {
          quit();
        }
      }
      memcpy(&game_o.p_cur, &game_o.p_next, sizeof(pezzo_t));
      pezzoRand(&game_o.p_next);
      add(&game_o.p_cur);
    }
    usleep(2000);
    getElement(&game_o.p_next, next);
    printScore(game_o.score, game_o.level, next);

    printMatrix();
  }

  return 0;
}
