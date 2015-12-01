#ifndef SCREEN_H
#define SCREEN_H

#include <curses.h>
#include <stdlib.h>
#include <string.h>


#define Y 22
#define X 10

/* window positioning stuff */
#define CENTER COLS/2

#define TITLE_CHARS 44
#define TITLE_LINES 5
#define TITLE "######  ######  ######  #######  ##  ######\n"\
              "  ##    ##        ##    ##   ##  ##  ##    \n"\
							"  ##    #####     ##    ######   ##  ######\n"\
							"  ##    ##        ##    ##  ##   ##      ##\n"\
							"  ##    ######    ##    ##   ##  ##  ######\n"

#define TITLE_W_START_X CENTER-TITLE_CHARS/2-5
#define TITLE_W_START_Y 0
#define TITLE_W_SIZE_X TITLE_CHARS
#define TITLE_W_SIZE_Y TITLE_LINES

#define GAME_W_START_X CENTER-X*2
#define GAME_W_START_Y TITLE_W_SIZE_Y+2
#define GAME_W_SIZE_X X*2
#define GAME_W_SIZE_Y Y

#define GAME_O_W_START_X GAME_W_START_X-1
#define GAME_O_W_START_Y GAME_W_START_Y-1
#define GAME_O_W_SIZE_X GAME_W_SIZE_X+2
#define GAME_O_W_SIZE_Y GAME_W_SIZE_Y+2

#define SCORE_W_START_X GAME_W_START_X+GAME_W_SIZE_X+3
#define SCORE_W_START_Y GAME_W_START_Y+2
#define SCORE_W_SIZE_X COLS-(SCORE_W_START_X)
#define SCORE_W_SIZE_Y GAME_W_SIZE_Y-4

WINDOW *game;
WINDOW *game_o;
WINDOW *score_win;
WINDOW *title;

char screen[Y][X];

void init();
void printMatrix();
int eliminateLine();
int lost();
void printScore(int score, int level, char next[4][4]);
int newGame();
void initCurses();
void quit();

#endif /* SCREEN_H */
