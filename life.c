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
#include "life.h"

#ifndef stat_bar_print
// Print text to status bar in the nice way, without erasing or refreshing
#define stat_bar_print(win,fmt,...)                                     \
  wprintw(win, fmt, ##__VA_ARGS__);                                     \
  for (int _i = getcurx(win); _i < COLS; _i++) waddch(win, ' ');
#endif

#ifndef foreach
#define foreach(func,count,first,...)                           \
  {typeof(first) things[] = {__VA_ARGS__};                      \
    func(first);                                                \
    for (int _i = 0; _i < count - 1; _i++) func(things[_i]);}
#endif

int read_num (WINDOW * win, int min, int max) {
  int ret = 0;
  werase(win);
  char * text = (char*) malloc((size_t)(COLS + 1));
  memset(text, 0, (size_t)(COLS + 1));
  curs_set(1);
  echo();
  wgetnstr(win, text, (size_t)(COLS + 1));
  ret = (int) strtol((const char *) text, NULL, 10);
  if (!(ret >= min && ret <= max)) {
    ret = 0;
  }
  noecho();
  curs_set(0);
  free(text);
  return ret;
}

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
  int timeout_val = 50;
  int width = 36;
  int height = 18;
  int playing = 0;
  int delaymax = 10;
  int delay = 0; // Counts up to 10 so display is slow
  unsigned long long generation = 0; // Store current generation number
  int livecell = '#';
  int deadcell = '.';
  if (args_info.width_given) {
    if (args_info.width_arg > COLS - 1) {
      width = COLS - 1;
    } else if (args_info.width_arg < 1) {
      width = 1;
    } else {
      width = args_info.width_arg;
    }
  }
  if (args_info.height_given) {
    if (args_info.height_arg > LINES - 2) {
      height = LINES - 2;
    } else if (args_info.height_arg < 1) {
      height = 1;
    } else {
      height = args_info.height_arg;
    }
  }
  if (args_info.delay_given) {
    if (args_info.delay_arg > 20) {
      delaymax = 20;
    } else if (args_info.delay_arg < 1) {
      delaymax = 1;
    } else {
      delaymax = args_info.delay_arg;
    }
  }
  if (args_info.live_given) {
    if (strlen(args_info.live_arg) > 1) {
      fprintf(stderr, "Live character must be one character long");
      exit(EXIT_FAILURE);
    } else {
      livecell = (isprint((int) args_info.live_arg[0]) != 0 ? (int) args_info.live_arg[0] : livecell);
    }
  }
  if (args_info.dead_given) {
    if (strlen(args_info.dead_arg) > 1) {
      fprintf(stderr, "Dead character must be one character long");
      exit(EXIT_FAILURE);
    } else {
      deadcell = (isprint((int) args_info.dead_arg[0]) != 0 ? (int) args_info.dead_arg[0] : deadcell);
    }
  }
  if (args_info.maximize_given) {
    width = COLS - 1;
    height = LINES - 2;
  }
  cmdline_parser_free(&args_info);
  WINDOW * board = newwin(height, width + 1, 0, 0);
  WINDOW * stat_bar = newwin(1, 0, LINES - 2, 0);
  WINDOW * entry = newwin(1, 0, LINES - 1, 0);
  wstandout(stat_bar);
  char * map = NULL;
  map = malloc((height * width)+1);
  memset(map, 0, height * width + 1);
  memset(map, deadcell, height * width);
  int x = width/2;
  int y = height/2;
  while (ch != 'q') {
    erase();
    werase(board);
    werase(stat_bar);
    werase(entry);
    if (playing == FALSE) {
      // Numpad and arrow directions
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
        timeout(timeout_val);
      } else if (ch == 'c') {
        memset(map, deadcell, width*height);
      } else if (ch == 'r') {
        werase(stat_bar);
        stat_bar_print(stat_bar, "Rule int - Current: %d - Default: 6152 - Max: 262143", RULE);
        wrefresh(stat_bar);
        {
          int old_rule = RULE;
          RULE = read_num(entry, 0, 262143);
          if (RULE == 0) {
            RULE = old_rule;
          }
        }
        werase(stat_bar);
        werase(entry);
      } else if (keyname(ch) == "^R") {
        // Fancy rule selection
        // TODO (aiden.woodruff@gmail.com) Write
      } else if (ch == 'h') {
        timeout(-1);
        erase();
        foreach(werase, 3, board, stat_bar, entry);
        stat_bar_print(stat_bar, "Press any key to return");
        printw("Arrow keys - Motion\n");
        printw("Space - Flip cell (if it's alive, to dead, and vice versa)\n");
        printw("Enter - Pause/Play\n");
        printw("\tN.B. - Many keys can only be used while paused.\n");
        printw(", - Lower delay time\n");
        printw(". - Increase delay time\n");
        printw("R - Input a rule-int\n");
        // TODO printw("Ctrl+R - Open the fancy rule menu\n");
        printw("Ctrl+L - Redraw screen\n");
        printw("W - Show warranty\n");
        printw("V - Print version\n");
        foreach(wnoutrefresh, 3, stdscr, entry, stat_bar);
        doupdate();
        getch();
        timeout(timeout_val);
      } else if (ch == 'w') {
        timeout(-1);
        foreach(werase, 4, stdscr, board, stat_bar, entry);
        stat_bar_print(stat_bar, "Press any key to return");
        print_copying_warranty(stdscr);
        foreach(wnoutrefresh, 3, stdscr, entry, stat_bar);
        doupdate();
        getch();
        timeout(timeout_val);
      }
    } else { // ie PLAYING == TRUE
      if (ch == '\n') {
        playing = FALSE;
        timeout(-1);
      }
    }

    // Commands that don't depend on state
    if (ch == ',') {
      delaymax -= (delaymax > 1 ? 1 : 0);
    } else if (ch == '.') {
      delaymax += (delaymax < 20 ? 1 : 0);
    } else if (strcmp(keyname(ch), "^L") == 0) {
      foreach(wclear, 4, stdscr, entry, stat_bar, board);
    }
    for (int i = 0; i < height; ++i) {
      if (i == y) {
        wprintw(board, "%.*s", x, map + (y * width));
        wstandout(board);
        wprintw(board, "%.1s", map + (y * width) + x);
        wstandend(board);
        wprintw(board, "%.*s\n", width-(x+1), (map + (i * width) + x + 1));
      } else {
        wprintw(board, "%.*s\n", width, (map + (i * width)));
      }
    }
    stat_bar_print(stat_bar, "(%d, %d)\t\tGeneration: %d\t\tDelay Time: %d", x, y, generation, delaymax);
    foreach(wnoutrefresh, 4, stdscr, board, entry, stat_bar);
    doupdate();
    if (playing == TRUE) {
      delay++;
      if (delay >= delaymax) {
        delay = 0;
        generation++;
        update_map(map, width, height, livecell, deadcell, RULE);
      }
    }
    ch = getch();
    if (ch <= 'Z' && ch >= 'A') {
      ch -= ('A' - 'a');
    }
  }
  foreach(werase, 4, stdscr, board, stat_bar, entry);
  foreach(delwin, 3, entry, stat_bar, board);
  board = stat_bar = entry = NULL;
  endwin();
  delwin(stdscr);
  free(map);
  map = NULL;
  exit(EXIT_SUCCESS);
}
