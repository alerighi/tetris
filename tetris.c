/*
 * Simple tetris game built using ncurses library
 * (c) 2016 Alessandro Righi
 * You are free to use this program under the terms of the BSD licence
 */

#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "game.h"
#include "screen.h"

#define VERSION "1.0"

static char *HELPMSG =
  "Usage: %s [-hv]\n"
  "\t -h        : Display this help message\n"
  "\t -v        : Display version information\n";

static char *VERSIONMSG =
  "Tetris version v" VERSION "\n"
  "(c) 2016 Alessandro Righi";

int main(int argc, char *argv[]){
  int i;
  int count=0;
  int bottom=0;
  char c;

  signal(SIGINT, quit);
  srand(time(NULL));

  /* Command line args parses */
  while ((c = getopt(argc, argv, "hv")) != -1){
    switch(c){
      case 'h':
        printf(HELPMSG, argv[0]);
        exit(0);
      case 'v':
        puts(VERSIONMSG);
        exit(0);
      default:
        printf(HELPMSG, argv[0]);
        exit(1);
    }
  }

  /* init ncurses */
  init_curses();
  start_new_game();

  /* main loop */
  while (true){

    /* Gets and processes user input */
    switch (getch()){
      case KEY_LEFT:
        move_left();
        break;
      case KEY_RIGHT:
        move_right();
        break;
      case KEY_UP:
        rotate();
        break;
      case KEY_DOWN:
        while (!move_down());
        bottom=1;
        break;
      case 'p': /* pause */
        while (getch()!='p')
        usleep(100000);
        break;
      case 'r':
        start_new_game();
        continue;
      case 'q':
        quit();
    }

    /* Moves down the piece if time is passed */
    count++;
    if (count>=150-pow(level,2)){
      count=0;
      bottom = move_down();
    }

    /* If the piece reaches bottom */
    if (bottom){
      bottom=0;
      score++;
      /* Check and eliminate full lines */
      if ((i=eliminate_line())){
        score += pow(i,2)*7;
        count=0;
      }

      /* Computes level  */
      level = 1 + score / 50;

      /* Checks if the game is lost */
      if (game_is_lost()){
        if (prompt_new_game()){
          start_new_game();
          continue;
        } else {
          quit();
        }
      }

      /* Swaps the current piece whith the next one */
      swap_pieces();
    }

    /* Refreshes the screen and sleeps 2ms */
    refresh_screen();
    usleep(2000);
  }
  return 0;
}
