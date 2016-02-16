#ifndef GAME_H
#define GAME_H

#include "screen.h"

typedef struct pezzo_s {
  int p;
  int r;
  int y;
  int x;
} pezzo_t;

typedef struct game_s {
  int score;
  int level;
  pezzo_t p_cur;
  pezzo_t p_next;
} game_t;


game_t start_new_game();
void add(pezzo_t *p);
void rem(pezzo_t *p);
int check(pezzo_t *p);
void move_left(pezzo_t *p);
void move_right(pezzo_t *p);
int move_down(pezzo_t *p);
void rotate(pezzo_t *p);
void pezzoRand(pezzo_t *p);
void getElement(pezzo_t *p, char str[4][4]);

extern char screen[Y][X];

#endif /* GAME_H */
