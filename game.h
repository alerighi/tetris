#ifndef GAME_H
#define GAME_H

extern int level;
extern int score;


static  int check(void);
int move_down(void);
static void add(void);
static void rem(void);
void move_left(void);
void move_right(void);
void rotate(void);
void get_next(char str[4][4]);
void start_new_game(void);
void swap_pieces(void);

#endif /* GAME_H */
