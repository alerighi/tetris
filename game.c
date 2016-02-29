#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "screen.h"

static const char tetris[7][4][4][4] = {
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

typedef struct pezzo_s {
  int p;
  int r;
  int y;
  int x;
} pezzo_t;

static pezzo_t current_piece;
static pezzo_t next_piece;

int level;
int score;

static int check(void);
static void pezzo_rand(pezzo_t *p);
static void rem(void);
static void add(void);

static void pezzo_rand(pezzo_t* p){
  p->x=4;
  p->y=3;
  p->p=random()%7;
  p->r=0;
}

void swap_pieces() {
  current_piece = next_piece;
  pezzo_rand(&next_piece);
  add();
}

void start_new_game() {
  memset(screen, 0, sizeof(screen));
  pezzo_rand(&current_piece);
  pezzo_rand(&next_piece);
  score = 0;
  level = 1;
  add();
  refresh_screen();
}

static void add(void){
  int i,e;
  for (i=0; i<4; i++){
    for (e=0; e<4; e++){
      if (tetris[current_piece.p][current_piece.r][i][e]=='*')
      screen[current_piece.y-i][current_piece.x+e]=current_piece.p+1;
    }
  }
}

static void rem(void){
  int i,e;
  for (i=0; i<4; i++){
    for (e=0; e<4; e++){
      if (tetris[current_piece.p][current_piece.r][i][e]=='*')
      screen[current_piece.y-i][current_piece.x+e]=0;
    }
  }
}

static int check(void){
  int i,e;
  for (i=0; i<4; i++){
    for (e=0; e<4; e++){
      if (tetris[current_piece.p][current_piece.r][i][e]=='*' &&
      ((current_piece.y-i)>=Y || (current_piece.y-i)<0 || (current_piece.x+e)>=X || (current_piece.x+e)<0 || screen[current_piece.y-i][current_piece.x+e])){
        return 0;
      }
    }
  }
  return 1;
}

void get_next(char str[4][4]){
  int i,e;
  for (i=0;i<4;i++)
  for (e=0;e<4;e++){
    if (tetris[next_piece.p][next_piece.r][i][e]=='*')
    str[i][e]=next_piece.p+1;
    else
    str[i][e]=0;
  }
}


void move_left(void){
  rem();
  current_piece.x--;
  if (!check())
  current_piece.x++;
  add();
}

void move_right(void){
  rem();
  current_piece.x++;
  if (!check())
  current_piece.x--;
  add();
}

void rotate(void){
  rem();
  current_piece.r++;
  if (current_piece.r>3)  current_piece.r=0;
  if (!check()){
    current_piece.r--;
    if (current_piece.r<0)  current_piece.r=3;
  }
  add();
}

int move_down(void){
  int ret=0;
  rem();
  current_piece.y++;
  if (!check()){
    ret=1;
    current_piece.y--;
  }
  add();
  return ret;
}
