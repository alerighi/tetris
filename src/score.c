/*
 * tetris - a small ncursess tetris game
 * Copyright (C) 2016-2019 Alessandro Righi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "score.h"

int high_score;
int score;
int level;

static char score_filename[1024];

static void
load_score(void)
{
	FILE *score_fp;

	if ((score_fp = fopen(score_filename, "r"))) {
		if (fscanf(score_fp, "%d", &high_score) != 1)
			high_score = 0;
		fclose(score_fp);
	} else {
		/* open fail for other reason than non existing file */
		if (errno != ENOENT)
			perror("error opening high score file in read mode");
	}
}

static void
save_score(void)
{
	FILE *score_fp;

	if ((score_fp = fopen(score_filename, "w"))) {
		fprintf(score_fp, "%d\n", high_score);
		fclose(score_fp);
	} else {
		perror("error opening high score file in write mode");
	}
}

void
init_score(void)
{
	snprintf(score_filename, sizeof score_filename, "%s/.tetris_score", getenv("HOME"));
	load_score();
	atexit(save_score);
}
