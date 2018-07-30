#include "menus.h"

#ifndef has
#define has(var, bit) ((var & bit) == bit)
#endif

int print_copying_warranty (WINDOW * win) {
  const char * WARRANTY = "  THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY\nAPPLICABLE LAW.\n  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT\nHOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY\nOF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,\nTHE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\nPURPOSE.\n  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM\nIS WITH YOU.\n  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF\nALL NECESSARY SERVICING, REPAIR OR CORRECTION.";
  return waddstr(win, WARRANTY);
}

int print_help (WINDOW * win) {
  if (waddstr(win, "Arrow keys - Motion\n") == OK &&
      waddstr(win, "Space - Flip cell (if it's alive, to dead, and vice versa)\n") == OK &&
      waddstr(win, "Enter - Pause/Play\n") == OK &&
      waddstr(win, "\tN.B. - Many keys can only be used while paused.\n") == OK &&
      waddstr(win, ", - Lower delay time\n") == OK &&
      waddstr(win, ". - Increase delay time\n") == OK &&
      waddstr(win, "R - Input a rule-int\n") == OK &&
      waddstr(win, "Ctrl+R - Open the fancy rule menu\n") == OK &&
      waddstr(win, "Ctrl+L - Redraw screen\n") == OK &&
      waddstr(win, "W - Show warranty\n") == OK &&
      waddstr(win, "V - Print version\n") == OK) {
    return OK;
  } else {
    return ERR;
  }
}

int read_num (WINDOW * win, int min, int max) {
  int ret = 0;
  werase(win);
  int width = getmaxx(win);
  char * text = (char*) malloc((size_t)(width + 1));
  memset(text, 0, (size_t)(width + 1));
  curs_set(1);
  echo();
  wgetnstr(win, text, (size_t)(width + 1));
  ret = (int) strtol((const char *) text, NULL, 10);
  if (!(ret >= min && ret <= max)) {
    ret = 0;
  }
  noecho();
  curs_set(0);
  free(text);
  return ret;
}

// Fancy rule menu, returns new rules or -1 on fail
int fancy_rules (WINDOW * win, int ruleint, int speed) {
  int done = FALSE;
  int cursor = 0;
  int ch = 0;
  wtimeout(win, speed);
  while (done == FALSE && ch != 'q') {
    werase(win);
    waddstr(win, "Press space to toggle buttons\n");
    wprintw(win, "Rule int: %d\n", ruleint);
    waddch(win, 'B');
    for (int i = 0; i < 9; i++) {
      if (cursor == i) {
        wstandout(win);
      }
      waddch(win, has(ruleint, 1 << i) ? '1' : '0');
      if (cursor == i) {
        wstandend(win);
      }
    }
    waddstr(win, "/S");
    for (int i = 9; i < 18; i++) {
      if (cursor == i) {
        wstandout(win);
      }
      waddch(win, has(ruleint, 1 << i) ? '1' : '0');
      if (cursor == i) {
        wstandend(win);
      }
    }
    waddstr(win, "\n");
    if (cursor == 18) {
      wstandout(win);
      waddstr(win, "Cancel");
      wstandend(win);
    } else {
      waddstr(win, "Cancel");
    }
    waddstr(win, "  ");
    if (cursor == 19) {
      wstandout(win);
      waddstr(win, "Default");
      wstandend(win);
    } else {
      waddstr(win, "Default");
    }
    waddstr(win, "  ");
    if (cursor == 20) {
      wstandout(win);
      waddstr(win, "Done");
      wstandend(win);
    } else {
      waddstr(win, "Done");
    }
    wrefresh(win);
    if (ch == KEY_LEFT) {
      if (cursor > 0 && cursor < 18) {
        cursor--;
      } else if (cursor > 18) {
        cursor--;
      }
    } else if (ch == KEY_RIGHT) {
      if (cursor < 17) {
        cursor++;
      } else if (cursor > 17 && cursor < 20) {
        cursor++;
      }
    } else if (ch == KEY_UP || ch == KEY_DOWN) {
      if (cursor > 17) {
        switch (cursor) {
        case 18:
          cursor = 0;
          break;
        case 19:
          cursor = 6;
          break;
        case 20:
          cursor = 13;
          break;
        }
      } else {
        if (cursor > -1 && cursor < 6) {
          cursor = 18;
        } else if (cursor > 5 && cursor < 13) {
          cursor = 19;
        } else if (cursor > 12) {
          cursor = 20;
        }
      }
    } else if (ch == ' ' || ch == '\n') {
      switch (cursor) {
      case 18:
        return -1;
        break;
      case 19:
        ruleint = 6152;
        break;
      case 20:
        return ruleint;
        break;
      default:
        ruleint ^= 1 << cursor;
        break;
      }
    }
    if (done == FALSE) {
      ch = wgetch(win);
    }
  }
  ch = 0;
}
