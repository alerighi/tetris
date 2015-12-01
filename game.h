#ifndef GAME_H
#define GAME_H

#include "screen.h"

typedef struct pezzo_s {
	int p,r,y,x;
} pezzo_t;

void add(pezzo_t *p);
void rem(pezzo_t *p);
int check(pezzo_t *p);
void move_left(pezzo_t *p);
void move_right(pezzo_t *p);
int move_down(pezzo_t *p);
void rotate(pezzo_t *p);
void pezzoRand(pezzo_t *p);
void getElement(pezzo_t *p, char str[4][4]);

#endif /* GAME_H */