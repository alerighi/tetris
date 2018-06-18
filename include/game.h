#ifndef GAME_H
#define GAME_H

extern int delay;

void move_down(void);
void move_bottom(void);
void move_left(void);
void move_right(void);
void rotate(void);
void start_new_game(void);
void get_next_piece(char next[static 4][4]);

#endif /* GAME_H */
