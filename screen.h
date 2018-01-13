#ifndef SCREEN_H
#define SCREEN_H

#define Y 22
#define X 10

extern char screen[Y][X];

void prompt_new_game(void);
_Noreturn void quit(void);
void init_curses(void);
void refresh_screen(void);
void redraw_screen(void);
_Noreturn void input_loop(void);

#endif /* SCREEN_H */
