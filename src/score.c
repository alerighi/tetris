#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "score.h"

int high_score;
int score;
int level;

static char score_filename[1024];

static void load_score(void) 
{
	FILE* score_fp;

	if ((score_fp = fopen(score_filename, "r"))) {
		fscanf(score_fp, "%d", &high_score);
		fclose(score_fp);
	} else {
		/* open fail for other reason than non existing file */
		if (errno != ENOENT)
			perror("error opening high score file in read mode");
	}
}

static void save_score(void) 
{
	FILE* score_fp;

	if ((score_fp = fopen(score_filename, "w"))) {
		fprintf(score_fp, "%d\n", high_score);
		fclose(score_fp);
	} else {
		perror("error opening high score file in write mode");
	}
}

void init_score(void) 
{
	snprintf(score_filename, sizeof score_filename, "%s/.tetris_score", getenv("HOME"));
	load_score();
	atexit(save_score);
}
