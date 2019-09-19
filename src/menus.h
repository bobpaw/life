/*

 * An arbitrary cellular automata model using ncurses
 * Copyright (C) 2018 Aiden Woodruff

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include <string.h>
#include <stdlib.h>

#include "correct_curses.h"

#include "life-macros.h"

#ifndef LIFE_MENUS_H_
#define LIFE_MENUS_H_

int print_copying_warranty (WINDOW * win);
int print_help (WINDOW * win);
int read_num (WINDOW * win, int min, int max);
int fancy_rules (WINDOW * win, int ruleint, int speed);

#endif // LIFE_MENUS_H_
