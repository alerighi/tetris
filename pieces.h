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
