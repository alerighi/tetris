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

#include <stdlib.h> /* for rand() */

#include "pieces.h"

const char tetris[7][4][4][4] = {
	{ /* I */
		{{1},{1},{1},{1}},
		{{1,1,1,1}},
		{{1},{1},{1},{1}},
		{{1,1,1,1}},
	},
	{ /* O */
		{{1,1},{1,1},{0},{0}},
		{{1,1},{1,1},{0},{0}},
		{{1,1},{1,1},{0},{0}},
		{{1,1},{1,1},{0},{0}},
	},
	{ /* L */
		{{1,1},{1},{1},{0}},
		{{1,1,1},{0,0,1},{0},{0}},
		{{0,1},{0,1},{1,1},{0}},
		{{1},{1,1,1},{0},{0}},
	},
	{ /* J */
		{{1,1},{0,1},{0,1},{0}},
		{{0,0,1},{1,1,1},{0},{0}},
		{{1},{1},{1,1},{0}},
		{{1,1,1},{1},{0},{0}},
	},
	{ /* T */
		{{0,1},{1,1,1},{0},{0}},
		{{1},{1,1},{1},{0}},
		{{1,1,1},{0,1},{0},{0}},
		{{0,1},{1,1},{0,1},{0}},
	},
	{ /* S */
		{{0,1},{1,1},{1},{0}},
		{{1,1},{0,1,1},{0},{0}},
		{{0,1},{1,1},{1},{0}},
		{{1,1},{0,1,1},{0},{0}},
	},
	{ /* Z */
		{{1},{1,1},{0,1},{0}},
		{{0,1,1},{1,1},{0},{0}},
		{{1},{1,1},{0,1},{0}},
		{{0,1,1},{1,1},{0},{0}},
	},
};

struct piece
get_random_piece(void)
{
	struct piece p = {
		.piece = rand() % NUMBER_OF_PIECES,
		.rotation = ROTATION_NORMAL,
		.position = {
			.x = 4,
			.y = 3,
		},
	};

	return p;
}
