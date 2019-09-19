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
#include "updatemap.h"

void update_map (BOARD base_map, BOARD intermap, const unsigned int width, const unsigned int height, const unsigned int ruleint) {
	cpyboard(intermap, base_map);
	for (int i = 0, x = 0, y = 0, surround = 0; i < width * height; i++, surround = 0) {
		x = i % width;
		y = (i - x) / width;
		/*
		  Where i is the current cell:
		   A1 | A2 | A3
		  ----|----|----
		   B1 | i  | B3
		  ----|----|----
		   C1 | C2 | C3
		*/
		if (x > 0) {
			if (board_getval_i(base_map, i - 1) == 1) ++surround; // B1
			if (y > 0) {
				if (board_getval_i(base_map, i - width - 1) == 1) ++surround; // A1
			}
			if (y < height - 1) {
				if (board_getval_i(base_map, i - 1 + width) == 1) ++surround; // C1
			}
		}
		if (x < width - 1) {
			if (board_getval_i(base_map, i + 1) == 1) ++surround; // B3
			if (y > 0) {
				if (board_getval_i(base_map, i + 1 - width) == 1) ++surround; // A3
			}
			if (y < height - 1) {
				if (board_getval_i(base_map, i + 1 + width) == 1) ++surround; // C3
			}
		}
		if (y > 0) {
			if (board_getval_i(base_map, i - width) == 1) ++surround;
		}
		if (y < height - 1) {
			if (board_getval_i(base_map, i + width) == 1) ++surround;
		}
		switch (board_getval_i(base_map, i)) {
		case 0:
			if (has(ruleint, 1 << surround)) {
				board_setval_i(intermap, i, 1);
			}
			break;
		case 1:
			if (!has(ruleint, 1 << (surround + 9))) {
				board_setval_i(intermap, i, 0);
			}
		}
	}

	// Rewrite map
	cpyboard(base_map, intermap);
	return base_map;
}
