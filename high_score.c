#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "high_score.h"

int high_score; 

static char *get_score_filename()
{
	static char score_filename[1024];

	if (score_filename[0] == '\0') /* if string empty, initialize it */
		sprintf(score_filename, "%s/.tetris_score", getenv("HOME"));
	
	return score_filename;
}

void load_score() 
{
	FILE *score_fp;

	if (access(get_score_filename(), F_OK) == -1) 
		return; // high score file doesn't exist

	if ((score_fp = fopen(get_score_filename(), "r"))) {
		fscanf(score_fp, "%d", &high_score);
		fclose(score_fp);
	} else {
		perror("load_score() :: error opening high score file");
	}
}

void save_score() 
{
	FILE *score_fp;

	if ((score_fp = fopen(get_score_filename(), "w+"))) {
		fprintf(score_fp, "%d\n", high_score);
		fclose(score_fp);
	} else {
		perror("save_score() :: error opening high score file");
	}
}
