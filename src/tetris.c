/*
 * Simple tetris game built using ncurses library
 * (c) 2016-2018 Alessandro Righi
 * You are free to use this program under the terms of the MIT licence
 */

#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <time.h>

#include "game.h"
#include "screen.h"
#include "score.h"

static const char HELPMSG[] = "Usage: tetris [-hv]\n"
	"\t -h\t display this help message\n"
	"\t -v\t display version information";

static const char VERSIONMSG[] = "This is Tetris, version v1.4.0\n"
	"(c) 2016-2018 Alessandro Righi\n"
	"This software is free software, relased under the terms of the MIT licence";

static void parse_cmdline(int argc, char *argv[]) 
{
	int opt;
	while ((opt = getopt(argc, argv, "hv")) != -1) {
		switch(opt) {
		case 'h':
			puts(HELPMSG);
			exit(EXIT_SUCCESS);
		case 'v':
			puts(VERSIONMSG);
			exit(EXIT_SUCCESS);
		default:
	   		puts(HELPMSG);
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char *argv[])
{
	srand((unsigned int) time(NULL));
	parse_cmdline(argc, argv);
	init_score();
	start_new_game();
	init_graphics();
	main_loop();
	return EXIT_SUCCESS;
}
