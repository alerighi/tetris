#ifndef GAME_H
#define GAME_H

typedef struct pezzo_s {
  int p;
  int r;
  int y;
  int x;
} pezzo_t;


int check(pezzo_t *p);
int move_down(pezzo_t *p);
void add(pezzo_t *p);
void rem(pezzo_t *p);
void move_left(pezzo_t *p);
void move_right(pezzo_t *p);
void rotate(pezzo_t *p);
void pezzo_rand(pezzo_t *p);
void get_element(pezzo_t *p, char str[4][4]);

#endif /* GAME_H */
