#ifndef GAME_H
#define GAME_H

struct piece_s {
	int p;
	int r;
	int y;
	int x;
};

extern int level;
extern int score;
extern int high_score;
extern struct piece_s current_piece;
extern struct piece_s next_piece;

void move_down(int multi);
void move_left(void);
void move_right(void);
void rotate(void);
void start_new_game(void);
void swap_pieces(void);
void load_score(void);
void save_score(void);
void pause_game(void);
void update_signal(int signal);
void move_bottom(void);

#endif /* GAME_H */
