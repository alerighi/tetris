/*
* Simple tetris game built using ncurses library
* (c) 2016 Alessandro Righi
* You are free to use this program under the terms of the MIT licence
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include "game.h"
#include "screen.h"

#define VERSION "1.1.0"

#define HELPMSG \
    "Usage: %s [-hv]\n"\
    "\t -h        : Display this help message\n"\
    "\t -v        : Display version information\n"

#define VERSIONMSG \
    "This is Tetris, version v" VERSION "\n"\
    "(c) 2016 Alessandro Righi\n"\
    "This software is free software, relased under the terms of the MIT licence"

int main(int argc, char *argv[])
{
    char c;

    signal(SIGINT, quit);
    signal(SIGWINCH, redraw_screen);
    signal(SIGALRM, update_signal); /* action for move down piece */
    srand(time(NULL));

    /* Command line args parses */
    while ((c = getopt(argc, argv, "hv")) != -1)
    {
        switch(c)
        {
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
    load_score();
    start_new_game();
    input_loop();
    return 0;
}
