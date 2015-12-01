#include "game.h"

const char tetris[7][4][4][4] = {
	{ // I
		{"****"},
		{"*","*","*","*"},
		{"****"},
		{"*","*","*","*"}
	},
	{ // 0
		{"**","**","",""},
		{"**","**","",""},
		{"**","**","",""},
		{"**","**","",""}
	},
	{ // L
		{"**","*","*",""},
		{"***","  *","",""},
		{" *"," *","**",""},
		{"*","***","",""}
	},
	{ // j
		{"**"," *"," *",""},
		{"  *","***","",""},
		{"*","*","**",""},
		{"***","*","",""}
	},
	{ // T
		{" *","***","",""},
		{"*","**","*",""},
		{"***"," *","",""},
		{" *","**"," *",""}
	},
	{ // S
		{" *","**","*",""},
		{"**"," **","",""},
		{" *","**","*",""},
		{"**"," **","",""}
	},
	{ // Z
		{"*","**"," *",""},
		{" **","**","",""},
		{"*","**"," *",""},
		{" **","**","",""}
	}
};


void pezzoRand(pezzo_t* p){
	p->x=4;
	p->y=3;
	p->p=random()%7;
	p->r=0;
}

void add(pezzo_t *p){
		int i,e;
		for (i=0; i<4; i++){
			for (e=0; e<4; e++){
				if (tetris[p->p][p->r][i][e]=='*')
					screen[p->y-i][p->x+e]='#';
			}
		}
}

void rem(pezzo_t *p){
		int i,e;
		for (i=0; i<4; i++){
			for (e=0; e<4; e++){
				if (tetris[p->p][p->r][i][e]=='*')
					screen[p->y-i][p->x+e]=' ';
			}
		}
}

int check(pezzo_t *p){
		int i,e;
		for (i=0; i<4; i++){
			for (e=0; e<4; e++){
				if (tetris[p->p][p->r][i][e]=='*' &&
					((p->y-i)>=Y || (p->y-i)<0 || (p->x+e)>=X || (p->x+e)<0 || screen[p->y-i][p->x+e]=='#')){
					return 0;
				}
			}
		}
		return 1;
}

void getElement(pezzo_t *p, char str[4][4]){
	int i,e;
	for (i=0;i<4;i++)
		for (e=0;e<4;e++){
			if (tetris[p->p][p->r][i][e]=='*')
				str[i][e]='#';
			else
				str[i][e]=' ';
		}
}


void move_left(pezzo_t *p){
	rem(p);
  p->x--;
	if (!check(p))
		p->x++;
	add(p);
}

void move_right(pezzo_t *p){
	rem(p);
  p->x++;
	if (!check(p))
		p->x--;
	add(p);
}

void rotate(pezzo_t *p){
	rem(p);
	p->r++;
	if (p->r>3)	p->r=0;
	if (!check(p)){
		p->r--;
		if (p->r<0)	p->r=3;
	}
	add(p);
}

int move_down(pezzo_t *p){
	int ret=0;
	rem(p);
	p->y++;
	if (!check(p)){
		ret=1;
		p->y--;
	}
	add(p);
	return ret;
}
