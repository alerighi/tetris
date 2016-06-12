#ifndef GAME_H
#define GAME_H

extern int level;
extern int score;
extern int high_score;

int move_down(void);
void move_left(void);
void move_right(void);
void rotate(void);
void get_next(char str[4][4]);
void start_new_game(void);
void swap_pieces(void);
void load_score(void);
void save_score(void);

#endif /* GAME_H */
