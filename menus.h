#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#ifndef LIFE_MENUS_H_
#define LIFE_MENUS_H_

int print_copying_warranty (WINDOW * win);
int print_help (WINDOW * win);
int read_num (WINDOW * win, int min, int max);
int fancy_rules (WINDOW * win, int ruleint, int speed);

#endif // LIFE_MENUS_H_
