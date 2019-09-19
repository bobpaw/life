#include <stdio.h>
#include "board.h"

int main () {
	printf("sizeof(unsigned int): %d\n", sizeof(unsigned int));
	BOARD jim = newboard(10, 10);
	for (unsigned int y = 0; y < 10; ++y) {
		for (unsigned int x = 0; x < 10; ++x)
			putchar(board_getval(jim, x, y) == 0 ? '.' : '#');
		putchar('\n');
	}
	board_setval(jim, 0, 3, 1);
	board_setval(jim, 4, 5, 1);
	for (unsigned int y = 0; y < 10; ++y) {
		for (unsigned int x = 0; x < 10; ++x)
			putchar(board_getval(jim, x, y) == 0 ? '.' : '#');
		putchar('\n');
	}
	delboard(jim);
	return 0;
}