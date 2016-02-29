#ifndef SCREEN_H
#define SCREEN_H

#ifndef Y
# define Y 22
#endif

#ifndef X
# define X 10
#endif

extern char screen[Y][X];

int game_is_lost(void);
int eliminate_line(void);
int prompt_new_game(void);
void quit();
void init_curses(void);
void refresh_screen(void);

#endif /* SCREEN_H */
