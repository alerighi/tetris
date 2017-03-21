/*
 * Tetris pieces definition file
 * TODO: properly rewrite this file whiout string but whit 0s and 1s
 */

static const char tetris[7][4][4][4] = {
  { /* I */
    {"****"},
    {"*","*","*","*"},
    {"****"},
    {"*","*","*","*"}
  },
  { /* O */
    {"**","**","",""},
    {"**","**","",""},
    {"**","**","",""},
    {"**","**","",""}
  },
  { /* L */
    {"**","*","*",""},
    {"***","  *","",""},
    {" *"," *","**",""},
    {"*","***","",""}
  },
  { /* J */
    {"**"," *"," *",""},
    {"  *","***","",""},
    {"*","*","**",""},
    {"***","*","",""}
  },
  { /* T */
    {" *","***","",""},
    {"*","**","*",""},
    {"***"," *","",""},
    {" *","**"," *",""}
  },
  { /* S */
    {" *","**","*",""},
    {"**"," **","",""},
    {" *","**","*",""},
    {"**"," **","",""}
  },
  { /* Z */
    {"*","**"," *",""},
    {" **","**","",""},
    {"*","**"," *",""},
    {" **","**","",""}
  }
};