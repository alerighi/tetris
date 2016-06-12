/*
 * Simple tetris game built using ncurses library
 * (c) 2016 Alessandro Righi
 * You are free to use this program under the terms of the MIT licence
 */

#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <math.h>

#include "game.h"
#include "screen.h"

#define VERSION "1.1.0"


static char *HELPMSG =
  "Usage: %s [-hv]\n"
  "\t -h        : Display this help message\n"
  "\t -v        : Display version information\n";

static char *VERSIONMSG =
  "This is Tetris, version v" VERSION "\n"
  "(c) 2016 Alessandro Righi\n"
  "Tetris name is a trademark of some russian company, i don't own any rights\n"
  "This software is free software, relased under the terms of the MIT licence";

int main(int argc, char *argv[]){
  int i;
  int count = 0;
  int bottom = 0;
  char c;

  signal(SIGINT, quit);
  signal(SIGWINCH, redraw_screen);
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
      case KEY_UP: /* rotate */
        rotate();
        break;
      case KEY_DOWN: /* fast down */
        while (!move_down());
        bottom=1;
        break;
      case 'p': /* pause */
        while (getch()!='p')
          usleep(100000);
        break;
      case 'r': /* restart */
        start_new_game();
        continue;
      case 'q': /* quit */
        quit();
    }

    load_score();

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
        score += pow(i, 2) * 4;
        count=0;
      }

      /* Computes level */
      level = 1 + score / 50;

      /* Checks if the game is lost */
      if (game_is_lost()){
        prompt_new_game();
        continue;
      }

      /* Swaps the current piece whith the next one */
      swap_pieces();
    }

    /* Refreshes the screen and sleeps 2ms */
    refresh_screen();
    usleep(2000);

  } /* end while(true) */

  return 0;
}
