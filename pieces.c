#include "pieces.h"

const char tetris[7][4][4][4] = {
#define X PIECE_I
	{
		{{X,X,X,X}},
		{{X},{X},{X},{X}},
		{{X,X,X,X}},
		{{X},{X},{X},{X}}
	},
#undef X
#define X PIECE_O
	{
		{{X,X},{X,X},{0},{0}},
		{{X,X},{X,X},{0},{0}},
		{{X,X},{X,X},{0},{0}},
		{{X,X},{X,X},{0},{0}}
	},
#undef X
#define X PIECE_L
	{
		{{X,X},{X},{X},{0}},
		{{X,X,X},{0,0,X},{0},{0}},
		{{0,X},{0,X},{X,X},{0}},
		{{X},{X,X,X},{0},{0}}
	},
#undef X
#define X PIECE_J
	{
		{{X,X},{0,X},{0,X},{0}},
		{{0,0,X},{X,X,X},{0},{0}},
		{{X},{X},{X,X},{0}},
		{{X,X,X},{X},{0},{0}}
	},
#undef X
#define X PIECE_T
	{
		{{0,X},{X,X,X},{0},{0}},
		{{X},{X,X},{X},{0}},
		{{X,X,X},{0,X},{0},{0}},
		{{0,X},{X,X},{0,X},{0}}
	},
#undef X
#define X PIECE_S
	{
		{{0,X},{X,X},{X},{0}},
		{{X,X},{0,X,X},{0},{0}},
		{{0,X},{X,X},{X},{0}},
		{{X,X},{0,X,X},{0},{0}}
	},
#undef X
#define X PIECE_Z
	{
		{{X},{X,X},{0,X},{0}},
		{{0,X,X},{X,X},{0},{0}},
		{{X},{X,X},{0,X},{0}},
		{{0,X,X},{X,X},{0},{0}}
	}
#undef X
}; 
