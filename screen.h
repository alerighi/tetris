#ifndef SCREEN_H
#define SCREEN_H

#define Y 22
#define X 10

/* window positioning stuff */
#define CENTER COLS/2

#define TITLE_CHARS 44
#define TITLE_LINES 5
#define TITLE "111111  222222  333333  4444444  55  666666\n"\
              "  11    22        33    44   44  55  66    \n"\
              "  11    22222     33    444444   55  666666\n"\
              "  11    22        33    44   44  55      66\n"\
              "  11    222222    33    44    44 55  666666\n"

#define COMANDI "\nControls:                   "\
                "\n   arrows: move             "\
                "\n   p: pause                 "\
                "\n   r: restart               "

#define TITLE_W_START_X CENTER-TITLE_CHARS/2-5
#define TITLE_W_START_Y 1
#define TITLE_W_SIZE_X TITLE_CHARS
#define TITLE_W_SIZE_Y TITLE_LINES

#define GAME_W_START_X CENTER-X*2
#define GAME_W_START_Y TITLE_W_SIZE_Y+3
#define GAME_W_SIZE_X X*2
#define GAME_W_SIZE_Y Y

#define GAME_O_W_START_X GAME_W_START_X-1
#define GAME_O_W_START_Y GAME_W_START_Y-1
#define GAME_O_W_SIZE_X GAME_W_SIZE_X+2
#define GAME_O_W_SIZE_Y GAME_W_SIZE_Y+2

#define SCORE_W_START_X GAME_W_START_X+GAME_W_SIZE_X+3
#define SCORE_W_START_Y GAME_W_START_Y
#define SCORE_W_SIZE_X COLS-(SCORE_W_START_X)
#define SCORE_W_SIZE_Y GAME_W_SIZE_Y-4

extern char screen[Y][X];

void print_matrix(void);
int eliminate_line(void);
int lost(void);
void print_score(int score, int level, char next[4][4]);
int new_game(void);
void init_curses(void);
void quit();

#endif /* SCREEN_H */
