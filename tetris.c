/*
 * Simple tetris game built using ncurses library
 * (c) 2016-2018 Alessandro Righi
 * You are free to use this program under the terms of the MIT licence
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include <time.h>

#include "game.h"
#include "screen.h"

#define VERSION "1.3.0"

#define HELPMSG "Usage: %s [-hv]\n"	\
	"\t -h\t display this help message\n"\
	"\t -v\t display version information\n"

#define VERSIONMSG "This is Tetris, version v" VERSION "\n" \
	"(c) 2016 Alessandro Righi\n" 							\
	"This software is free software, relased under the terms of the MIT licence"

static void parse_cmdline(int argc, char *argv[]) 
{
	int opt;
	while ((opt = getopt(argc, argv, "hv")) != -1) {
		switch(opt) {
		case 'h':
			printf(HELPMSG, argv[0]);
			exit(EXIT_SUCCESS);
		case 'v':
			puts(VERSIONMSG);
			exit(EXIT_SUCCESS);
		default:
	   		printf(HELPMSG, argv[0]);
			exit(EXIT_FAILURE);
		}
	}
}

static void signal_handler(int signal) 
{
	switch (signal) {
	case SIGINT:
	case SIGTERM:
		quit();
	case SIGWINCH:
		redraw_screen();
		break;
	case SIGALRM:
		update_on_alarm();
	}
}

int main(int argc, char *argv[])
{
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGWINCH, signal_handler);
	signal(SIGALRM, signal_handler);
	srand((unsigned int) time(NULL));
	parse_cmdline(argc, argv);
	init_curses();
	load_score();
	start_new_game();
	input_loop();
}
