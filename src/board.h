#ifndef LIFE_BOARD_H_
#define LIFE_BOARD_H_
// Everything assumes there are 8 bits per byte

struct BOARD;
typedef struct BOARD* BOARD;

BOARD newboard(unsigned int w, unsigned int h);
BOARD dupboard(BOARD old);
BOARD cpyboard(BOARD dest, BOARD src);
void delboard(BOARD b);
unsigned int board_getval(BOARD b, unsigned int x, unsigned int y);
unsigned int board_setval(BOARD b, unsigned int x, unsigned int y, unsigned int val);
unsigned int board_flip (BOARD b, unsigned int x, unsigned int y);

// Variant of the regular functions which takes i instead of (x, y)
unsigned int board_getval_i(BOARD b, unsigned int i);
unsigned int board_setval_i(BOARD b, unsigned int i, unsigned int val);
unsigned int board_flip_i(BOARD b, unsigned int i);

void board_setall(BOARD b, unsigned int val);

#endif // LIFE_BOARD_H_