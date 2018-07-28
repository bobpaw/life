/*
    An arbitrary cellular automata model using ncurses
    Copyright (C) 2018 Aiden Woodruff

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "life.h"

// Declare TRUE and FALSE
#ifndef TRUE
#define TRUE (1==1)
#define FALSE (!TRUE)
#endif

int main (int argc, char * argv[]) {
  unsigned int RULE = 0;
  struct gengetopt_args_info args_info;
  if (cmdline_parser(argc, argv, &args_info) != 0) exit(EXIT_FAILURE);
  initscr();
  raw();
  curs_set(0);
  keypad(stdscr, TRUE);
  noecho();
  RULE = args_info.ruleint_arg;
  srand(time(NULL));
  int ch = 0;
  int width = 36;
  int height = 18;
  int playing = 0;
  int delaymax = 10;
  int delay = 0; // Counts up to 10 so display is slow
  unsigned long long generation = 0; // Store current generation number
  int livecell = '#';
  int deadcell = '.';
  if (args_info.width_given) width = args_info.width_arg;
  if (args_info.height_given) height = args_info.height_arg;
  if (args_info.delay_given) delaymax = args_info.delay_arg;
  if (args_info.live_given) {
    if (strlen(args_info.live_arg) > 1) {
      fprintf(stderr, "Live character must be one character long");
      exit(EXIT_FAILURE);
    } else {
      livecell = (int) args_info.live_arg[0];
    }
  }
  if (args_info.dead_given) {
    if (strlen(args_info.dead_arg) > 1) {
      fprintf(stderr, "Dead character must be one character long");
      exit(EXIT_FAILURE);
    } else {
      deadcell = (int) args_info.dead_arg[0];
    }
  }
  if (args_info.maximize_given) {
    getmaxyx(stdscr, height, width);
    width -= 1;
    height -= 2;
  }
  cmdline_parser_free(&args_info);
  char * map = NULL;
  map = malloc((height * width)+1);
  memset(map, 0, height * width + 1);
  memset(map, deadcell, height * width);
  int x = width/2;
  int y = height/2;
  while (ch != 'q') {
    erase();
    if (playing == FALSE) {
      if (ch == KEY_UP) {
        if (y > 0) y--;
      } else if (ch == KEY_DOWN) {
        if (y < height - 1) y++;
      } else if (ch == KEY_LEFT) {
        if (x > 0) x--;
      } else if (ch == KEY_RIGHT) {
        if (x < width - 1) x++;
      } else if (ch == KEY_A1) {
        if (x > 0) x--;
        if (y > 0) y--;
      } else if (ch == KEY_A3) {
        if (x < width - 1) x++;
        if (y > 0) y--;
      } else if (ch == KEY_C1) {
        if (x > 0) x--;
        if (y < height - 1) y++;
      } else if (ch == KEY_C3) {
        if (x < width - 1) x++;
        if (y < height - 1) y++;
      } else if (ch == ' ') {
        if (map[y*width+x] == deadcell) {
          map[y*width+x] = livecell;
        } else if (map[y*width+x] == livecell) {
          map[(y*width)+x] = deadcell;
        }
      } else if (ch == '\n') {
        playing = TRUE;
        timeout(50);
      } else if (ch == 'c') {
        memset(map, deadcell, width*height);
      } else if (ch == '.') {
        delaymax -= (delaymax > 1 ? 1 : 0);
      } else if (ch == ',') {
        delaymax += (delaymax < 20 ? 1 : 0);
      }
    } else {
      if (ch == '\n') {
        playing = FALSE;
        timeout(-1);
      } else if (ch == '.') {
        delaymax -= (delaymax > 1 ? 1 : 0);
      } else if (ch == ',') {
        delaymax += (delaymax < 20 ? 1 : 0);
      }
    }
    for (int i = 0; i < height; ++i) {
      if (i == y) {
        printw("%.*s", x, map + (y * width));
        standout();
        printw("%.1s", map + (y * width) + x);
        standend();
        printw("%.*s\n", width-(x+1), (map + (i * width) + x + 1));
      } else {
        printw("%.*s\n", width, (map + (i * width)));
      }
    }
    printw("(%d, %d) Generation: %d\tDelay Time: %d\n", x, y, generation, delaymax);
    refresh();
    if (playing == TRUE) {
      delay++;
      if (delay >= delaymax) {
        delay = 0;
        generation++;
        update_map(map, width, height, livecell, deadcell, RULE);
      }
    }
    ch = getch();
  }
  endwin();
  free(map);
  exit(EXIT_SUCCESS);
}
