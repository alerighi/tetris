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
  "Usage: %s [-hv] [-c on/off] [-b char]\n"
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
        break;
    }

    count++;
    if (count>=150-pow(level,2)){
      count=0;
      bottom = move_down();
    }

    if (bottom){
      bottom=0;
      score++;
      if ((i=eliminate_line())){
        score += i*14;
        count=0;
      }
      level = 1 + score / 50;
      if (lost()){
        if (prompt_new_game()){
          start_new_game();
          continue;
        } else {
          quit();
        }
      }
      swap_pieces();
    }
    usleep(2000);
    refresh_screen();
  }
  return 0;
}
