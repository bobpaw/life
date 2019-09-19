#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "board.h"

struct BOARD {
	unsigned int* arr;
	unsigned int arr_size;
	unsigned int w;
	unsigned int h;
};

BOARD newboard(unsigned int w, unsigned int h) {
	BOARD ret = (BOARD) malloc(sizeof(struct BOARD));
	ret->w = w;
	ret->h = h;
	// (a / b) + ((a % b) != 0)
	unsigned int bits = sizeof(unsigned int) * 8;
	ret->arr_size = w * h / bits + ((w * h) % bits != 0);
	ret->arr = ( unsigned int*) calloc(ret->arr_size, sizeof(unsigned int));
	return ret;
}

BOARD dupboard (BOARD old) {
	BOARD ret = (BOARD) malloc(sizeof(struct BOARD));
	ret->w = old->w;
	ret->h = old->h;
	ret->arr_size = old->arr_size;
	ret->arr = ( unsigned int*) calloc(old->arr_size, sizeof(unsigned int));
	memcpy(ret->arr, old->arr, old->arr_size * sizeof(*old->arr));
	return ret;
}

// Copy boards of the same size
// For boards of differing sizes, just use dupboard
BOARD cpyboard (BOARD dest, BOARD src) {
	assert(dest != NULL && dest->arr != NULL);
	assert(src != NULL && src->arr != NULL);
	memcpy(dest->arr, src->arr, src->arr_size * sizeof(*src->arr));
	return dest;
}

void delboard(BOARD b) {
	free(b->arr);
	free(b);
}

unsigned int board_getval(BOARD b, unsigned int x, unsigned int y) {
	assert(("board_getval out of bounds", x < b->w && y < b->h));
	unsigned int n = b->w * y + x;
	return (b->arr[n / sizeof(unsigned int) / 8] >> (n % (sizeof(unsigned int) * 8))) & 1;
}

unsigned int board_setval(BOARD b, unsigned int x, unsigned int y, unsigned int val) {
	assert(("board_setval out of bounds", x < b->w && y < b->h));
	unsigned int n = b->w * y + x;
	unsigned int retval = (b->arr[n / sizeof(unsigned int) / 8] >> (n % (sizeof(unsigned int) * 8))) & 1;
	unsigned int pos = n % (sizeof(unsigned int) * 8);
	switch (val) {
	case 0:
		b->arr[n / sizeof(unsigned int) / 8] &= ~(( unsigned int) (1 << pos));
		break;
	case 1:
	default:
		b->arr[n / sizeof(unsigned int) / 8] |= 1 << pos;
		break;
	}
	return retval;
}
unsigned int board_flip(BOARD b, unsigned int x, unsigned int y) {
	assert(("board_flip out of bounds", x < b->w && y < b->h));
	unsigned int n = b->w * y + x;
	unsigned int retval = (b->arr[n / sizeof(unsigned int) / 8] >> (n % (sizeof(unsigned int) * 8))) & 1;
	unsigned int pos = n % (sizeof(unsigned int) * 8);
	b->arr[n / sizeof(unsigned int) / 8] ^= 1 << pos;
	return retval;
}

unsigned int board_getval_i(BOARD b, unsigned int i) {
	assert(("board_getval_i out of bounds", i < b->w * b->h));
	return (b->arr[i / sizeof(unsigned int) / 8] >> (i % (sizeof(unsigned int) * 8))) & 1;
}

unsigned int board_setval_i(BOARD b, unsigned int i, unsigned int val) {
	assert(("board_setval_i out of bounds", i < b->w * b->h));
	unsigned int retval = (b->arr[i / sizeof(unsigned int) / 8] >> (i % (sizeof(unsigned int) * 8))) & 1;
	unsigned int pos = i % (sizeof(unsigned int) * 8);
	switch (val) {
	case 0:
		b->arr[i / sizeof(unsigned int) / 8] &= ~(( unsigned int) (1 << pos));
		break;
	case 1:
	default:
		b->arr[i / sizeof(unsigned int) / 8] |= 1 << pos;
		break;
	}
	return retval;
}

unsigned int board_flip_i(BOARD b, unsigned int i) {
	assert(("board_flip_i out of bounds", i < b->w * b->h));
	unsigned int retval = (b->arr[i / sizeof(unsigned int) / 8] >> (i % (sizeof(unsigned int) * 8))) & 1;
	unsigned int pos = i % (sizeof(unsigned int) * 8);
	b->arr[i / sizeof(unsigned int) / 8] ^= 1 << pos;
	return retval;
}

void board_setall(BOARD b, unsigned int val) {
	assert(b != NULL);
	memset(b->arr, val == 0 ? 0 : 1, b->arr_size * sizeof(*b->arr));
}