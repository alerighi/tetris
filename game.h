#ifndef GAME_H
#define GAME_H

/* struct that represents one tetris piece in the game */
struct piece_s {
	unsigned char p; /* piece number [0, 6] */
	unsigned char r; /* piece rotation [0, 3] */
	unsigned char y; /* piece y cordinate [0, Y-1] */
	unsigned char x; /* piece x coordinate [0, X-1] */
};

extern int level;
extern int score;
extern struct piece_s current_piece;
extern struct piece_s next_piece;

void move_down(int multi);
void move_left(void);
void move_right(void);
void rotate(void);
void start_new_game(void);
void load_score(void);
void save_score(void);
void update_on_alarm(void);
void game_pause(int active);

#endif /* GAME_H */
