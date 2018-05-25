#ifndef SCREEN_H
#define SCREEN_H

enum {
	Y = 22,
	X = 10,
};

extern char screen[Y][X];

void prompt_new_game(void);
void init_curses(void);
void refresh_screen(void);
void redraw_screen(void);
_Noreturn void input_loop(void);

#endif /* SCREEN_H */
