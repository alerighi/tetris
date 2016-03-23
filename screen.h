#ifndef SCREEN_H
#define SCREEN_H

#define Y 22
#define X 10

extern char screen[Y][X];

int game_is_lost(void);
int eliminate_line(void);
void prompt_new_game(void);
void quit();
void init_curses(void);
void refresh_screen(void);

#endif /* SCREEN_H */
