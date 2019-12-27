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

#ifndef PIECES_H
#define PIECES_H

enum piece_e {
	PIECE_I,
	PIECE_O,
	PIECE_L,
	PIECE_J,
	PIECE_T,
	PIECE_S,
	PIECE_Z,
	NUMBER_OF_PIECES,
};

enum rotation {
	ROTATION_NORMAL,
	ROTATION_LEFT,
	ROTATION_RIGHT,
	ROTATION_REVERSE,
};

struct coordinate {
	short x, y;
};

/* struct that represents one tetris piece in the game */
struct piece {
	enum piece_e piece;
	enum rotation rotation;
	struct coordinate position;
};

extern const char tetris[NUMBER_OF_PIECES][4][4][4];

struct piece get_random_piece(void);

#endif /* PIECES_H */
