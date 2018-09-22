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
#define stat_bar_print(win,...)                                         \
  mvwprintw(win, 0, 0, __VA_ARGS__);                                    \
  for (int _i = getcurx(win); _i < getmaxx(win); _i++) waddch(win, ' ');
#endif

#ifndef CTRL
#define CTRL(ch) ((ch) & 037)
#endif

int main (int argc, char * argv[]) {
  srand(time(NULL));
  unsigned int RULE = 0;
  int ruleint = 0;
  int ch = 0;
  int livecell = '#';
  int deadcell = '.';
  int timeout_val = 50;
  int width = 36;
  int height = 18;
  int x = 0;
  int y = 0;
  int playing = 0;
  int delaymax = 10;
  int delay = 0;
  unsigned int generation = 0;
  WINDOW * board = NULL;
  WINDOW * stat_bar = NULL;
  WINDOW * entry = NULL;
  WINDOW * rule_entry_box = NULL;
  WINDOW * rule_entry = NULL;
  char * map = NULL;
  struct gengetopt_args_info args_info;
  if (cmdline_parser(argc, argv, &args_info) != 0) {
    fprintf(stderr, "Couldn't correctly parse commandline arguments.\n");
    exit(EXIT_FAILURE);
  }
  initscr();
  raw();
  curs_set(0);
  noecho();
  RULE = args_info.ruleint_arg;
  if (args_info.width_given) {
    if (args_info.width_arg > COLS) {
      width = COLS;
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
      fprintf(stderr, "Live character must be one character long.\n");
      endwin();
      exit(EXIT_FAILURE);
    } else {
      livecell = (isprint((int) args_info.live_arg[0]) != 0 ? (int) args_info.live_arg[0] : livecell);
    }
  }
  if (args_info.dead_given) {
    if (strlen(args_info.dead_arg) > 1) {
      fprintf(stderr, "Dead character must be one character long.\n");
      endwin();
      exit(EXIT_FAILURE);
    } else {
      deadcell = (isprint((int) args_info.dead_arg[0]) != 0 ? (int) args_info.dead_arg[0] : deadcell);
    }
  }
  if (args_info.maximize_given) {
    width = COLS;
    height = LINES - 2;
  }
  cmdline_parser_free(&args_info);
  board = newwin(LINES - 2, COLS, 0, 0);
  stat_bar = newwin(1, 0, LINES - 2, 0);
  entry = newwin(1, 0, LINES - 1, 0);
  rule_entry_box = newwin(10, 36, (LINES >> 1) - 5, (COLS >> 1) - 18);
  wborder(rule_entry_box, 0, 0, 0, 0, 0, 0, 0, 0);
  rule_entry = derwin(rule_entry_box, 8, 34, 1, 1);
  keypad(rule_entry, RULE);
  keypad(board, TRUE);
  wstandout(stat_bar);
  map = malloc((height * width)+1);
  memset(map, 0, height * width + 1);
  memset(map, deadcell, height * width);
  x = width >> 2;
  y = height >> 2;
  while (ch != 'q') {
    foreach(werase, stdscr, board, stat_bar, entry);
    if (playing == FALSE) {
      // Numpad and arrow directions
      switch(ch) {
      case KEY_UP:
        if (y > 0) y--;
        break;
      case KEY_DOWN:
        if (y < height - 1) y++;
        break;
      case KEY_LEFT:
        if (x > 0) x--;
        break;
      case KEY_RIGHT:
        if (x < width - 1) x++;
        break;
      case KEY_A1:
        if (x > 0) x--;
        if (y > 0) y--;
        break;
      case KEY_A3:
        if (x < width - 1) x++;
        if (y > 0) y--;
        break;
      case KEY_C1:
        if (x > 0) x--;
        if (y < height - 1) y++;
        break;
      case KEY_C3:
        if (x < width - 1) x++;
        if (y < height - 1) y++;
        break;
      case ' ':
        if (map[y*width+x] == deadcell) {
          map[y*width+x] = livecell;
        } else if (map[y*width+x] == livecell) {
          map[(y*width)+x] = deadcell;
        }
        break;
      case '\n':
        playing = TRUE;
        wtimeout(board, timeout_val);
        break;
      case 'c':
        memset(map, deadcell, width*height);
        break;
      case 'r':
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
        break;
      case 'h':
        wtimeout(board, -1);
        foreach(werase, stdscr, board, stat_bar);
        stat_bar_print(stat_bar, "Press any key to return");
        print_help(board);
        foreach(wnoutrefresh, stdscr, board, stat_bar);
        doupdate();
        getch();
        wtimeout(board, timeout_val);
        break;
      case 'w':
        wtimeout(board, -1);
        foreach(werase, stdscr, board, stat_bar, entry);
        stat_bar_print(stat_bar, "Press any key to return");
        print_copying_warranty(board);
        foreach(wnoutrefresh, stdscr, entry, stat_bar);
        doupdate();
        getch();
        wtimeout(board, timeout_val);
        break;
      case CTRL('r'):
        foreach(werase, stdscr, board, stat_bar, entry);
        stat_bar_print(stat_bar, "Click Cancel to exit or Done to exit and save rule");
        wrefresh(stat_bar);
        wrefresh(rule_entry_box);
        ruleint = fancy_rules(rule_entry, RULE, timeout_val);
        if (ruleint != -1) {
          RULE = ruleint;
        }
        break;
      default:
        // Nothing
        break;
      }
    } else { // ie PLAYING == TRUE
      if (ch == '\n') {
        playing = FALSE;
        wtimeout(board, -1);
      }
    }

    // Commands that don't depend on state
    if (ch == ',') {
      delaymax -= (delaymax > 1 ? 1 : 0);
    } else if (ch == '.') {
      delaymax += (delaymax < 20 ? 1 : 0);
    } else if (ch== CTRL('l')) {
      foreach(wclear, stdscr, entry, stat_bar, board);
    }
    for (int i = 0; i < height; ++i) {
      mvwaddnstr(board, i, 0, map + (i * width), width);
    }
    mvwchgat(board, y, x, 1, A_STANDOUT, COLOR_PAIR(0), NULL);
    stat_bar_print(stat_bar, "(%d, %d)\t\tGeneration: %d\t\tDelay Time: %d", x, y, generation, delaymax);
    foreach(wnoutrefresh, stdscr, board, entry, stat_bar);
    doupdate();
    if (playing == TRUE) {
      delay++;
      if (delay >= delaymax) {
        delay = 0;
        generation++;
        update_map(map, width, height, livecell, deadcell, RULE);
      }
    }
    ch = wgetch(board);
    ch = tolower(ch);
  }
  foreach(werase, stdscr, board, stat_bar, entry, rule_entry, rule_entry_box);
  foreach(delwin, entry, stat_bar, board, rule_entry, rule_entry_box);
  board = stat_bar = entry = rule_entry = rule_entry_box = NULL;
  endwin();
  delwin(stdscr);
  free(map);
  map = NULL;
  return 0;
}
